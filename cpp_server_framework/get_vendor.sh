mkdir vendor
cd vendor && wget https://github.com/miloyip/rapidjson/archive/master.zip
/usr/bin/unzip master.zip > /dev/null 
mv rapidjson-master/include/rapidjson ./ 
rm -rf ./rapidjson-master 
rm master.zip 
