#!/usr/bin/env python
# coding: utf-8

from onedrive import OneDrive 
import sys
import time
import os
import shutil


def printResult(msg, content):
    print "============================ %s ======================" % (msg,)
    print content

def main():
    client_id = "your-app-id"
    client_secret = "your-app-code"
    o = OneDrive(client_id, client_secret)
    o.auth()

    res = o.getDriveList()
    printResult("driver list", res)

    res = o.getDriveInfo()
    printResult("driver info", res)

    res = o.getRecentItems()
    printResult("recent items", res)

    res = o.getSharedItems()
    printResult("shared items", res)
    
    res = o.getItemInfoByAppPath("/")
    printResult("app root path info", res)

    res = o.getItemInfoByCommonPath("/")
    printResult("common root path info", res)

    res = o.getItemChildrenByAppPath("/")
    printResult("children of app root path", res)
    
    res = o.getItemChildrenByCommonPath("/")
    printResult("children of common root path", res)

    res = o.createCommonDir("TEST_DIR", "/")
    printResult("create dir in common root dir", res)

    res = o.deleteCommonItem("/TEST_DIR")
    printResult("delete dir in common root dir", res)

    res = o.createAppDir("TEST_APP_DIR", "/")
    printResult("create dir in app root dir", res)

    res = o.deleteAppItem("/TEST_APP_DIR")
    printResult("delete dir in app root dir", res)

    res = o.createCommonDir("TEST_DIR", "/")
    res = o.copyItem("/TEST_DIR", "/", "TEST_DIR_COPY")
    res = o.deleteCommonItem("/TEST_DIR")
    time.sleep(2)
    res = o.deleteCommonItem("/TEST_DIR_COPY")
    printResult("copy dir in common root", res)

    res = o.createAppDir("TEST_DIR", "/")
    res = o.copyItem("/TEST_DIR", "/", "TEST_DIR_COPY", True, True)
    time.sleep(2)
    res = o.deleteAppItem("/TEST_DIR")
    res = o.deleteAppItem("/TEST_DIR_COPY")
    printResult("copy dir in app root", res)

    res = o.createCommonDir("TEST_DIR", "/")
    res = o.moveItem("/TEST_DIR", "/", "TEST_DIR_COPY")
    res = o.deleteCommonItem("/TEST_DIR_COPY")
    printResult("move dir in common root", res)

    res = o.createAppDir("TEST_DIR", "/")
    res = o.moveItem("/TEST_DIR", "/", "TEST_DIR_COPY", True, True)
    res = o.deleteAppItem("/TEST_DIR_COPY")
    printResult("move dir in app root", res)

    file_path = os.path.abspath(__file__)
    file_name = os.path.split(file_path)[1]
    res = o.uploadCommonFile(file_path, "/")
    printResult("upload local file to common root", res)

    os.mkdir("_TEMP")
    res = o.downloadCommonFile("/" + file_name, "./_TEMP")
    o.deleteCommonItem("/" + file_name)
    printResult("download common file from common root", res)
    
    res = o.uploadCommonDir("_TEMP", "/")
    shutil.rmtree("_TEMP")
    printResult("upload dir to common root", res)

    res = o.downloadCommonDir("/_TEMP", ".")
    shutil.rmtree("_TEMP")
    o.deleteCommonItem("/_TEMP")
    printResult("download dir from common root", res)
