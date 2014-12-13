#!/usr/bin/env python
import base64
class ProxyMiddleware(object):
    def process_request(self,request,spider):
        request.meta['proxy'] = YOUR_PROXY_IP
        user_pass = "USERNAME:PASSWORD"
        encoded = base64.encodestring(user_pass)
        request.headers['Proxy-Authorization'] = 'Basic '+encoded
