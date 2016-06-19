#!/usr/bin/python 
# encoding: utf-8

import requests 
import sys
import json

################################
#                              # 
# a script to test restful api #
#                              #
# feilengcui008@gmail.com      #
#                              #
################################

USERNAME = "your username"
PASSWORD = "your password"

class TestScript(object):
    url = None
    data = None
    method = None
    headers = None 
    res = None 
    token = None 
    cookies = None 
    def __init__(self, url, headers = None, data = None, cookies = None, \
            method = 'get'):
        self.url = url
        self.headers = headers 
        self.data = data 
        self.method = method 
        # self.cookies = cookies 

    def login(self):
        username = USERNAME 
        password = PASSWORD 
        payload = {}
        payload["account"] = username 
        payload["password"] = password 
        url = "http://x.x.x.x/login"
        res = requests.post(url, data = payload)
        if res.text:
            # self.cookies = res.cookies 
            self.token = json.loads(res.text)["msg_data"]["token"]
        else:
            raise Exception("login failed")

    def sendRequest(self):
        self.data["token"] = self.token
        if self.method == 'get':
            self.res = requests.get(url, params = self.data, headers \
                    = self.headers, cookies = self.cookies) 
            #print self.res.content 
            outfile = sys.stdout
            with outfile:
        		json.dump(json.loads(self.res.content), outfile, \
                                sort_keys=True,
                  	indent=4, separators=(',', ': '))
        		outfile.write('\n')
            return 
        if self.method == 'post':
            self.res = requests.post(url, data= self.data, headers = \
                    self.headers, cookies = self.cookies)   
            #print self.res.contentoutfile = sys.stdout
            outfile = sys.stdout
            with outfile:
        		json.dump(json.loads(self.res.content), outfile, \
                                sort_keys=True,
                  	indent=4, separators=(',', ': '))
        		outfile.write('\n')
            return 
        

if __name__ == '__main__':
    if len(sys.argv) < 2:
        print "python test.py request_data_json_file_path"
        sys.exit()
    filename = sys.argv[1]
    data = None 
    with open(filename, 'r') as fp:
        data = json.loads(fp.read())
    url = data["url"]
    method = data["method"]
    data = data["data"]
    headers = {}
    ts = TestScript(url, headers, data, method = method)
    ts.login()
    #sys.exit()
    ts.sendRequest()
