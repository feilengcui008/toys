#!/usr/bin/env python
# -*- coding:utf-8 -*- 

import tornado.web
from tornado.httpclient import AsyncHTTPClient, HTTPRequest, HTTPResponse, HTTPClient
import tornado.gen
import json
import urllib 
import os 
import sys 
import time 
import redis 


class BaseHandler(tornado.web.RequestHandler):
    pass         

# academic handler  
class AcademicHandler(BaseHandler):
    API_URL = 'https://api.projectoxford.ai/academic/v1.0/evaluate?%s'
    TOKEN = '742398f16ecc46f7b0f6c650a287144e'
    begin_time = None 
    end_time = None 
    redis = redis.Redis()

    @tornado.web.asynchronous
    def get(self):
        self.begin_time = time.time()
        #self.test2()    
        #self.testEntities()
        #id1 = self.get_parameter('id1')
        #id2 = self.get_parameter('id2')
        id1 = "2157025439"
        id2 = "1982462162"
        id1 = "2157025439"
        id2 = "2061503185"
        #id1 = "1982462162"
        #id2 = "676500258"
        initialDataList1 = DataList()
        initialDataList2 = DataList()
        id1Type = id2Type = 0
        #data1 = json.loads(self.getEntitiesById(id1))
        data1 = self.getEntitiesById(id1)
        print data1.next()
        self.write("AAA")
        self.finish()
        return 
        #data2 = json.loads(self.getEntitiesById(id2))
        data2 = json.loads(self.getEntitiesById(id2))
        print len(data1["entities"])
        print len(data2["entities"])
        if not data1.has_key("entities"):
            print "id1 is auid"
            id1Type = 1
            data1 = json.loads(self.getEntitiesByAuid(id1))
        if not data2.has_key("entities"):
            print "id2 is auid"
            id2Type = 1
            data2 = json.loads(self.getEntitiesByAuid(id2))
        if not data1.has_key("entities") or not data2.has_key("entities"):
            print "parameter invalid"
            self.write(json.dumps([]))
            self.finish()

        # 先构造id1和id2的1 hop的path node
        for i in data1["entities"]:
            if i.has_key("RId"):
                for j in i["RId"]:
                    if j not in initialDataList1.RidList:
                        initialDataList1.addIdNode(j)
            if i.has_key("AA"):
                for j in i["AA"]:
                    if j.has_key("AuId") and j not in initialDataList1.AuidList:
                        initialDataList1.addAuidNode(j["AuId"])
                    if j.has_key("AfId") and j not in initialDataList1.AfidList:
                        initialDataList1.addAfidNode(j["AfId"])
            if i.has_key("F"):
                for j in i["F"]:
                    if j.has_key("FId") and j not in initialDataList1.FidList:
                        initialDataList1.addFidNode(j["FId"])
            if i.has_key("C"):
                for j in i["C"]:
                    if j.has_key("CId") and j not in initialDataList1.CidList:
                        initialDataList1.addCidNode(j["CId"])
            if i.has_key("J"):
                for j in i["J"]:
                    if j.has_key("JId") and j not in initialDataList1.CidList:
                        initialDataList1.addCidNode(j["JId"])

        for i in data2["entities"]:
            if i.has_key("RId"):
                for j in i["RId"]:
                    if j not in initialDataList2.RidList:
                        initialDataList2.addIdNode(j)
            if i.has_key("AA"):
                for j in i["AA"]:
                    if j.has_key("AuId") and j not in initialDataList2.AuidList:
                        initialDataList2.addAuidNode(j["AuId"])
                    if j.has_key("AfId") and j not in initialDataList2.AfidList:
                        initialDataList2.addAfidNode(j["AfId"])
            if i.has_key("F"):
                for j in i["F"]:
                    if j.has_key("FId") and j not in initialDataList2.FidList:
                        initialDataList2.addFidNode(j["FId"])
            if i.has_key("C"):
                for j in i["C"]:
                    if j.has_key("CId") and j not in initialDataList2.CidList:
                        initialDataList2.addCidNode(j["CId"])
            if i.has_key("J"):
                for j in i["J"]:
                    if j.has_key("JId") and j not in initialDataList2.CidList:
                        initialDataList2.addCidNode(j["JId"])

        # compute the path for 1 hop, 2 hop and 3 hop 
        # 1 hop 
        res = []
        if id2Type == 0:
            # RId and Id 
            if id2 in initialDataList1.RidList:
                res.append([int(id1), int(id2)])
        else:
            # AuId : AuId 
            if id2 in initialDataList1.AuidList:
                res.append([int(id1), int(id2)])
        # 2 hop
        if id2Type == 0: # id2 is paper id 
            # Fid and Id 
            for i in initialDataList1.FidList:
                if self.checkExists(FId = i, Id = id2):
                    res.append([int(id1), int(i), int(id2)])
            # AuId and Id
            for i in initialDataList1.AuidList:
                if self.checkExists(AuId = i, Id = id2):
                    res.append([int(id1), int(i), int(id2)])
            # CId and Id 
            for i in initialDataList1.CidList:
                if self.checkExists(CId = i, Id = id2):
                    res.append([int(id1), int(i), int(id2)])
            # JId and Id 
            for i in initialDataList1.JidList:
                if self.checkExists(JId = i, Id = id2):
                    res.append([int(id1), int(i), int(id2)])
        else: # id2 is AuId
            # AfId and AuId 
            for i in initialDataList1.AfidList:
                if self.checkExists(AfId = i, AuId = id2):
                    res.append([int(id1), int(i), int(id2)])
        # 3 hop 
        # RId : RId 
        
        self.write(json.dumps(res))
        print (time.time() - self.begin_time) * 1000, "ms"
        self.finish()

   
    @tornado.gen.coroutine 
    def sendRequest(self, parameters_dict):
        data = self.redis.get(parameters_dict["expr"])
        data = None
        if data is not None:
            #return data 
            pass
        else:
            get_params = urllib.urlencode(parameters_dict)
            url = self.API_URL % get_params
            # request 
            request = HTTPRequest(url, method='GET')
            request.headers['Content-Type'] = 'application/json'
            request.headers['Ocp-Apim-Subscription-Key'] = self.TOKEN 
            # fetch 
            client = AsyncHTTPClient()
            #response  = yield client.fetch(request, self.handleResponse)
            response  = yield client.fetch(request)
            raise tornado.gen.Return(response.body) 
            #client = HTTPClient()
            #response = client.fetch(request)
            #self.redis.set(parameters_dict["expr"], response.body)

    #def handleResponse(self, response):
    #    self.end_time = time.time()
    #    print (self.end_time - self.begin_time) * 1000, "ms" 
    #    return response.body 

   
    def getEntitiesById(self, x = "2296319761"):
        attributes = "Id,RId,AA.AuId,AA.AfId,F.FId"
        parameters_dict = {
            'expr': "Id=%s" % (x, ),
            #'model': '',
            'count': 1000,
            #'offset': 0,
            #'orderby' : '',
            'attributes' : attributes
            }
        yield self.sendRequest(parameters_dict)

    def getEntitiesByAuid(self, x):
        attributes = "Id,RId,AA.AuId,AA.AfId,F.FId"
        parameters_dict = {
            'expr': "Composite(AA.AuId=%s)" % (x, ),
            #'model': '',
            'count': 1000,
            #'offset': 0,
            #'orderby' : '',
            'attributes' : attributes
            }
        return self.sendRequest(parameters_dict) 

    def checkExists(self, Id = None, AuId = None, FId = None, AfId = None, CId = None, JId = None):
        attributes = "Id"
        expr = "And("
        if Id != None:
            expr = expr + "Id=" + str(Id)
        if AuId != None:
            expr = expr + ",Composite(AA.AuId=%s)" % (str(AuId,))
        if FId != None:
            expr = expr + ",Composite(F.FId=%s)" % (str(FId,))
        if AfId != None:
            expr = expr + ",Composite(AA.AfId=%s)" % (str(AfId,))
        if CId != None:
            expr = expr + ",Composite(C.Id=%s)" % (str(CId,))
        if JId != None:
            expr = expr + ",Composite(J.Id=%s)" % (str(JId,))
        expr = expr + ")"
        parameters_dict = {
            'expr': expr,
            #'model': '',
            'count': 1000,
            #'offset': 0,
            #'orderby' : '',
            'attributes' : attributes
            }
        print "checkExists:", expr
        response = self.sendRequest(parameters_dict) 
        data = json.loads(response)
        if data.has_key("entities") and len(data["entities"]) > 0:
            return True 
        return False 

    
    # test getEntitiesById
    def testEntities(self):
        data = self.getEntitiesById()
        self.write(data)
        print (time.time() - self.begin_time) * 1000, "ms"
        self.finish()
        
    # test request by author name 
    def test1(self):
        attributes = "Id,Ti,AA.AuN,AA.AfId,F.FId,RId"
        parameters_dict = {
                'expr': "Composite(AA.AuN=='jaime teevan')",
                #'model': '',
                #'count': 1000,
                #'offset': 0,
                #'orderby' : '',
                'attributes' : attributes
                }
        data = self.sendRequest(parameters_dict)
        self.write(data)
        print (time.time() - self.begin_time) * 1000, "ms"
        self.finish()

    # test request by id 
    def test2(self):
        #attributes = "Id,AA.AuN,AA.AfId,F.FId,F.FN,RId"
        attributes = "Id,RId,AA.AuId,AA.AfId,F.FId"
        parameters_dict = {
                #'expr': "Id=2296319761",
                'expr': "Composite(F.FId=90119067)",
                #'model': '',
                'count': 1000,
                #'offset': 0,
                #'orderby' : '',
                'attributes' : attributes
                }
        self.sendRequest(parameters_dict)
        data  = self.sendRequest(parameters_dict)
        self.write(data)
        print (time.time() - self.begin_time) * 1000, "ms"
        self.finish()




# list node
class Node(object):
    nodeId = None 
    preNode = None 
    def __init__(self, nodeid, pre = None):
        self.nodeId = nodeid 
        self.preNode = pre 

# represent all entities linked to some pre datalist
class DataList(object):
    RidList = []
    AuidList = [] 
    FidList = [] 
    JidList = [] 
    CidList = [] 
    AfidList = []
    preDataList = None 
    def __init__(self, preDataList = None):
        self.preDataList = preDataList
    def addIdNode(self, x):
        if x not in self.RidList:
            self.RidList.append(x)
    def addAuidNode(self, x):
        if x not in self.AuidList:
            self.AuidList.append(x)
    def addFidNode(self, x):
        if x not in self.FidList:
            self.FidList.append(x)
    def addJidNode(self, x):
        if x not in self.JidList:
            self.JidList.append(x)
    def addCidNode(self, x):
        if x not in self.CidList:
            self.CidList.append(x)
    def addAfidNode(self, x):
        if x not in self.AfidList:
            self.AfidList.append(x)
