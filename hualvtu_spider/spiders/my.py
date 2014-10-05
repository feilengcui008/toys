#!/usr/bin/env python
#coding:utf-8
""" 
a spider for hualvtu.com using scrapy framework and BeautifulSoup
优质图片网站画旅途的图片爬虫，按城市和相册集存储，可适当修改爬去任意城市
coding by feilengcui008@gmail.com
github:https://github.com/feilengcui

"""
import os
import scrapy
from scrapy.http import Request
import BeautifulSoup as bs
from hualvtu_spider.items import CityItem,AlbumItem,PictureItem
import re

#storage place
OUTPUT = os.path.join(os.path.dirname(os.path.realpath(__file__)),"OUT/")
CITY_PAGE_DEPTH = 5
MYCOUNTRY = u"意大利"

picture_album_re = re.compile(r'hualvtu.com/(\d+)$')
picture_re = re.compile('img3A.hualvtu.com')
dest_re = re.compile(r'hualvtu.com/dest')
dest_more = re.compile(r'hualvtu.com/loadMoreQuarks3.action')

class MySpider(scrapy.Spider):
    name = "my"
    allowed_domains = ["hualvtu.com"]
    start_urls = (
                "http://www.hualvtu.com/explore",
            )

    def parse(self,response):
        
        #---------------------------------------------------------
        #parse the start_urls
        if re.match(r'http://www.hualvtu.com/explore',response.url) is None:
            pass
        else:
            soup = bs.BeautifulSoup(response.body)
            labels = soup.findChildren('div',{'class':'labels'})
            #labels = labels[0:1]
            for i in labels:
                dests = i.findChildren('a')
                dests = [dest for dest in dests if dest.get('title')==MYCOUNTRY ]
                if dests:
                    for j in dests:
                        new_city_item = CityItem()
                        new_city_item['city_name'] = j.get('title')
                        new_city_item['city_id'] = j.get('areaid')
                        new_city_item['city_link'] = "http://www.hualvtu.com/dest/"+j.get('areaid')
                        req = Request(new_city_item['city_link'],callback=self.parse)
                        req.city = new_city_item
                        yield req
        #---------------------------------------------------------

        #================================
        #parse the city url
        if dest_re.search(response.url) is not None:
            #handle current_page_album
            soup = bs.BeautifulSoup(response.body)
            current_page_albums = soup.findChildren('div',{'class':'quark-inner '})
            if current_page_albums is not None:
                for i in current_page_albums:
                    links = i.findChild('a').get('href')
                    title = i.findChild('span',{'class':'title-text'}).getString()
                    if links is not None:
                        new_album_item = AlbumItem()
                        new_album_item['album_name'] = title
                        new_album_item['album_link'] = links
                        req = Request(new_album_item['album_link'],callback=self.parse)
                        req.album_item = new_album_item
                        req.city = response.request.city
                        yield req

            #handle more_page_album request back
            div = soup.findChild('div',{'more':'true'})
            if div is not None:
                album_id = div.get('last')
                if album_id is not None:
                    link = "http://www.hualvtu.com/loadMoreQuarks3.action?did="+response.request.city["city_id"]+"&last="+album_id
                    req = Request(link,callback=self.parse)
                    req.city = response.request.city
                    req.num = 1
                    yield req
        #================================

        #++++++++++++++++++++++++++++++++
        # parse more albums request
        elif dest_more.search(response.url) is not None:
            soup = bs.BeautifulSoup(response.body)
            #handle current_page_album
            current_page_albums = soup.findChildren('div',{'class':'quark-inner '})
            if current_page_albums is not None:
                for i in current_page_albums:
                    links = i.findChild('a').get('href')
                    title = i.findChild('span',{'class':'title-text'}).getString()
                    if links is not None:
                        new_album_item = AlbumItem()
                        new_album_item['album_name'] = title
                        new_album_item['album_link'] = links
                        req = Request(new_album_item['album_link'],callback=self.parse)
                        req.album_item = new_album_item
                        req.city = response.request.city
                        yield req
            #handle more_page_album request back
            if response.request.num > CITY_PAGE_DEPTH:
                pass
            else:
                script = soup.findChild('script')
                if script is not None:
                    r = re.search(r'(\d+)',script.getString())
                    album_id = r.group()
                    link = "http://www.hualvtu.com/loadMoreQuarks3.action?did="+response.request.city["city_id"]+"&last="+album_id
                    req = Request(link,callback=self.parse)
                    req.city = response.request.city
                    req.num = response.request.num+1
                    yield req

        #!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        #handle album request
        elif picture_album_re.search(response.url) is not None:
            soup = bs.BeautifulSoup(response.body)
            divs = soup.findChildren('div',{'backcolor':'#333'})
            print "----------"+response.request.city["city_name"]+"-----"+response.request.album_item["album_name"]+"--------"
            if divs:
                for i in divs:
                    picture_link = i.get('photo')
                    print "----"+picture_link+"----"
                    if picture_link:
                        req = Request(picture_link,callback=self.parse)
                        req.city = response.request.city
                        req.album = response.request.album_item
                        yield req
        #!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

        #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        #handle picture storage
        elif picture_re.search(response.url) is not None:
            filedir = OUTPUT+response.request.city["city_name"]+"/"+response.request.album["album_name"].replace(' ','_')+"/"
            if not os.path.exists(filedir):
                try:
                    os.makedirs(filedir)
                except:
                    return
            filename = filedir+response.url[5:].replace("/","_")
            if os.path.exists(filename):
                pass
            else:
                with open(filename,"w+") as fp:
                    fp.write(response.body)
        #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

        else:
            pass
