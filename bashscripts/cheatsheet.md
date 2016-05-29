#### a cheat sheet for some common utility commands on Linux platform
to add more


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

+ head 
  + -n[k]                first k lines
  + -n[-k]               all except last k lines 

+ tail 
  + -f[filename]         follow while file content growing
  + -n[k]                last k lines 
  + -n[+k]               from kth line to end 

+ wc
  + -m                   char count
  + -l                   line count 
  + -w                   word count 
  + -L                   longest line length
  + -c                   byte count 

+ cut 
  + -d[delimiter]        use to split each line to fields
  + -f[number]           select only the number fileds
  + -d' ' -f1,3          use space to split each line and print 1,3 fields 

+ sort
  + -u                   sort file and keep unique contents
  + -r                   reverse sort 
  + -R                   random sort 
