#!/usr/bin/env python
# -*- coding:utf-8 -*- 

import tornado.web
#import torndb
from tornado.httpclient import AsyncHTTPClient, HTTPRequest, HTTPResponse
import tornado.gen
import uuid 
import time
import json
import urllib 
import os 
import sys 
import SimpleCV as scv 
import cv2


COMIC_IMAGES = [(43, 29, 106, 106), (80, 130, 125, 125), (49, 69, 75, 75),
                (60, 52, 150, 163), (83, 39, 123, 106), (164, 105, 104, 99),
                (151, 123, 140, 140), (51, 101, 137, 116), (44, 32, 74, 74)]


class BaseHandler(tornado.web.RequestHandler):
    pass         

class FaceHandler(BaseHandler):
    API_URL = 'https://api.projectoxford.ai/face/v0/detections?%s'
    TOKEN = '61fc12aaa7764417bf96a99e1cf9281e'
    IMAGE_PATH = '/image/'
    CLIENT_IMG_PATH = '/client_imgs/'
    HOST = 'http://121.42.29.143:8000/'
    client_saved_img = None 
    client_cv_img = None
    client_img_params = None 
    comic_picture_id = 1
    returned_url = ''
    threash_hold = 90

    def initialize(self):
        self.CLIENT_IMG_PATH = self.settings['static_path'] + self.CLIENT_IMG_PATH 
        self.IMAGE_PATH = self.settings['static_path'] + self.IMAGE_PATH  
    
    def get(self):
        self.render("index.html")

    @tornado.web.asynchronous
    def post(self):
        # param from the client 
        message = {}
        message['status'] = 'error'
        message['returned_url'] = 'null'
        #url = self.get_argument('url', None)
        comic_id = self.get_argument('img', None)
        self.threash_hold = self.get_argument('threshold', 90)
        if not self.threash_hold:
            self.threash_hold = float(90)
        else:
            self.threash_hold =  float(self.threash_hold)
        if comic_id:
            comic_id = int(comic_id[5:])
        if len(self.request.files['file']) > 0:
            img_file = self.request.files['file'][0]
        # check the client params  
        if not img_file or comic_id <= 0:
            self.write(json.dumps(message))
            self.finish()
        else:
            self.comic_picture_id = comic_id 

        ######################################################
        # print self.client_cv_img 
        filename = img_file['filename']
        saved_path = self.IMAGE_PATH + filename 
        self.client_saved_img = saved_path 
        if not os.path.exists(filename):
            with open(saved_path, "wb") as fp:
                fp.write(img_file['body'])

        # save the client img 
        self.client_cv_img = scv.Image(r'%s' % str(saved_path)) 

        ######################################################
        # ok, we save the client image and gen the SimpleCV img 
        # we pass the client img url to oxford to get the params 
        # get parameters 
        get_params = urllib.urlencode({
                'analyzesFaceLandmarks': 'true',
                'analyzesAge': 'true',
                'analyzesGender': 'true',
                'analyzesHeadPose': 'true',
                })
        url = self.API_URL % get_params
        post_params = {}
        post_params['url'] = self.HOST + saved_path  
        #print post_params['url']
        # request 
        request = HTTPRequest(url, method='POST')
        request.body = json.dumps(post_params)
        request.headers['Content-Type'] = 'application/json'
        request.headers['Ocp-Apim-Subscription-key'] = self.TOKEN 
        # fetch 
        client = AsyncHTTPClient()
        client.fetch(request, self.handleResponse)

    # for face api return handling
    def handleResponse(self, response):
        #response_data = {}
        #response_data['status'] = 'error'
        #response_data['url'] = 'null'
        client_img_params = json.loads(response.body)
        self.client_img_params = client_img_params 
        #print client_img_params 
        self.genPicture()
        #response_data['url'] = returned_url 
        self.render("show.html", returned_img_url = self.returned_url)
        #self.finish()

    def genPicture(self):
        #print self.client_cv_img 
        self.crop_img()
        #return self.IMAGE_PATH+"image1.png"
        self.scale_img()
        self.merge_img()

    def center(self, top, left, width, hight):
        return (left+width/2.0, top+hight/2.0)

    def crop_img(self):
        rectangle = self.client_img_params[0]['faceRectangle']

        faceLandmarks = self.client_img_params[0]['faceLandmarks']

        # Find the highest point in eyebrow.
        eyebrowLeftOuter = faceLandmarks['eyebrowLeftOuter'];
        eyebrowRightOuter = faceLandmarks['eyebrowRightOuter'];

        # Find the lowest point.
        underLipBottom = faceLandmarks['underLipBottom']

        # Calc the rectangle of Box.
        x = eyebrowLeftOuter['x']
        y = eyebrowLeftOuter['y']

        width = eyebrowRightOuter['x'] - eyebrowLeftOuter['x']
        height = underLipBottom['y'] - eyebrowRightOuter['y']

        self.client_cv_img = self.client_cv_img.crop(x, y, width, height)



        #point1 = Point(rectangle['top'], rectangle['left'])
        #point2 = Point(rectangle['top']+rectangle['width'], rectangle['left']+rectangle['height'])
        #self.client_cv_img = self.client_cv_img.crop(rectangle['left'], rectangle['top'], rectangle['width'], rectangle['height'])
        path = "%s%s" % (self.IMAGE_PATH, "crop.jpg")
        self.client_cv_img.save(path)
        
    def scale_img(self):
        imgid = self.comic_picture_id - 1
        self.client_cv_img = self.client_cv_img.binarize(self.threash_hold).invert().scale(COMIC_IMAGES[imgid][2], COMIC_IMAGES[imgid][3])
        path = "%s%s.jpg" % (self.CLIENT_IMG_PATH, str(uuid.uuid4().int))
        #self.client_cv_img.save("%s%s" % (self.CLIENT_IMG_PATH, "binarize.jpg"))
        self.client_cv_img.save(path)
        #self.optimize_scale(path)


    def optimize_scale(self, filename, percentage = 0.01):
        begin = False
        img = cv2.imread(filename)
        dot_list = img.tolist()
        height = len(dot_list)
        width = len(dot_list[0])
        
        for i in range(height): # each line
            for j in range(int(width*percentage)): # each dot 
                if equal(img[i][j], (0, 0, 0)):
                    begin = True
                if begin:
                    if equal(img[i][j], (255, 255, 255)):
                        break;
                    else:
                        img[i][j] = (255, 255, 255)
            begin = False 
            for j in range(int(width*percentage)):
                if equal(img[i][int(width*percentage)-j-1], (0, 0, 0)):
                    begin = True 
                if begin:
                    if equal(img[i][int(width*percentage)-j-1], (255, 255, 255)):
                        break;
                    else:
                        img[i][int(width*percentage)-j-1] = (255, 255, 255)
        newpath = filename[:-4] + "_optimized.jpg" 
        cv2.imwrite(newpath, img)    
        self.client_cv_img = scv.Image(newpath)
        print self.client_cv_img
        
    def merge_img(self):
        imgid = self.comic_picture_id - 1
        comic_img = scv.Image("%simage%s.jpg" % (self.IMAGE_PATH, str(self.comic_picture_id)))
        comic_img_layer = comic_img.dl()
        comic_img_layer.blit(self.client_cv_img, (COMIC_IMAGES[imgid][0], COMIC_IMAGES[imgid][1]))
        self.returned_url = "%s%s.jpg" % (self.CLIENT_IMG_PATH, str(uuid.uuid4().int))
        comic_img.save(self.returned_url)

class Point(object):
    def __init__(self, x, y):
        self.x = x 
        self.y = y

def equal(src, target):
    result = True 
    for i in src:
        for j in target:
            if i != j:
                result = False 
                break 
    return result 

