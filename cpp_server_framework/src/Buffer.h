// Copyright [2016] <feilengcui008@gmail.com>
#ifndef SRC_BUFFER_H_
#define SRC_BUFFER_H_

#include <string>

namespace Tan {
class Buffer {
 public:
  explicit Buffer(int size = 4096) : size_(size), len_(0) {
    buf_ = new char(size_);
    position_ = buf_;
  }
  Buffer(Buffer &&b);
  Buffer(const Buffer &b);
  Buffer &operator=(const Buffer &b);
  ~Buffer();

  char *getBuf();
  void appendBuf(char *buf, int len);
  void cleanBuf();
  void expandTo(int size);
  void expandBy(int size);
  char getOneByte();
  // maybe
  std::string getOneLine();

 private:
  int size_;
  int len_;
  char *buf_;
  char *position_;
};

}  // namespace Tan

#endif  // SRC_BUFFER_H_
