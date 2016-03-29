#!/usr/bin/env python
#-*- coding : utf-8 -*-

import requests
import json 
import urllib
import os

#################################################################
#                                                               #
# a class for Microsoft OneDrive service operations             #
#                                                               #
# todo:                                                         #
#      1.make the auth more automation                          #
#      2.cache the dentries                                     #
#      3.make a simple cmd-like tool for terminal operations    #
#      4.combine these little logics into more difficult logcs  #
#      5.more                                                   #
#                                                               #
# author: feilengcui008@gmail.com                               #
# license: whatver you like                                     #
#                                                               #
#################################################################

class OneDrive(object):
    client_id = ""
    client_secret = "" 
    code = ""
    access_data = {}
    auth_scopes = "wl.signin wl.offline_access onedrive.readwrite"
    api_root = "https://api.onedrive.com/v1.0"
    redirect_uri = ""
    write_file_path = "auth.auth"

    def __init__(self, client_id, client_secret):
        self.client_id = client_id 
        self.client_secret = client_secret 

    ########## auth related ##########
    # include all steps for auth 
    def auth(self):
        if self.readAuthFile():
            return 
        else:
            self.getCode()
            self.getBearer()

    # use usernme:pass to get the authentiation code 
    def getCode(self):
        url = "https://login.live.com/oauth20_authorize.srf"
        url = "%s?client_id=%s&scope=%s&response_type=%s&redirect_uri=%s" % (url, self.client_id, self.auth_scopes, "code", self.redirect_uri)
        # login into microsoft redirect to the url and get the code in the headers 
        #res = requests.get(url)
        self.code = "your code"
    
    # use authentication code to get the api access code 
    def getBearer(self):
        url = "https://login.live.com/oauth20_token.srf"
        data = {}
        data["client_id"] = self.client_id 
        data["client_secret"] = self.client_secret
        data["code"] = self.code 
        data["grant_type"] = "authorization_code"
        res = requests.post(url, data)
        self.access_data = json.loads(res.content) 
        self.writeAuthFile()

    # after the access_token expires, get the new access_token 
    def getRefreshToken(self):
        url = "https://login.live.com/oauth20_token.srf"
        data = {}
        data["client_id"] = self.client_id 
        data["client_secret"] = self.client_secret
        data["refresh_token"] = self.access_data["refresh_token"]
        data["grant_type"] = "refresh_token"
        res = requests.post(url, data)
        self.access_data = json.loads(res.content) 
        print self.access_data, "-----refreshed------"
        self.writeAuthFile()
        
    # write the code and access_token into local files 
    def writeAuthFile(self):
        s = "%s\n%s\n%s\n%s\n" % (self.code, self.access_data["access_token"], self.access_data["refresh_token"], self.auth_scopes)
        with open(self.write_file_path, "w") as fp:
            fp.write(s)

    # reload the local auth file 
    def readAuthFile(self):
        if not os.path.exists(self.write_file_path):
            return False 
        lines = None
        with open(self.write_file_path, "r") as fp:
            lines = fp.readlines()
        if lines:
            self.code = lines[0]
            self.access_data["access_token"] = lines[1]
            self.access_data["refresh_token"] = lines[2]
            return True
        else:
            return False 


    ########## drive level ops ##########
    def getDriveMetaInfo(self):
        url = "%s%s?access_token=%s" % (self.api_root, "/drive", self.access_data["access_token"])
        res = requests.get(url)
        return json.loads(res.content)
        
    def getDriveRootInfo(self):
        info = self.getItemInfo()
        return info 

    def searchDriveItem(self, item_name):
        url = "%s%s?q=%s&access_token=%s" % (self.api_root, "/drive/root/view.search", item_name, self.access_data["access_token"])
        res = requests.get(url)
        data = json.loads(res.content)
        if data.has_key("value"):
            return data["value"]
        else:
            return None

    def getDriveFolderInfo(self, folder = "/drive/root/children", expand = False):
        url = "%s%s?access_token=%s" % (self.api_root, folder, self.access_data["access_token"])
        if expand:
            url = url + "&expand=children"
        res = requests.get(url)
        data = json.loads(res.content)
        if data.has_key("value"):
            for i in data["value"]:
                self._printItemInfo(i)
        else:
            self.getRefreshToken()
            self.getDriveFolderInfo(folder)

    ########## item level curd ops ############
    def _construct_item_path(self, item_id, item_path = ""):
        by_id = None 
        by_path = None 
        if item_id:
            by_id = "/drive/items/%s" % str(item_id)
            url = "%s%s?access_token=%s" % (self.api_root, by_id, self.access_data["access_token"])
        elif item_path:
            by_path = "/drive/root:/%s" % str(item_path)
            url = "%s%s?access_token=%s" % (self.api_root, by_path, self.access_data["access_token"])
        else:
            print "no item_id or item_path"
            return None
        return url
    
    def _getIdByPath(self, parent_path = ""):
        if parent_path:
            parent_info = self.getItemInfo(item_path = parent_path)
            if not parent_info.has_key("id"):
                print "in getIdByPath get id for %s failed" % parent_path
                return None
            return parent_info["id"]
        else:
            parent_info = self.getItemInfo(item_path = "/drive/root")
            if not parent_info.has_key("id"):
                print "in getIdByPath get id for root failed"
                return None
            return parent_info["id"]

    def _printItemInfo(self, item_data = {}):
        for i in item_data:
            print i, item_data[i]
        if item_data:
            print "================="
            print "id:", item_data["id"]
            print "name:", item_data["name"]
            print "size:", item_data["size"]
            if item_data.has_key("folder"):
                print "folder:", item_data["folder"]
            print "================="

    def getItemInfo(self, item_id = "", item_path = ""):
        if not item_id and not item_path:
            return {} 
        url = self._construct_item_path(item_id, item_path)
        if url:
            res = requests.get(url)
            data = json.loads(res.content)
            #self._printItemInfo(data)
            if data.has_key("name"):
                return data 
            else:
                #self.getRefreshToken()
                #return self.getItemInfo(item_id, item_path)
                pass 
        return {}

    def getItemChildren(self, item_id = "", item_path = ""):
        temp_id = item_id 
        temp_path = item_path 
        if item_id:
            item_id = item_id + "/children"
        if item_path:
            item_path = item_path + ":/children"
        url = self._construct_item_path(item_id, item_path)
        if url:
            res = requests.get(url)
            data = json.loads(res.content)
            if data.has_key("value"):
                return data["value"]
            else:
                #self.getRefreshToken()
                #return self.getItemChildren(temp_id, temp_path)
                return None
        else:
            return None



    def createDirItem(self, name, parent_id = "", parent_path = "", is_dir = True):
        if not parent_id:
            parent_id = self._getIdByPath(parent_path)
            if not parent_id:
                return 
        by_id = "/drive/items/%s/children" % parent_id
        url = "%s%s?access_token=%s" % (self.api_root, by_id, self.access_data["access_token"])
        if is_dir:
            data = {}
            data["name"] = name 
            data["folder"] = { "childCount" : 0 }
            data["@name.conflictBehavior"] = "fail"
            headers = { 'Content-Type' : 'application/json' }
            res = requests.post(url, json.dumps(data), headers = headers)
            data = json.loads(res.content)
            #print "create results:", data
            if data.has_key("id"):
                print "success create item %s" % data["name"]
                return data 
            else:
                print "create item %s failed" % name 
                print "the message is:", data 
        else: # handle file upload, leave for put file item 
            pass 

    # upload a file item 
    def uploadFileItem(self, name, parent_id = "", parent_path = ""):
        if not name:
            return 
        files = {'file' : open(name, "rb")}
        if not parent_id:
            parent_id = self._getIdByPath(parent_path)
            if not parent_id:
                return 
        filename = name.split("/")[-1]
        by_id = "/drive/items/%s/children/%s/content" % (parent_id, filename)
        url = "%s%s?access_token=%s" % (self.api_root, by_id, self.access_data["access_token"])
        res = requests.put(url, files = files)
        data = json.loads(res.content)
        if data.has_key("id"):
            print "success upload file"
        else:
            print "failed upload file"
            print "the message is:", data

    # upload a directory
    def uploadDirItem(self, name, parent_id = "", parent_path = ""):
        if not os.path.isdir(name):
            print "%s is not dir" % name 
            return 
        if not parent_id:
            parent_id = self._getIdByPath(parent_path)
            if not parent_id:
                return 
        dir_name = name.split("/")[-1]
        create_data = self.createDirItem(dir_name, parent_id)
        item_list = os.listdir(name)
        for i in item_list:
            if os.path.isdir(i):
                self.uploadDirItem(name.rstrip("/") + "/" + i, parent_id = create_data["id"])
            else:
                self.uploadFileItem(name.rstrip("/") + "/" + i, parent_id = create_data["id"])

    # delete the item 
    def deleteItem(self, item_id = "", item_path = ""):
        if not item_id and not item_path:
            print "both item_id and item_path are missing"
            return 
        url = self._construct_item_path(item_id, item_path)
        res = requests.delete(url)
        if res.status_code == 204:
            print "success delete item"
        else:
            print "error happens when deleted item, the message is:", json.loads(res.content)

    # download the item 
    def downloadItem(self, item_info = {}, current_path = ""):
        item_id = item_info["id"]
        if item_id:
            item_id = item_id + "/content"
        else:
            return 
        url = self._construct_item_path(item_id)
        if url:
            res = requests.get(url)
            item_name = current_path + item_info["name"]
            # redirect to download url 
            redirect_url = res.headers["Content-Location"]
            res = requests.get(redirect_url)
            if not os.path.exists(item_name):
                with open(item_name, "wb") as fp:
                    fp.write(res.content)

    # download directory 
    def downloadDir(self, dir_item_info = {}, parent_path = ""):
        current_path = parent_path + dir_item_info["name"] + "/"
        print current_path, "======="
        if not dir_item_info.has_key("folder"):
            return 
        elif dir_item_info["folder"] == 0:
            if not os.path.exists(current_path):
                os.mkdir(current_path)
        else:
            # has children 
            if not os.path.exists(current_path):
                os.mkdir(current_path)
            children = self.getItemChildren(dir_item_info["id"])
            for i in children:
                if i.has_key("folder"):
                    self.downloadDir(i, current_path)
                else:
                    self.downloadItem(i, current_path)
