#!/bin/bash 
CUR=$(cd `dirname $0`; pwd)
cd /tmp && git clone https://github.com/golang/image
sudo cp -r image/font/gofont/ttfs /usr/share/fonts/gofont
rm -rf image
cd $CUR

sudo mkfontscale && sudo mkfontdir && sudo fc-cache
