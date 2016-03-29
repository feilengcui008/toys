#!/usr/bin/env python
#-*- coding : utf-8 -*-

from onedrive import OneDrive 

if __name__ == "__main__":
    
    client_id = "your microsoft app client id"
    client_secret = "your microsoft app client secret"
    o = OneDrive(client_id, client_secret)
    o.auth()
    
    # some testings 
    #o.getDriveFolderInfo(folder="/drive/root/children")
    #o.downloadItem(info)
    #o.downloadDir(info)
    #o.createDirItem("from_ubuntu", parent_path = "study")
    #o.deleteItem(item_info["id"])
    #o.deleteItem(item_path = "study/from_ubuntu")
    #o.uploadFileItem("temp.txt", parent_path = "study/from_ubuntu")
    
    #info = o.getItemChildren(item_path = "study/from_ubuntu")
    #print info
    #o.deleteItem(item_path = "study/from_ubuntu/temp")
    #o.uploadDirItem("temp", parent_path = "study/from_ubuntu")
    
    info = o.searchDriveItem("ubuntu")[0]["name"]
    print info 

