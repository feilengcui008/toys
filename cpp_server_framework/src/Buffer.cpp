// Copyright [2016] <feilengcui008@gmail.com>
#include "Buffer.h"
#include <cstring>
#include <string>

namespace Tan {

Buffer::Buffer(Buffer &&b) {
    size_ = b.size_;
    len_ = b.len_;
    buf_ = b.buf_;
    b.buf_ = nullptr;
    position_ = buf_;
}

Buffer::Buffer(const Buffer &b) {
    size_ = b.size_;
    len_ = b.len_;
    buf_ = new char(size_);
    position_ = buf_;
    strncpy(buf_, b.buf_, size_);
}

Buffer &Buffer::operator=(const Buffer &b) {
    if (&b != this) {
        size_ = b.size_;
        len_ = b.len_;
        buf_ = new char(size_);
        position_ = buf_;
        strncpy(buf_, b.buf_, size_);
    }
    return *this;
}

Buffer::~Buffer() {
    if (buf_ != nullptr) {
        delete buf_;
        buf_ = nullptr;
    }
}

char *Buffer::getBuf() { return buf_; }

void Buffer::appendBuf(char *b, int len) {
    if (len > (size_ - len_)) {
        // todo resize the buf_ and fill it
    } else {
        strncpy(position_, b, len);
        position_ += len;
        len_ += len;
    }
}

void Buffer::cleanBuf() {
    memset(buf_, 0, size_);
    position_ = buf_;
    len_ = 0;
}

char Buffer::getOneByte() {
    char c = *position_;
    position_++;
    return c;
}

std::string Buffer::getOneLine() {
    std::string temp{""};
    while (*position_ != '\n') {
        temp.push_back(*position_);
        position_++;
    }
    return temp;
}

void Buffer::expandTo(int size) {
    if (size < size_) {
        return;
    } else {
        // todo
        char *n = new char(size);
        strncpy(n, buf_, len_);
        delete buf_;
        buf_ = n;
    }
}

void Buffer::expandBy(int size) {
    int n = size + size_;
    expandTo(n);
}
}  // namespace Tan
