#!/usr/bin/env python
# encoding: utf-8

import os
import thread
import requests
import BeautifulSoup as bs

class DoubanBackuper(object):
    cookie_str = ""
    cookies_dict = {}
    login_url = "https://www.douban.com/accounts/login"
    rating_level_map = {
            "rating1-t": "*",
            "rating2-t": "**",
            "rating3-t": "***",
            "rating4-t": "****",
            "rating5-t": "*****",
            }

    def __init__(self):
        self.root = os.path.dirname(os.path.abspath(__file__))

    def get_cookie_by_file(self, filepath="cookie.txt"):
        if not os.path.exists(filepath) or not os.path.isfile(filepath):
            print("cookie file %s not exist\n" % (filepath,))
            return
        with open(filepath, "r") as fp:
            self.cookie_str = fp.read()[:-1]
        if self.cookie_str == "":
            print("cookie file %s is empty\n" % (filepath,))
            return

    def get_cookie_by_login(self, email="", password=""):
        if not email or not password:
            print("username or password not valid\n")
        data = {"form_email": email, "form_password": password}
        resp = requests.post(self.login_url, data)
        if resp.status_code != 200:
            print("post request to %s failed with status code %d\n" % (self.login_url, resp.status_code))
            return
        # should be redirected
        if len(resp.history) < 2:
            print("login failed\n")
            return
        self.cookies_dict = resp.history[0].cookies.get_dict()

    def _get_with_cookie(self, url):
        resp = None
        if len(self.cookie_str) > 0:
            resp = requests.get(url, headers={"Cookie": self.cookie_str})
        else:
            resp = requests.get(url, cookies = self.cookies_dict)
        if resp is None or resp.status_code != 200:
            resp = None
        return resp

    def backup(self):
        self.backup_book()
        self.backup_video()
        self.backup_music()
        self.backup_doulist()

    def backup_book(self):
        read_url = "https://book.douban.com/people/feilengcui008/collect"
        wish_url = "https://book.douban.com/people/feilengcui008/wish"
        reading_url = "https://book.douban.com/people/feilengcui008/do"
        book_dir = os.path.join(self.root, "book")
        if not os.path.exists(book_dir):
            os.mkdir(book_dir)
        self._backup_book(read_url, os.path.join(book_dir, "done.txt"))
        self._backup_book(wish_url, os.path.join(book_dir, "todo.txt"))
        self._backup_book(reading_url, os.path.join(book_dir, "doing.txt"))

    def _backup_book(self, url, filepath):
        buf = ""
        while url != None:
            resp = self._get_with_cookie(url)
            if resp is None:
                print("backup_book url %s failed\n" % (url,))
                return
            # find next page url
            bsp = bs.BeautifulSoup(resp.content)
            next_pages = bsp.findAll("link", attrs={"rel": "next"})
            if len(next_pages) > 0:
                url = next_pages[0].attrMap["href"]
            else:
                url = None
            # find all books in this page
            books = bsp.findAll("li", attrs={"class": "subject-item"})
            for book in books:
                info = book.find("div", attrs={"class": "info"})
                short_note = book.find("div", attrs={"class": "short-note"})
                note_div = short_note.findAll("div")[0]
                # title
                title = info.find("a").attrs[1][1]
                # pub
                pub = book.find("div", attrs={"class": "pub"}).contents[0].strip()
                pub = "/".join(pub.split("/")[:-1])
                # rating
                def get_rating_level(node):
                    for k in self.rating_level_map.iterkeys():
                        e = node.find("span", attrs={"class": k})
                        if e:
                            return k
                    return ""
                rating_level = get_rating_level(note_div)
                if rating_level:
                    rating_level = self.rating_level_map[rating_level]
                # date
                read_date = note_div.find("span", attrs={"class": "date"}).contents[0].strip()
                read_date = read_date.split("\n")[0]
                # comment
                comment = short_note.find("p", attrs={"class": "comment"}).contents[0].strip()
                item_str = "%s | %s" % (title, pub)
                if rating_level:
                    item_str = "%s | %s" % (item_str, rating_level)
                if comment:
                    item_str = "%s | %s" % (item_str, comment)
                if read_date:
                    item_str = "%s | %s" % (item_str, read_date)
                buf = "%s%s\n" % (buf, item_str)
            
        # write to file
        with open(filepath, "wb") as fp:
            fp.write(buf.encode("utf-8"))

    def backup_video(self):
        watched_url = "https://movie.douban.com/people/feilengcui008/collect"
        wish_url = "https://movie.douban.com/people/feilengcui008/wish"
        watching_url = "https://movie.douban.com/people/feilengcui008/do"
        video_dir = os.path.join(self.root, "video")
        if not os.path.exists(video_dir):
            os.mkdir(video_dir)
        self._backup_video(watched_url, os.path.join(video_dir, "done.txt"))
        self._backup_video(wish_url, os.path.join(video_dir, "todo.txt"))
        self._backup_video(watching_url, os.path.join(video_dir, "doing.txt"))

    def _backup_video(self, url, filepath):
        buf = ""
        while url != None:
            resp = self._get_with_cookie(url)
            if resp is None:
                print("backup_video url %s failed\n" % (url,))
                return
            # find next page url
            bsp = bs.BeautifulSoup(resp.content)
            next_pages = bsp.findAll("link", attrs={"rel": "next"})
            if len(next_pages) > 0:
                url = next_pages[0].attrMap["href"]
            else:
                url = None
            # find all books in this page
            videos = bsp.findAll("div", attrs={"class": "item"})
            for video in videos:
                info = video.find("div", attrs={"class": "info"})
                # title
                title = info.find("li", attrs={"class": "title"}).find("a").find("em").contents[0]
                # rating
                def get_rating_level(node):
                    for k in self.rating_level_map.iterkeys():
                        e = node.find("span", attrs={"class": k})
                        if e:
                            return k
                    return ""
                rating_level = get_rating_level(info)
                if rating_level:
                    rating_level = self.rating_level_map[rating_level]
                # date
                watch_date = info.find("span", attrs={"class": "date"}).contents[0].strip()
                watch_date = watch_date.split("\n")[0]
                # comment
                comment = info.find("span", attrs={"class": "comment"})
                if comment:
                    comment = comment.contents[0].strip()
                item_str = "%s" % (title,)
                if rating_level:
                    item_str = "%s | %s" % (item_str, rating_level)
                if comment:
                    item_str = "%s | %s" % (item_str, comment)
                if watch_date:
                    item_str = "%s | %s" % (item_str, watch_date)
                buf = "%s%s\n" % (buf, item_str)
            
        # write to file
        with open(filepath, "wb") as fp:
            fp.write(buf.encode("utf-8"))

    def backup_music(self):
        listened_url = "https://music.douban.com/people/feilengcui008/collect"
        wish_url = "https://music.douban.com/people/feilengcui008/wish"
        listening_url = "https://music.douban.com/people/feilengcui008/do"
        music_dir = os.path.join(self.root, "music")
        if not os.path.exists(music_dir):
            os.mkdir(music_dir)
        self._backup_music(listened_url, os.path.join(music_dir, "done.txt"))
        self._backup_music(wish_url, os.path.join(music_dir, "todo.txt"))
        self._backup_music(listening_url, os.path.join(music_dir, "doing.txt"))

    def _backup_music(self, url, filepath):
        buf = ""
        while url != None:
            resp = self._get_with_cookie(url)
            if resp is None:
                print("backup_music url %s failed\n" % (url,))
                return
            # find next page url
            bsp = bs.BeautifulSoup(resp.content)
            next_pages = bsp.findAll("link", attrs={"rel": "next"})
            if len(next_pages) > 0:
                url = next_pages[0].attrMap["href"]
            else:
                url = None
            # find all books in this page
            musics = bsp.findAll("div", attrs={"class": "item"})
            for music in musics:
                info = music.find("div", attrs={"class": "info"})
                # title
                title = info.find("li", attrs={"class": "title"})
                title = title.find("a").find("em").contents[0].strip()
                # rating
                def get_rating_level(node):
                    for k in self.rating_level_map.iterkeys():
                        e = node.find("span", attrs={"class": k})
                        if e:
                            return k
                    return ""
                rating_level = get_rating_level(info)
                if rating_level:
                    rating_level = self.rating_level_map[rating_level]
                # date
                listen_date = info.find("span", attrs={"class": "date"}).contents[0].strip()
                listen_date = listen_date.split("\n")[0]
                # comment
                comment = info.find("p", attrs={"class": "comment"})
                if comment:
                    comment = comment.contents[0].strip()
                item_str = title
                if rating_level:
                    item_str = "%s | %s" % (item_str, rating_level)
                if comment:
                    item_str = "%s | %s" % (item_str, comment)
                if listen_date:
                    item_str = "%s | %s" % (item_str, listen_date)
                buf = "%s%s\n" % (buf, item_str)
            
        # write to file
        with open(filepath, "wb") as fp:
            fp.write(buf.encode("utf-8"))

    def backup_doulist(self):
        doulist_dir = os.path.join(self.root, "doulist")
        if not os.path.exists(doulist_dir):
            os.mkdir(doulist_dir)
        root_url = "https://www.douban.com/people/feilengcui008/doulists/all"
        resp = self._get_with_cookie(root_url)
        if not resp:
            return
        bsp = bs.BeautifulSoup(resp.content)
        doulist = bsp.find("ul", attrs={"class": "doulist-list"})
        lists = doulist.findAll("li")
        for lst in lists:
            lsta = lst.find("a")
            title = lsta.contents[0].strip()
            lst_url = lsta.attrs[0][1]
            temp_path = os.path.join(doulist_dir, title)
            if not os.path.exists(temp_path):
                os.mkdir(temp_path)
            # handle this list
            buf = ""
            resp = self._get_with_cookie(lst_url)
            if not resp:
                print("get doulist %s failed\n" % (title,))
                continue
            bsp = bs.BeautifulSoup(resp.content)
            doulist_items = bsp.findAll("div", attrs={"class": "doulist-item"})
            for item in doulist_items:
                item_title = item.find("div", attrs={"class": "title"}).find("a")
                item_name = item_title.contents[0].strip()
                item_url = item_title.attrs[0][1]
                buf = buf + "%s | %s\n" % (item_name, item_url)
            file_path = os.path.join(temp_path, "items.txt")
            with open(file_path, "wb") as fp:
                fp.write(buf.encode("utf-8"))

    def backup_diary(self):
        # TODO
        pass


def main():
    douban = DoubanBackuper()
    douban.get_cookie_by_file()
    douban.backup()

if __name__ == '__main__':
    main()
