#!/usr/bin/env python
# -*- coding: utf-8 -*-

""" 
a spider for freebuf.com using scrapy framework and BeautifulSoup
"""

import scrapy
import os
import BeautifulSoup as bs
from scrapy.http import Request
from freebuf_spider.items import SpiderItem

# storage the items seperated by page number in ./OUTPUT dir
OUTPUT = os.path.join(os.path.dirname(os.path.realpath(__file__)), "OUT/")

class MySpider(scrapy.Spider):
    name = "my"
    allowed_domains = ["freebuf.com"]
    start_urls = ("http://www.freebuf.com/page/1", )
    def parse(self, response):
        if "freebuf.com/page"  in response.url:
            soup = bs.BeautifulSoup(response.body)
            # handle next_page
            next_page_url = soup.find('div', {'id': 'pagination'})
            next_page_url = next_page_url.findChild('a').get('href')
            print "-" * 20 + next_page_url + "-" * 20
            yield Request(next_page_url,callback=self.parse)
            # handle current page
            current_page = response.url[28:]
            divs = soup.findAll('div', {'class': 'news_inner'})
            for i in divs:
                j = i.findChild('dt')
                if j is not None:
                    a = j.findChild('a')
                    new_item = SpiderItem()
                    new_item["name"] = a.get("title")
                    new_item["link"] = a.get("href")
                    req =  Request(new_item["link"], callback=self.storage_item)
                    req.item_title = new_item['name']
                    req.current_page = current_page
                    yield req

    def storage_item(self,response):
        pagedir = "%s%s%s/" % (OUTPUT, "page", response.request.current_page)
        if not os.path.exists(pagedir):
            os.makedirs(pagedir)
        filename = pagedir+response.request.item_title
        with open(filename,"w+") as fp:
            fp.write(response.body)
