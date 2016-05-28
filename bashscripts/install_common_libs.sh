#!/bin/bash 

# some arguments from cmdline 

LIB_PATH=$PWD
JOBS=1
if [ -n $1 ]; then 
  JOBS=$1
fi

# some configurations for output color
RED_COLOR='\E[1;31m'
GREEN_COLOR='\E[1;32m'
YELLOW_COLOR='\E[1;33m'
BLUE_COLOR='\E[1;34m'
END_COLOR='\E[0m'

# basic utilities
echo -e "${RED_COLOR}installing basic utilities...${END_COLOR}"
sudo apt-get -y install gcc g++ gdb valgrind binutils autoconf automake make #cmake 

# basic libs 
echo -e "${RED_COLOR}installing basic libs...${END_COLOR}"
sudo apt-get -y install libboost1.55-all-dev openssl libcurl3


#### C++ libs ####

# libevent 
cd $LIB_PATH 
if [ ! -d "Libevent" ]; then
  echo -e "${RED_COLOR}installing Libevent...${END_COLOR}"
  git clone https://github.com/nmathewson/Libevent 
  cd Libevent 
  #./autogen.sh 
  #./configure && make -j 2
  mkdir build && cd build 
  cmake .. -DEVENT__BUILD_SHARED_LIBRARIES=ON && make -j $JOBS 
  sudo make install 
else
  echo -e "${RED_COLOR}Libevent has been installed${END_COLOR}"
fi 

# glog
cd $LIB_PATH
if [ ! -d "glog" ]; then 
  echo -e "${RED_COLOR}installing glog...${END_COLOR}"
  git clone https://github.com/google/glog
  cd glog
  mkdir build && cd build
  #export CXXFLAGS="-fPIC" && cmake .. && make -j 2
  cmake .. -DBUILD_SHARED_LIBS=ON && make -j $JOBS
  sudo make install 
else 
  echo -e "${RED_COLOR}glog has been installed${END_COLOR}"
fi 

# gflags
cd $LIB_PATH
if [ ! -d "gflags" ]; then 
  echo -e "${RED_COLOR}installing gflags...${END_COLOR}"
  git clone https://github.com/gflags/gflags
  cd gflags
  mkdir build && cd build 
  cmake .. -DBUILD_SHARED_LIBS=ON && make -j $JOBS 
  sudo make install 
else
  echo -e "${RED_COLOR}gflags has been installed${END_COLOR}"
fi 

# googletest 
cd $LIB_PATH 
if [ ! -d "googletest" ]; then
  echo -e "${RED_COLOR}installing googletest...${END_COLOR}"
  git clone https://github.com/google/googletest 
  cd googletest 
  mkdir build && cd build 
  cmake .. -DBUILD_SHARED_LIBS=ON && make -j $JOBS
  sudo make install 
else 
  echo -e "${RED_COLOR}googletest has been installed${END_COLOR}"
fi

# protobuf 
#cd $LIB_PATH 
#if [ ! -d "protobuf" ]; then 
#  echo -e "${RED_COLOR}installing protobuf...${END_COLOR}"
#  git clone https://github.com/google/protobuf
#  ./autogen.sh 
#  ./configure && make -j $JOBS
#  sudo make install 
#else 
#  echo -e "${RED_COLOR}protobuf has been installed${END_COLOR}"
#fi 

# grpc and protobuf
cd $LIB_PATH 
if [ ! -d "grpc" ]; then
  echo -e "${RED_COLOR}installing grpc and protobuf...${END_COLOR}"
  git clone https://github.com/grpc/grpc.git
  cd grpc
  git submodule update --init
  make -j $JOBS
  cd third_party/protobuf 
  sudo make install 
  cd $LIB_PATH/grpc
  sudo make install
else 
  echo -e "${RED_COLOR}grpc and protobuf has been installed${END_COLOR}"
fi

# folly 
cd $LIB_PATH 
if [ ! -d "folly" ]; then 
  echo -e "${RED_COLOR}installing folly...${END_COLOR}"
  # cannot use local build gflags, glog and libevent... 
  sudo apt-get -y install libgflags-dev libgoogle-glog-dev
  git clone https://github.com/facebook/folly
  cd folly/folly 
  autoreconf -ivf 
  ./configure
  make
  sudo make install
else 
  echo -e "${RED_COLOR}folly has been installed${END_COLOR}"
fi

# thrift
cd $LIB_PATH 
if [ ! -d "thrift" ]; then
  echo -e "${RED_COLOR}installing thrift...${END_COLOR}"
  git clone https://github.com/apache/thrift.git 
  cd thrift 
  ./bootstrap.sh 
  ./configure
  make -j $JOBS 
  sudo make install 
  cd lib/java
  ant
else 
  echo -e "${RED_COLOR}thrift has been installed${END_COLOR}"
fi
# wangle
# proxygen 






########### some notes for bash grammar ##############

#echo $#  # arguments number
#echo $0  # exec filename
#echo $1  # first argument
#echo $2  # second argument 
#echo "$*"  # all arguments as a whole string   
#echo "$@"  # all arguments as an array
#echo $!  # last executed cmd pid
#echo $?  # exec status 
#echo $$  # current bash script process pid 


#if [ -e "/bin/bash" ]; then
#  echo "yes"
#else
#  echo "no"
#fi


#read temp 
#case $temp in
#  1)
#    echo "1"
#    ;;
#  2)
#    echo "2"
#    ;;
#  [3-9])
#    echo "3-9"
#esac


#for i in "$*"; do
#  echo $i
#done

#for i in `ls`; do 
#  echo $i
#done

#for (( i = 0; i < $#; i++ )); do
#  echo $i
#done 

#x=1
#while [[ $x -le $# ]]; do
#  echo $x
#  let x=x+1
#done


#function testFunc1 {
#  echo "in testFunc1"
#}
#testFunc2() {
#  echo "in testFunc2", $*
#}
#testFunc1
#testFunc2 123 456


#-e filename   =>  file exists 
#-d dirname    =>  dir exists 
#-f filename   =>  regular file
#-L filename   =>  symbol link 
#-r filename   =>  can be read 
#-w filename   =>  can be written
#-x filename   =>  can be executed 
#filename1 -nt filename2   =>  newer than 
#filename1 -ot filename2   =>  older than 

#-z string        =>  null string 
#-n string        =>  not null string 
#string1=string2  =>  equal
#string1!=string2 =>  not equal 

#num1 -eq num2    =>  = 
#num1 -ne num2    =>  != 
#num1 -lt num2    =>  < 
#num1 -le num2    =>  <=
#num1 -gt num2    =>  >
#num1 -ge num2    =>  >=
