#!/usr/bin/env python
# -*-coding:utf-8 -*-
import tornado.httpserver
import tornado.ioloop
import sys
import os
from tornado.options import options

##add the module path
sys.path.insert(0,os.path.dirname(__file__))
import application


if __name__ == "__main__":
    tornado.options.parse_command_line()
    server = tornado.httpserver.HTTPServer(application.Application())
    server.listen(options.PORT)
    tornado.ioloop.IOLoop.instance().start()

