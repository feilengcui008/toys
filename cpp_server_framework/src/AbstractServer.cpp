// Copyright [2016] <feilengcui008@gmail.com>
#include "AbstractServer.h"

#include <string>
#include <fstream>
#include <iostream>

#include "../vendor/rapidjson/document.h"

namespace Tan {
AbstractServer::AbstractServer(std::string host, unsigned short port,
                               unsigned int backlog) {
    host_ = host;
    port_ = port;
    backlog_ = backlog;
    // some default parameters
    mode_ = EPOLL;  // default epoll mode for client sockets
    rootdir_ = "";
    logdir_ = rootdir_ + "/log";
    keepalived_ = false;
    timeout_ = 2000;  // ms
    max_connection_ = 8000;
    reuseaddr_ = true;
    serverfd_ = -1;
    nonblocking_ = true;
    current_connection_num_ = 0;
    history_max_connection_ = 0;
    running_ = false;
    initialized_ = false;
    poll_timeout_ = -1;
    for (int i = 0; i < SERVER_POLL_NUMBER; i++) {
        pollfds_[i].fd = -1;
    }
}

AbstractServer::~AbstractServer() {
    if (mode_ == THREAD) {
        if (threadHandler_ != nullptr) {
            delete threadHandler_;
            threadHandler_ = nullptr;
        }
    } else if (mode_ == PROCESS) {
        if (processHandler_ != nullptr) {
            delete processHandler_;
            processHandler_ = nullptr;
        }
    } else {
        if (mode_ == EPOLL) {
            if (epollHandler_ != nullptr) {
                delete epollHandler_;
                epollHandler_ = nullptr;
            }
        }
    }
}

// we create the needed sockets and init the specific
// parameters need by different modes
void AbstractServer::init() {
    if (initialized_) return;
    // create the server sockets and pollfd_
    int ret;
    serverfd_ = socket(AF_INET, SOCK_STREAM, 0);
    if (serverfd_ < 0) {
        my_error_exit("server init error : create server socket failed\n");
    }
    if (reuseaddr_) {
        int opt = 1;
        ret =
            setsockopt(serverfd_, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
        if (ret < 0) {
            my_error_exit("server init error : set reuseaddr failed\n");
        }
    }
    if (nonblocking_) makeNonblock(serverfd_);

    struct hostent *h = gethostbyname(host_.c_str());
    if (!h) {
        close(serverfd_);
        my_error_exit("abstractserver init error : gethostbyname error\n");
    }
    sockaddr_in in;
    memset(&in, 0, sizeof(struct sockaddr_in));
    in.sin_family = AF_INET;
    in.sin_port = htons(port_);
    in.sin_addr = *(struct in_addr *)h->h_addr;

    ret = bind(serverfd_, (struct sockaddr *)&in, sizeof(struct sockaddr_in));
    if (ret < 0) {
        close(serverfd_);
        my_error_exit("abstractserver init error : bind error\n");
    }

    ret = listen(serverfd_, backlog_);
    if (ret < 0) {
        close(serverfd_);
        my_error_exit("abstractserver init error : listen error\n");
    }

    // add the server socket to poll
    pollfds_[0].fd = serverfd_;
    pollfds_[0].events = POLLIN;
    pollfds_[0].revents = 0;

    // initilize the mode-specific parameters
    if (mode_ == PROCESS) {
        // processHandler_ = std::make_shared<ConnectionProcessHandler>
        //                                 (ConnectionProcessHandler());
        processHandler_ = new ConnectionProcessHandler();
        processHandler_->init();
        processHandler_->setServer(this);
    }

    if (mode_ == THREAD) {
        // threadHandler_ = std::make_shared<ConnectionThreadHandler>
        //                                (ConnectionThreadHandler());
        threadHandler_ = new ConnectionThreadHandler();
        threadHandler_->init();
        threadHandler_->setServer(this);
    }

    // create the EpollThread to handle client connections if in EPOLL mode
    if (mode_ == EPOLL) {
        // epollHandler_ = std::make_shared<ConnectionEpollHandler>
        //                               (ConnectionEpollHandler());
        epollHandler_ = new ConnectionEpollHandler();
        epollHandler_->init();
        epollHandler_->setServer(this);
    }

    initialized_ = true;
}

// start the server
void AbstractServer::start() {
    if (running_) return;

    if (!initialized_) init();

    // start the server poll
    running_ = true;

    // if the mode is EPOLL
    // thread it and run the epoller_
    if (mode_ == EPOLL) {
        epollHandler_->getEpoller()->start();
    }

    int max = 0;
    int ready = -1;
    // entering polling loop
    while (running_) {
        ready = poll(pollfds_, max + 1, poll_timeout_);
        if (ready < 0) {
            my_error_exit("server poll error\n");
        }
        // handle the comming client socket
        if ((pollfds_[0].revents & POLLIN) == POLLIN) {
            // accept the socket and new Connection
            // pass it to handleConnection
            // maybe by process by thread or by epoll
            // or other user-defined ways
            struct sockaddr_in client;
            socklen_t slen = sizeof(client);
            memset(&client, 0, sizeof(client));
            int clientfd =
                accept(pollfds_[0].fd, (struct sockaddr *)&client, &slen);
            if (clientfd < 0) {
                my_error_exit("poll error : accept client error\n");
            }
            if (mode_ == EPOLL) {  // we need block for epoll mode
                makeNonblock(clientfd);
            }
            // pass the connection to ConnectionHandler
            // let the ConnectionHandler manage the life
            // span of Connection
            Connection *c = new Connection();
            c->setFd(clientfd);
            c->setState(CONNECTED);
            handleConnection(c);
        }
    }
}

// stop the server
// useful for other threads to stop this server
void AbstractServer::stop() {
    // stop myself
    running_ = false;

    // stop the epoller_ if in EPOLL mode
    if (mode_ == EPOLL) {
        epollHandler_->getEpoller()->stopLoop();
    }
}

// we use different operations according to mode
void AbstractServer::handleConnection(Connection *conn) {
    // multi-process mode
    if (mode_ == PROCESS) {
        processHandler_->handleConnection(conn);
    }
    // multi-thread mode
    if (mode_ == THREAD) {
        threadHandler_->handleConnection(conn);
    }
    // epoll mode
    if (mode_ == EPOLL) {
        epollHandler_->handleConnection(conn);
    }
    // handle Connection in the main poll server, for testing
    if (mode_ == DIRECT) {
        handleConnectionDirect(conn);
    }
}

void AbstractServer::handleConnectionDirect(Connection *conn) {}

// interface for user-specific server
void AbstractServer::parseRequest(Request *req) {}

// interface for user-specific server
void AbstractServer::transformResponse(Response *res) {}

void AbstractServer::setMode(MODE m) { mode_ = m; }

MODE AbstractServer::getMode() const { return mode_; }

// std::shared_ptr<ConnectionEpollHandler>
//                &AbstractServer::getEpollHandler()
ConnectionEpollHandler *AbstractServer::getEpollHandler() {
    return epollHandler_;
}

// std::shared_ptr<ConnectionThreadHandler>
//                &AbstractServer::getThreadHandler()
ConnectionThreadHandler *AbstractServer::getThreadHandler() {
    return threadHandler_;
}

// std::shared_ptr<ConnectionProcessHandler>
//                &AbstractServer::getProcessHandler()
ConnectionProcessHandler *AbstractServer::getProcessHandler() {
    return processHandler_;
}

bool AbstractServer::isKeepalived() const { return keepalived_; }

void AbstractServer::setKeepAlived(bool flag) { keepalived_ = flag; }

void AbstractServer::setPollTimeout(int n) { poll_timeout_ = n; }

void AbstractServer::parseConf(std::string path) {
    char *buf;
    int len;
    std::ifstream ifs(path.c_str(), std::ios::in);
    ifs.seekg(0, std::ios::end);
    len = ifs.tellg();
    ifs.seekg(0, std::ios::beg);
    buf = new char[sizeof(char) * len];
    if (!buf) {
        my_error_exit("allocate memory for buffer in parseConf failed\n");
    }
    ifs.read(buf, len);
    ifs.close();
    rapidjson::Document d;
    d.Parse(buf);
    delete[] buf;
    rapidjson::Value &v = d["host"];
    if (v.GetString()) {
        host_ = v.GetString();
    }
    v = d["port"];
    if (v.GetInt() > 0) {
        port_ = v.GetInt();
    }
    v = d["mode"];
    if (v.GetString()) {
        if (v.GetString() == std::string("thread")) {
            mode_ = EPOLL;
        }
    }
    v = d["keepalived"];
    if (v.GetBool()) {
        keepalived_ = true;
    }
    v = d["timeout"];
    if (v.GetInt() > 0) {
        timeout_ = v.GetInt();
    }
    v = d["max_connection"];
    if (v.GetInt() > 0) {
        max_connection_ = v.GetInt();
    }
    v = d["root_dir"];
    if (v.GetString()) {
        rootdir_ = v.GetString();
    }
    v = d["log_dir"];
    if (v.GetString()) {
        logdir_ = rootdir_ + v.GetString();
    }
}

void AbstractServer::printConf() const {
    std::cout << "==== server configuration start ====" << std::endl;
    std::cout << "host: " << host_ << std::endl << "port: " << port_
              << std::endl << "mode: " << (mode_ == EPOLL ? "EPOLL" : "THREAD")
              << std::endl
              << "keepalived: " << (keepalived_ == true ? "true" : "false")
              << std::endl << "timeout: " << timeout_ << std::endl
              << "max_connection: " << max_connection_ << std::endl
              << "rootdir: " << rootdir_ << std::endl << "logdir: " << logdir_
              << std::endl;
    std::cout << "==== server configuration end ====" << std::endl;
}

}  // namespace Tan
