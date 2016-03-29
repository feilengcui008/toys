# -*- coding: utf-8 -*-

# Define here the models for your scraped items
#
# See documentation in:
# http://doc.scrapy.org/en/latest/topics/items.html

import scrapy


class HualvtuSpiderItem(scrapy.Item):
    # define the fields for your item here like:
    # name = scrapy.Field()
    link = scrapy.Field()

class CityItem(scrapy.Item):
    city_name = scrapy.Field()
    city_link = scrapy.Field()
    city_id = scrapy.Field()

class AlbumItem(scrapy.Item):
    album_name = scrapy.Field()
    album_link = scrapy.Field()

class PictureItem(scrapy.Item):
    picture_link = scrapy.Field()

