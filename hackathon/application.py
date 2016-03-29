#!/usr/bin/env python
# -*- coding:utf-8 -*-
import tornado.web
import os
import conf
from tornado.options import options
from handlers import *

class Application(tornado.web.Application):
    def __init__(self):
        handlers = [
            (r'/', FaceHandler),
        ]

        settings = dict(
            template_path = options.TEMPLATE_PATH,
            static_path = options.STATIC_PATH,
            xsrf_cookies=options.XSRF_COOKIES,
            cookie_secret=options.COOKIE_SECRET,
            debug=options.DEBUG,
            login_url=options.LOGIN_URL,
            #autoescape=None,
            )
        tornado.web.Application.__init__(self, handlers, **settings)
        #self.db = torndb.Connection(
        #    host=options.MYSQL_HOST, database=options.MYSQL_DATABASE,
        #    user=options.MYSQL_USER, password=options.MYSQL_PASSWORD)
        #self.add_handlers("blog",sub_handlers)
