#!/usr/bin/env python
#-*- coding : utf-8 -*-

from onedrive import OneDrive 

if __name__ == "__main__":
    
    client_id = "000000004416DF2B"
    client_secret = "wbX7pTlwHhnZIty7hjUxssKd9rJcitUO"
    o = OneDrive(client_id, client_secret)
    o.auth()
    o.access_data["access_token"] = "EwAYA61DBAAUGCCXc8wU/zFu9QnLdZXy+YnElFkAAWE0A+6uWGbO5hKCfE7IGsy96AVO9hqp7/WaDpeknUd1f486inH9OmVOws+AmnAiJgWcnxTmMqRJ6AOhByoJ6qk9zhz+IWZ6uXWu9dodXhqKWvpNUjbsApw4ZKTS2M06BLiT4dGo8rN/S5qLWFdq+TZqq06yhBj99CXV349Fych0/HbWo7JFIxmmDm6BZEahVnFVgVi7MRz1cJFV7Odc+eajlCjr1MP50QE/52rtya/fZJooBLpsuV4ZZ6VCWYyXB8FxIerBwWBdK7bScoqDc9iLA6nb1/eNc974WuQk1eUdlVwdaHwUqBrX0vIE04E6PGf741SlmzhxlnezHjrS5IgDZgAACCk9IEHilS4g6AGHxzY6MM3lm7UkYmkMzo8Jtv5DRj4LfT6tTTARRm5NVeNEFhyjO6gs6+/zasqycIw9Ogymik4GuRBAFQWgiHYFwwvooSpLPugDG8BeKSFNwQ9adS1rxzxky0pXMXr8c50v731qWcuUDJz94FT+dbT4AUCtxLTeaQAkd8zNi4oDxqCt31udYiNQgUQ31365Hfhs19olFA2MNvripJ5bI6gmfxH4vJgTEtO6vgEan62zOv66pn3VyuLwW5n4EMQWErp3LZJnhTOsIiCBWGSIykt8p0Sv1RCuMN8qkJ7GQzZzGYxxLSWWvuIsL3X3EXG6VfjW33UxBYex/hd7MBiYc2DA2mpx76b/HmMTGjgB/xsoWuZAS8Zp/BAQ8uMpFxhXFstKJkIbsw5mMNQ2NhKDhD7ZWPggzy/WPmRipELXpmOVoZdeVMCEARbNIwQhiyFiqSviipMtkmDIk63zZaPNhuIVVnIe2QneoAajI80Xxzf2VQqgVvuL6ZtOFvzMY0eMWrHHae5HYh/ObUfB0PAbPgg2LIezTSpQyKOAur6i5sCLArCzizQsKc4OAo3E2cXHWDvIk+GXERAe/3djGszXRH8O+VtXhTwALQVbUJ0GfGnSu4dEp8EtXOZ9irtgG/18N2ms8VCnyYSIBhkC" 
    o.access_data["refresh_token"] = "MCczfITCjvzeKoarXp8E5zHxunA8AWMEC0oVp0MjDRl2fXHBjAQjKp*0ya230GcFqq9PHv0irlmhBeIhpoOYDREjpIdyjW*1JwzKTh39Hd9c4rjPjZ5k7LFL*gPQeTBTuA1V78jpWFzMuVJyBWqCeDZStGaoiceQL8CXyu!1MFfyHyMMKqTLaMNKojHU8r6cWka10ovg*Exp3coI1WCPVxFABKIGZUjPQhlTatI4*!FqwbWFNXvcqavwPsc73awoVfBHG*CScRd1I7C2wER1iCsisHA34Pm5QGiVkfuGBStdvxDcX8SFl7wBTFZBEEM9KXuNkUOhAqZKNS8gQ3oNepWuOzzgOkKh2d07lujg!knz8"
    print o.createDirItem("testDir", parent_path="")
    #print o.moveItem("/IX.png", "/ix.png")
    #print o.deleteItem(item_path="/test")

    #print o.getItemInfo("/Documents", expand=True)
    #print o.searchDriveItem("bakeup")
    #print o.copyItem('/bakeup', '/bakeup.copy')
    #print o.getDriveList()
    #print "==========="
    #print o.getItemChildren()
    #print "==========="
    #print o.getItemInfo(item_id="root", expand=True)
    #print "==========="
    #print o.getDriveList()
    #print "==========="
    #print o.getDriveInfo()
    #print "==========="
    #print o.getSharedItems()
    #print "==========="
    #print o.getRecentItems()
    #print "==========="
    #print o.getItemInfo()
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
    
    #info = o.searchDriveItem("ubuntu")[0]["name"]
    #print info 

