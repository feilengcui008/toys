#!/usr/bin/env python
import base64
class ProxyMiddleware(object):
    def process_request(self,request,spider):
        request.meta['proxy'] = "http://202.117.3.57:8000"
        user_pass = "xjtu:flyover2013"
        encoded = base64.encodestring(user_pass)
        request.headers['Proxy-Authorization'] = 'Basic '+encoded
