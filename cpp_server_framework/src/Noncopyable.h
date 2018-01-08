#ifndef Noncopyable_H
#define Noncopyable_H

namespace Tan {
class Noncopyable {
 protected:
  Noncopyable() = default;
  ~Noncopyable() = default;
  // Noncopyable(const Noncopyable &) = delete;
  // Noncopyable(Noncopyable &&) = delete;
  // Noncopyable &operator=(const Noncopyable &) = delete;
 private:
  Noncopyable(const Noncopyable &);
  Noncopyable(Noncopyable &&);
  Noncopyable &operator=(const Noncopyable &);
};
}
#endif
