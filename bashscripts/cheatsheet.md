## a cheat sheet for some common utility commands and bash script programming on Linux platform


### common utilities commands

+ system infomation and configuration
  + summary
    + ps/top/mpstat
  + process 
    + ps/pidstat
  + memory 
    + free/vmstat/sar
  + cpu 
    + sar 
  + netowrk
    + netstat/sar/ssh/nc/ip/route/ifconfig/rsync/scp/iptables
  + filesystem 
    + du/lsblk/df/fdisk/mount/find 
  + disk io
    + sar/iostat 
  + others
    + ulimit/sysctl/getconf

+ string processing tools
  + grep/egrep/wc/sort/head/tail/cut/awk/sed/tr/nl/iconv/find/xargs

+ binary processing tools
  + objdump/objcopy/readelf/nm/hexdump

+ compile/debug/trace/perf tools 
  + gcc/g++/gdb/clang/clang++/lldb/strace/valgrind/ldconfig/
    autoscan/aclocal/autoconf/automake/make/cmake/
    ftrace/systemtap/perf/oprofile/kdump/crash

+ more


----


```
+ ps
  + unix options with slash
    + -e                 every process
    + -u[username]       processes of username 
    + -L                 light weight process(thread)
    + -f                 full format listing
    + -a                 all processes except session leaders and processes not attached to terminal 
    + -d                 all processes except session leaders 
    + -u[username]       all processes of user username
    + -p(--pid)[pid]     process with pid 
    + -s[sessionid]      processes with session id 
    + -g[groupid]        processes in group 
    + -t[tty]            processes in tty 
  + BSD style options without slash
    + a                  all processes with a terminal(tty)
    + r                  running processes 
    + t/T                processes attached to terminal
    + x                  all processes owned by you 
    + ax                 all processes 
    + p[pid]             process with pid
    + X                  show register content of EIP/ESP/STACKP
    + s                  show signal masks
    + v                  show virtual memory 
    + e                  show environment info
    + f                  show a process tree 
    + m                  show threads of each process 
  + some common combinations 
    + ps aux             all processes 
    + ps -ef             all processes with full formatting list 
    + ps aux -L          all thread 
    + ps -f --ppid 2     all kernel threads 

```

----


```
+ head 
  + -n[k]                first k lines
  + -n[-k]               all except last k lines 

```

----


```
+ tail 
  + -f[filename]         follow while file content growing
  + -n[k]                last k lines 
  + -n[+k]               from kth line to end 

```

----


```
+ wc
  + -m                   char count
  + -l                   line count 
  + -w                   word count 
  + -L                   longest line length
  + -c                   byte count 

```


----


```
+ cut 
  + -d[delimiter]        use to split each line to fields
  + -f[number]           select only the number fileds
  + -d' ' -f1,3          use space to split each line and print 1,3 fields 

```

----


```
+ sort
  + -u                   sort file and keep unique contents
  + -r                   reverse sort 
  + -R                   random sort 

```





----
----



### bash script programming 

```
echo $#  # arguments number
echo $0  # exec filename
echo $1  # first argument
echo $2  # second argument 
echo "$*"  # all arguments as a whole string   
echo "$@"  # all arguments as an array
echo $!  # last executed cmd pid
echo $?  # exec status 
echo $$  # current bash script process pid 

# some configurations for output color
RED_COLOR='\E[1;31m'
GREEN_COLOR='\E[1;32m'
YELLOW_COLOR='\E[1;33m'
BLUE_COLOR='\E[1;34m'
END_COLOR='\E[0m'
echo -e "${RED_COLOR}installing basic utilities...${END_COLOR}"


if [ -e "/bin/bash" ]; then
  echo "yes"
else
  echo "no"
fi


read temp 
case $temp in
  1)
    echo "1"
    ;;
  2)
    echo "2"
    ;;
  [3-9])
    echo "3-9"
esac


for i in "$*"; do
  echo $i
done

for i in `ls`; do 
  echo $i
done

for (( i = 0; i < $#; i++ )); do
  echo $i
done 

x=1
while [[ $x -le $# ]]; do
  echo $x
  let x=x+1
done


function testFunc1 {
  echo "in testFunc1"
}
testFunc2() {
  echo "in testFunc2", $*
}
testFunc1
testFunc2 123 456


-e filename   =>  file exists 
-d dirname    =>  dir exists 
-f filename   =>  regular file
-L filename   =>  symbol link 
-r filename   =>  can be read 
-w filename   =>  can be written
-x filename   =>  can be executed 
filename1 -nt filename2   =>  newer than 
filename1 -ot filename2   =>  older than 

-z string        =>  null string 
-n string        =>  not null string 
string1=string2  =>  equal
string1!=string2 =>  not equal 

num1 -eq num2    =>  = 
num1 -ne num2    =>  != 
num1 -lt num2    =>  < 
num1 -le num2    =>  <=
num1 -gt num2    =>  >
num1 -ge num2    =>  >=

```
