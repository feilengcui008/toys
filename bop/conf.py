#!/usr/bin/env python
# -*- coding:utf-8 -*- 
from tornado.options import define,options
import os

#server config
define("PORT", default=4000, help="run on the given port", type=int)

#database config
define("MYSQL_HOST", default="127.0.0.1:3306", help="database host")
define("MYSQL_DATABASE", default="xjtu_info", help="database name")
define("MYSQL_USER", default="***", help="database user")
define("MYSQL_PASSWORD", default="***", help="database password")

#application config
define("TEMPLATE_PATH", default=os.path.join(os.path.dirname(__file__),"templates"),help="the templates path")
define("STATIC_PATH", default=os.path.join(os.path.dirname(__file__),"static"))
define("DEBUG",default=False)
define("XSRF_COOKIES",default=False)
define("COOKIE_SECRET",default="mykey")
define("LOGIN_URL",default="/login")
