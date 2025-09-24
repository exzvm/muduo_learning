#pragma once

#include <vector>
#include <string>
#include <algorithm>

class Buffer
{
public:
  static const size_t kCheapPrepend = 8;
  static const size_t kInitialSize = 1024;

  explicit Buffer(size_t initialSize = kInitialSize)
    : buffer_(kCheapPrepend * kInitialSize)
    , readerIndex_(kCheapPrepend)
    , writerIndex_(kCheapPrepend)
    {}

  size_t readableBytes() const {
    return writerIndex_ - readerIndex_;
  }

  size_t writableBytes() const {
    return buffer_.size() - writerIndex_;
  }

  size_t prependableBytes() const {
    return readerIndex_;
  }

  void ensureWritableBytes(size_t len) {
    if (writableBytes() < len) {
      makeSpace(len); // 扩容函数
    }
  }

  // [data, data+len]内存上的数据，添加到writeable缓冲区
  void append(const char *data, size_t len) {
    ensureWritableBytes(len);
    std::copy(data, data + len, beginWrite());
    writerIndex_ += len;
  }

  char* beginWrite() {
    return begin() + writerIndex_;
  }

  const char* beginWrite() const {
    return begin() + writerIndex_;
  }

  // 从fd上读取数据
  ssize_t readFd(int fd, int *saveErrno);
  // 通过fd发送数据
  ssize_t writeFd(int fd, int *saveErrno);

    // onMessage  string <- Buffer
  void retrieve(size_t len) {
    if (len < readableBytes()) {
      readerIndex_ += len; // 只读取了一部分(len长度)，还剩下  readerIndex_ + len => writerIndex_ 没读
    } else {
      retrieveAll();
    }
  }

  // 把onMessage函数上报的Buffer数据，转成string类型的数据返回
  std::string retrieveAllAsString() {
    return retrieveAsString(readableBytes()); // 应用可读取数据长度
  }

  void retrieveAll() {
    readerIndex_ = kCheapPrepend;
    writerIndex_ = kCheapPrepend;
  }

  std::string retrieveAsString(size_t len) {
    std::string result(peek(), len);
    retrieve(len); // 对缓冲区进行复位操作
    return result;
  }

  // 返回缓冲区中可读数据的起始地址
  const char* peek() const {
    return begin() + readerIndex_;
  }
private:
  char* begin() {
    // it.operator*().operator&()
    return &*buffer_.begin();  // vector底层数组首元素的地址，也就是数组的起始地址
  }

  const char* begin() const {
    return &*buffer_.begin();
  }

  /**
   * 扩容函数
   */
  void makeSpace(size_t len) {
    if (writableBytes() + prependableBytes() < len + kCheapPrepend) {
      buffer_.resize(writerIndex_ + len);
    } else {
      size_t readable = readableBytes();
      std::copy(begin() + readerIndex_,
                begin() + writerIndex_, 
                begin() + kCheapPrepend);
      readerIndex_ = kCheapPrepend;
      writerIndex_ = readerIndex_ + readable;
    }
  }

  std::vector<char> buffer_;
  size_t readerIndex_;
  size_t writerIndex_;

};