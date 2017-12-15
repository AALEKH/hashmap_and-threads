#ifndef TEST_THREAD_H_
#define TEST_THREAD_H_

#include <cinttypes>
#include <thread>
#include <iostream>
#include <map>
#include <array>
#include <string>
#include <vector>
#include <chrono>
#include <ctime>

class WriteBuffer {
 public:

  std::thread thread_buffer_handler_;

  WriteBuffer() {

    stop_requested_ = false;
    im_live_ = 0;
    im_copy_ = 1;
    sizes_[im_live_] = 0;
    sizes_[im_copy_] = 0;
    thread_buffer_handler_ = std::thread(&WriteBuffer::ProcessingLoop, this);

  }
  ~WriteBuffer() { Close(); }

  void Flush();

  void Close () {
    std::unique_lock<std::mutex> lock(mutex_close_);
    if (is_closed_) return;
    is_closed_ = true;
    Stop();
    Flush();
    cv_flush_.notify_one();
    thread_buffer_handler_.join();
  }

  bool IsStopRequestedAndBufferEmpty() {
    return (   IsStopRequested()
            && buffers_[im_live_].empty()
            && buffers_[im_copy_].empty());
  }
  bool IsStopRequested() { return stop_requested_; }
  void Stop() { stop_requested_ = true; }
  bool stop_requested_;

  void WritePart(int);
  void ProcessingLoop();
  
 private:

  int im_live_;
  int im_copy_;
  uint64_t buffer_size_;
  int num_readers_;
  std::array<std::vector<int>, 2> buffers_;
  std::array<int, 2> sizes_;
  bool is_closed_;
  std::mutex mutex_close_;

  // Using a lock hierarchy to avoid deadlocks
  std::mutex mutex_live_write_level1_;
  std::mutex mutex_flush_level2_;
  std::mutex mutex_indices_level3_;
  std::mutex mutex_copy_write_level4_;
  std::mutex mutex_copy_read_level5_;
  std::condition_variable cv_flush_;
  std::condition_variable cv_flush_done_;
  std::condition_variable cv_read_;
};

#endif 