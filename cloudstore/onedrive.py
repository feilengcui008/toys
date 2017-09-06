#!/usr/bin/env python
#-*- coding : utf-8 -*-

import json 
import urllib
import urllib2
import os
import BaseHTTPServer
import threading
import thread
import webbrowser
import base64

from http import Urllib2HttpProvider
from http import RequestsHttpProvider
from http import HttpProviderException


class OneDriveException(Exception):
    """ wrap onedrive related exception """
    pass


class OneDrive(object):

    def __init__(self, client_id, client_secret):
        """ initialize onedrive with client id and secrect """
        self.client_id = client_id 
        self.client_secret = client_secret 
        self.code_server_uri = "https://login.live.com/oauth20_authorize.srf"
        self.access_token_uri = "https://login.live.com/oauth20_token.srf"
        self.api_root_uri = "https://api.onedrive.com/v1.0"
        self.scope = " ".join([
            "offline_access",
            "onedrive.readwrite"
        ])
        self.auth_db = "auth.db"
        self.code = self.access_token = self.refresh_token = None
        try:
            self.http_provider = RequestsHttpProvider()
        except:
            self.http_provider = Urllib2HttpProvider()
        self.initCodeServer("localhost", 8884)

    def initCodeServer(self, addr, port):
        """ init code server """
        self.redirect_uri = "http://%s:%s" % (str(addr), str(port))
        self.code_server = SimpleCodeServer(addr, port, SimpleCodeHandler)

    def auth(self):
        """ auth """
        if self._readAuthCacheFile():
            return
        self.getCode(self.scope)
        self.getAccessToken()

    def _writeAuthCacheFile(self):
        """ write into cache file """
        data = {'access_token': self.access_token, 'refresh_token': self.refresh_token}
        with open(self.auth_db, "w") as fp:
            fp.write(base64.encodestring(json.dumps(data)))

    def _readAuthCacheFile(self):
        """ check if cached """
        if not os.path.exists(self.auth_db):
            return False
        with open(self.auth_db, "r") as fp:
            lines = fp.read()
            if not lines or len(lines) == 0:
                return False
            data = json.loads(base64.decodestring(lines))
            try:
                self.access_token = data["access_token"]
                self.refresh_token = data["refresh_token"]
            except Exception as e:
                return False
        return True

    def getCode(self, scope, response_type="code"):
        """ get code """
        parameters = {
            "client_id": self.client_id,
            "scope": scope,
            "response_type": response_type,
            "redirect_uri": self.redirect_uri
        }
        target = "%s?%s" % (self.code_server_uri, urllib.urlencode(parameters))
        server_thread = threading.Thread(target=self.code_server.serve_forever)
        browser_thread = threading.Thread(target=webbrowser.open, args=(target,))
        server_thread.start()
        browser_thread.setDaemon(True)
        browser_thread.start()
        self.code_server.event.wait()
        self.code_server.shutdown()
        server_thread.join()
    
    def getAccessToken(self, grant_type="authorization_code"):
        """ get access token """
        if not self.code_server.token:
            raise OneDriveException("get access token failed, code none")
        parameters = {
            "client_id": self.client_id,
            "client_secret": self.client_secret,
            "code": self.code_server.token,
            "grant_type": grant_type,
            "redirect_uri": self.redirect_uri
        }
        (_, _, data, _) = self.http_provider.post(self.access_token_uri, data=parameters)
        if data is None:
            raise OneDriveException("get access token request failed")
        data = json.loads(data)
        self.access_token = data["access_token"]
        self.refresh_token = data["refresh_token"]
        self._writeAuthCacheFile()

    def refreshToken(self):
        """ refresh access_token and refresh_token """
        if not self.refresh_token:
            raise OneDriveException("can not refresh token without refresh_token")
        parameters = {
            "client_id": self.client_id,
            "client_secret": self.client_secret,
            "grant_type": "refresh_token",
            "redirect_uri": self.redirect_uri,
            'refresh_token': self.refresh_token
        }
        (_, _, data, _) = self.http_provider.post(self.access_token_uri, data=parameters)
        if data is None:
            raise OneDriveException("refresh token failed")
        data = json.loads(data)
        self.access_token = data["access_token"]
        self.refresh_token = data["refresh_token"]
        self._writeAuthCacheFile()

    def _requestWithAuthHeader(self, url, method="GET", headers=dict(), data=None, files=None, count=0):
        """ get with auth header """
        if count > 3:
            raise OneDriveException("refresh token failed max times 3")
        headers.update({"Authorization": "bearer %s" % (self.access_token,)})
        if method == "GET":
            data = self.http_provider.get(url, headers=headers, data=data)
        if method == "HEAD":
            data = self.http_provider.head(url, headers=headers)
        if method == "POST":
            data = self.http_provider.post(url, headers=headers, data=data, files=files)
        if method == "PUT":
            data = self.http_provider.put(url, headers=headers, data=data, files=files)
        if method == "PATCH":
            data = self.http_provider.patch(url, headers=headers, data=data)
        if method == "DELETE":
            data = self.http_provider.delete(url, headers=headers)
        if data[1] == 'Unauthorized':
            self.refreshToken()
            headers['Authorization'] = "bearer %s" % (self.access_token,)
            return self._requestWithAuthHeader(url, method, headers, data, files, count + 1)
        temp_data = data[2]
        try:
            temp_data = json.loads(data[2])
        except:
            pass
        return (data[0], data[1], temp_data, data[3])

    def getDriveList(self):
        """ get drive list """
        url = "%s/drives" % (self.api_root_uri,)
        (code, msg, data, _) = self._requestWithAuthHeader(url, method="GET")
        if code != 200:
            raise OneDriveException("get drive list failed with msg: %s" % (msg,))
        return data

    def getDriveInfo(self, drive_id=""):
        """ get info for a specific drive """
        url = "%s/drive/%s" % (self.api_root_uri, drive_id)
        (code, msg, data, _) = self._requestWithAuthHeader(url, method="GET")
        if code != 200:
            raise OneDriveException("get drive info for %s failed with msg: %s" % (drive_id, msg))
        return data

    def getSharedItems(self):
        """ get item list shared by user """
        url = "%s/drive/shared" % (self.api_root_uri,)
        (code, msg, data, _) = self._requestWithAuthHeader(url, method="GET")
        if code != 200:
            raise OneDriveException("get shared items failed with msg: %s" % (msg,))
        return data

    def getRecentItems(self):
        """ get recent items """
        url = "%s/drive/view.recent" % (self.api_root_uri,)
        (code, msg, data, _) = self._requestWithAuthHeader(url, method="GET")
        if code != 200:
            raise OneDriveException("get recent items failed with msg: %s" % (msg,))
        return data

    def searchDriveItem(self, item_name):
        """ search drive item """
        url = "%s/drive/root/view.search?q=%s" % (self.api_root_uri, item_name)
        (code, msg, data, _) = self._requestWithAuthHeader(url, method="GET")
        if code != 200:
            raise OneDriveException("search for %s failed with msg: %s" % (item_name, msg))
        return data

    def _getItemInfoByFullPath(self, item_path, expand=False):
        """ get item info """
        url = "%s%s" % (self.api_root_uri, item_path)
        code = msg = data = None
        if expand:
            (code, msg, data, _) = self._requestWithAuthHeader(url, method="GET", data={'expand': 'children'})
        (code, msg, data, _) = self._requestWithAuthHeader(url, method="GET")
        if code != 200:
            raise OneDriveException("get item info for %s failed with msg: %s" % (item_path, msg))
        return data

    def getItemInfoById(self, item_id, expand=False):
        """ get item info by id """
        url = "%s/drive/items/%s" % (self.api_root_uri, item_id)
        code = msg = data = None
        if expand:
            (code, msg, data, _) = self._requestWithAuthHeader(url, method="GET", data={'expand': 'children'})
            return data
        (code, msg, data, _) = self._requestWithAuthHeader(url, method="GET")
        if code != 200:
            raise OneDriveException("get item info for %s failed with msg: %s" % (item_id, msg))
        return data
    
    def getItemInfoByAppPath(self, item_path, expand=False):
        """ get item info for special folders """
        full_path = "/drive/special/approot:%s" % (item_path,)
        return self._getItemInfoByFullPath(full_path)

    def getItemInfoByCommonPath(self, item_path, expand=False):
        """ get item info """
        full_path = "/drive/root:%s" % (item_path,)
        return self._getItemInfoByFullPath(full_path)

    def _getItemChildrenByFullPath(self, item_path):
        """ get children of a item """
        url = "%s%s" % (self.api_root_uri, item_path)
        (code, msg, data, _) = self._requestWithAuthHeader(url, method="GET")
        if code != 200:
            raise OneDriveException("get item children for %s failed with msg: %s" % (item_path, msg))
        return data

    def getItemChildrenById(self, item_id):
        """ get children of a item """
        url = "%s/drive/items/%s/children" % (self.api_root_uri, item_id)
        (code, msg, data, _) = self._requestWithAuthHeader(url, method="GET")
        if code != 200:
            raise OneDriveException("get item children for %s failed with msg: %s" % (item_id, msg))
        return data

    def getItemChildrenByCommonPath(self, item_path):
        """ get children of a item """
        full_path = "/drive/root:%s:/children" % (item_path,)
        return self._getItemChildrenByFullPath(full_path)

    def getItemChildrenByAppPath(self, item_path):
        """ get children of a item """
        full_path = "/drive/special/approot:%s:/children" % (item_path,)
        return self._getItemChildrenByFullPath(full_path)

    def copyItemById(self, item_id, parent_id, new_name=None):
        """ copy item by id """
        data = {
            "parentReference": {"id": parent_id},
        }
        if new_name:
            data["name"] = str(new_name)
        url = "%s/drive/items/%s/action.copy" % (self.api_root_uri, item_id)
        headers = {'Content-Type': 'application/json'}
        headers.update({'Authorization': 'bearer ' + self.access_token, 'Prefer': 'respond-async'})
        # here we just assume the copy succ if code is 202, actually we need to poll the result
        # this can be implemented later
        (code, msg, data, _) = self._requestWithAuthHeader(url, method="POST", headers=headers, data=json.dumps(data))
        if code != 202:
            raise OneDriveException("copy item failed with msg: %s" % (msg,))
        return data

    def _getOrException(self, d, k):
        """ get or exception """
        try:
            return d.get(k)
        except:
            raise OneDriveException("obj do not has key %s" % (str(k),))

    def copyItem(self, item_path, parent_path, new_name=None, item_app=False, parent_app=False):
        """ copy a item """
        item_id = parent_id = None
        if item_app:
            item_id = self._getOrException(self.getItemInfoByAppPath(item_path), "id")
        else:
            item_id = self._getOrException(self.getItemInfoByCommonPath(item_path), "id")
        if parent_app:
            parent_id = self._getOrException(self.getItemInfoByAppPath(parent_path), "id")
        else:
            parent_id = self._getOrException(self.getItemInfoByCommonPath(parent_path), "id")
        return self.copyItemById(item_id, parent_id, new_name)

    def moveItem(self, item_path, parent_path, new_name=None, item_app=False, parent_app=False):
        """ move item """
        item_id = parent_id = None
        if item_app:
            item_id = self._getOrException(self.getItemInfoByAppPath(item_path), "id")
        else:
            item_id = self._getOrException(self.getItemInfoByCommonPath(item_path), "id")
        if parent_app:
            parent_id = self._getOrException(self.getItemInfoByAppPath(parent_path), "id")
        else:
            parent_id = self._getOrException(self.getItemInfoByCommonPath(parent_path), "id")
        data = {
            "parentReference": {"id": parent_id},
        }
        if new_name:
            data["name"] = str(new_name)
        return self.updateItemById(item_id, data)

    def updateItemById(self, item_id, metadata):
        """ update a item """
        url = "%s/drive/items/%s" % (self.api_root_uri, item_id)
        headers = {'Content-Type': 'application/json'}
        headers['Authorization'] = 'bearer ' + self.access_token
        (code, msg, data, _) = self._requestWithAuthHeader(url, method="PATCH", data=json.dumps(metadata), headers=headers)
        if code != 200:
            raise OneDriveException("update item %s failed with code: %s, msg: %s" % (item_id, code, msg))
        return data

    def updateCommonItem(self, item_path, metadata):
        """ update a item """
        item_id = self._getOrException(self.getItemInfoByCommonPath(item_path), "id")
        return self.updateItemById(item_id, metadata)

    def updateAppItem(self, item_path, metadata):
        """ update a app item """
        item_id = self._getOrException(self.getItemInfoByAppPath(item_path), "id")
        return self.updateItemById(item_id, metadata)
    
    def deleteItemById(self, item_id):
        """ delete item """
        url = "%s/drive/items/%s" % (self.api_root_uri, item_id)
        (code, msg, data, _) = self._requestWithAuthHeader(url, method="DELETE")
        if code != 204:
            raise OneDriveException("delete item %s failed with code: %s, msg: %s" % (str(code), msg))
        return data

    def deleteCommonItem(self, item_path):
        """ delete item """
        item_id = self._getOrException(self.getItemInfoByCommonPath(item_path), "id")
        return self.deleteItemById(item_id)

    def deleteAppItem(self, item_path):
        """ delete item """
        item_id = self._getOrException(self.getItemInfoByAppPath(item_path), "id")
        return self.deleteItemById(item_id)

    def createDirById(self, name, parent_id=""):
        """ create dir """
        url = "%s/drive/items/%s/children" % (self.api_root_uri, parent_id)
        headers = {'Content-Type': 'application/json'}
        data = {
            "name": name,
            "folder": {},
            "@name.conflictBehavior": "fail",
        }
        (code, msg, data, _) = self._requestWithAuthHeader(url, method="POST", data=json.dumps(data), headers=headers)
        if code != 201:
            raise OneDriveException("create dir %s failed with msg: %s" % (name, msg))
        return data

    def createCommonDir(self, name, parent_path):
        """ create common dir """
        parent_id = self._getOrException(self.getItemInfoByCommonPath(parent_path), "id")
        return self.createDirById(name, parent_id)

    def createAppDir(self, name, parent_path):
        """ create special dir like approot """
        parent_id = self._getOrException(self.getItemInfoByAppPath(parent_path), "id")
        return self.createDirById(name, parent_id)

    def uploadFileById(self, local_full_path, parent_id=None, name=None):
        """ upload a local item"""
        if not parent_id:
            raise OneDriveException("upload error, parent_id not valid")
        if not os.path.exists(local_full_path) or not os.path.isfile(local_full_path):
            raise OneDriveException("upload error, local file not exist or not file")
        if not name:
            name = os.path.split(local_full_path)[1]
            if not name:
                raise OneDriveException("you must specify name when push local root dir")
        url = "%s/drive/items/%s/children/%s/content" % (self.api_root_uri, parent_id, name)
        code = msg = data = None
        with open(local_full_path, "rb") as fp:
            (code, msg, data, _) = self._requestWithAuthHeader(url, method="PUT", data=fp.read())
        if code != 201 and code != 200:
            raise OneDriveException("upload %s failed with code: %s, msg: %s" 
                % (local_full_path, str(code), msg))

    def uploadCommonFile(self, local_full_path, parent_path, name=None):
        """ upload common file """
        item_info = self.getItemInfoByCommonPath(parent_path)
        return self.uploadFileById(local_full_path, item_info["id"], name)

    def uploadAppFile(self, local_full_path, parent_path, name=None):
        """ upload common file """
        item_info = self.getItemInfoByAppPath(parent_path)
        return self.uploadFileById(local_full_path, item_info["id"], name)

    def uploadDirById(self, local_full_path, parent_id, name=None):
        """ uplaod local dir"""
        if not os.path.exists(local_full_path) or not os.path.isdir(local_full_path):
            raise OneDriveException("upload dir error, local path not valid")
        if not name:
            name = os.path.split(local_full_path)[1]
            if not name:
                raise OneDriveException("you must specify remote dir name when upload local root")
        data = self.createDirById(name, parent_id)
        local_item_list = os.listdir(local_full_path)
        for item in local_item_list:
            full_item_path = os.path.join(local_full_path, item)
            if os.path.isdir(full_item_path):
                self.uploadDirById(full_item_path, data["id"])
            else:
                self.uploadFileById(full_item_path, data["id"])
    
    def uploadCommonDir(self, local_full_path, parent_path, name=None):
        """ upload common dir """
        item_info = self.getItemInfoByCommonPath(parent_path)
        return self.uploadDirById(local_full_path, item_info["id"])

    def uploadAppDir(self, local_full_path, parent_path, name=None):
        """ upload app dir """
        item_info = self.getItemInfoByAppPath(parent_path)
        return self.uploadDirById(local_full_path, item_info["id"])

    def downloadFileById(self, item_id, local_dir, item_info=None):
        """ download a item by id """
        url = "%s/drive/items/%s/content" % (self.api_root_uri, item_id)
        if not item_info:
            item_info = self.getItemInfoById(item_id)
        if not item_info:
            raise OneDriveException("get down item info for %s failed" % (item_id,))
        local_full_path = os.path.join(local_dir, item_info["name"])
        (code, msg, data, headers) = self._requestWithAuthHeader(url)
        if code != 200 and "content-location" not in headers:
            raise OneDriveException("download file failed with code: %s, msg: %s" % (str(code), msg))
        content_url = headers["content-location"]
        (code, msg, data, headers) = self._requestWithAuthHeader(content_url)
        if code != 200 or data is None:
            raise OneDriveException("download file failed with code: %s, msg: %s" % (str(code), msg))
        with open(local_full_path, "wb") as fp:
            fp.write(data)
        return None
        

    def downloadCommonFile(self, item_path, local_dir):
        """ download a common item """
        item_info = self.getItemInfoByCommonPath(item_path)
        if not item_info:
            raise OneDriveException("failed to download common file %s" % (item_path,))
        return self.downloadFileById(item_info["id"], local_dir, item_info)

    def downloadAppFile(self, item_path, local_dir):
        """ download a common item """
        item_info = self.getItemInfoByAppPath(item_path)
        if not item_info:
            raise OneDriveException("failed to download app file %s" % (item_path,))
        return self.downloadFileById(item_info["id"], local_dir, item_info)
        
    def downloadDirById(self, item_id, local_dir, item_info=None):
        """ download dir recursively """
        if not item_info:
            item_info = self.getItemInfoById(item_id)
        if not item_info:
            raise OneDriveException("get item info for download dir %s failed" % (item_id,))
        current_path = os.path.join(local_dir, item_info["name"])
        if "folder" not in item_info.keys():
            return
        elif item_info["folder"] == 0:
            if not os.path.exists(current_path):
                os.mkdir(current_path)
        else:
            if not os.path.exists(current_path):
                os.mkdir(current_path)
            children = self.getItemChildrenById(item_info["id"])
            for child in children["value"]:
                if "folder" in child.keys():
                    self.downloadDirById(child["id"], current_path)
                else:
                    self.downloadFileById(child["id"], current_path)
        return None

    def downloadCommonDir(self, item_path, local_dir):
        """ download common dir """
        item_info = self.getItemInfoByCommonPath(item_path)
        return self.downloadDirById(item_info["id"], local_dir, item_info)

    def downloadAppDir(self, item_path, local_dir):
        """ download app dir """
        item_info = self.getItemInfoByAppPath(item_path)
        return self.downloadDirById(item_info["id"], local_dir, item_info)


class SimpleCodeServer(BaseHTTPServer.HTTPServer, object):
    """ server to get code """
    def __init__(self, addr, port, handler):
        self.token = None
        self.event = threading.Event()
        super(SimpleCodeServer, self).__init__((addr, port), handler)


class SimpleCodeHandler(BaseHTTPServer.BaseHTTPRequestHandler):
    """ handler to get token """
    def do_GET(self):
        """ parse code from get parameter """
        if "code=" in self.path:
            self.server.token = self.path.split("code=")[1]
        self.send_response(200)
        self.send_header('Content-type', 'text/plain')
        self.end_headers()
        self.server.event.set()
        self.wfile.write(self.server.token)
