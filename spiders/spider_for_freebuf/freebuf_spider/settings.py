#!/usr/bin/env python
# -*- coding: utf-8 -*-

# Scrapy settings for freebuf_spider project
#
# For simplicity, this file contains only the most important settings by
# default. All the other settings are documented here:
#
#     http://doc.scrapy.org/en/latest/topics/settings.html
#

BOT_NAME = 'freebuf_spider'

SPIDER_MODULES = ['freebuf_spider.spiders']
NEWSPIDER_MODULE = 'freebuf_spider.spiders'

# Crawl responsibly by identifying yourself (and your website) on the user-agent
#USER_AGENT = 'freebuf_spider (+http://www.yourdomain.com)'
