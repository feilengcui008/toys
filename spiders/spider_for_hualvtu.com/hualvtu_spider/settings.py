# -*- coding: utf-8 -*-

# Scrapy settings for hualvtu_spider project
#
# For simplicity, this file contains only the most important settings by
# default. All the other settings are documented here:
#
#     http://doc.scrapy.org/en/latest/topics/settings.html
#

BOT_NAME = 'hualvtu_spider'

SPIDER_MODULES = ['hualvtu_spider.spiders']
NEWSPIDER_MODULE = 'hualvtu_spider.spiders'

DOWNLOADER_MIDDLEWARES = {
    'scrapy.contrib.downloadermiddleware.httpproxy.HttpProxyMiddleware':110,
    'hualvtu_spider.middleware.ProxyMiddleware':100,
}

# Crawl responsibly by identifying yourself (and your website) on the user-agent
#USER_AGENT = 'hualvtu_spider (+http://www.yourdomain.com)'
