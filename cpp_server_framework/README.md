# cpp_server_framework

a light weight linux C++ server development framework(or library) 

now finish the poll + epoll mode, poll + multithread mode, actually simple testing of the performance shows it's not so bad :-) (compared to nginx + echo module)

+ 1.extract the common stuff of servers, use poll for the server 
listening socket, as the socket is just a few so we don't need 
epoll here 
+ 2.leave the choice of multi-process(maybe useful), multi-thread, 
epoll(one thread one loop)(may be one epoll handle thread or more)
to the specific server developer
+ 3.solve the stream handleRead and handleWrite logic in framework 
and leave handleRequest logic to specific-server developer so the 
developer can implements a web server, game server, ftp...etc. 
+ 4.maybe add ficilities like threadpool, Connection cache...
+ 5.in developing

example usage : 

see ExampleServer.h 
./get_vendor.sh 
cd src && cmake . && make && ./example_server conf.json

