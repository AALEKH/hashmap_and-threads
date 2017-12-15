#include "test_thread.h"


void WriteBuffer::Flush() {
  std::unique_lock<std::mutex> lock_flush(mutex_flush_level2_);
  if (IsStopRequestedAndBufferEmpty()) return;

  for (auto i = 0; i < 2; i++) {

    cv_flush_.notify_one();
    cv_flush_done_.wait_for(lock_flush, std::chrono::milliseconds(1000));

  }

}

void WriteBuffer::WritePart(int write_part) {


  std::unique_lock<std::mutex> lock_live(mutex_live_write_level1_);
  mutex_indices_level3_.lock();

  buffers_[im_live_].push_back(write_part);
  std::cout << "Im Live index is: " << im_live_ << std::endl;
  std::cout << "Buffer O size is: " << buffers_[0].size() << std::endl;
  std::cout << "Buffer 1 Size is: " << buffers_[1].size() << std::endl;

  sizes_[im_live_] += 1;
  

  uint64_t size_buffer_live = sizes_[im_live_];
  mutex_indices_level3_.unlock();



  if (size_buffer_live > 5) {

    mutex_flush_level2_.lock();

    std::unique_lock<std::mutex> lock_swap(mutex_indices_level3_);
    cv_flush_.notify_one();

    mutex_flush_level2_.unlock();


  } else {

  }

}


void WriteBuffer::ProcessingLoop() {
  while(true) {
    // bool force_sync = false;

    std::unique_lock<std::mutex> lock_flush(mutex_flush_level2_);
    while (sizes_[im_live_] == 0) {

      std::cv_status status = cv_flush_.wait_for(lock_flush, std::chrono::milliseconds(1000));
      
      if (IsStopRequestedAndBufferEmpty()) return;

      if (status == std::cv_status::no_timeout) {
      
        // force_sync = true; 
      	std::cout << "No Timeout print's out" << std::endl;
      }
    
    }

    mutex_indices_level3_.lock();

    if (sizes_[im_copy_] == 0) {
      std::swap(im_live_, im_copy_);
    }
    
    mutex_indices_level3_.unlock();


    mutex_copy_write_level4_.lock();
    while(true) {

      std::unique_lock<std::mutex> lock_read(mutex_copy_read_level5_);

      cv_read_.wait(lock_read);
    }

    sizes_[im_copy_] = 0;
    buffers_[im_copy_].clear();


    mutex_copy_write_level4_.unlock();

    cv_flush_done_.notify_all();

    if (IsStopRequestedAndBufferEmpty()) return;
  }
}

// int main() {
// 	return 0;
// }