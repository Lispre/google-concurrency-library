// Copyright 2011 Google Inc. All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef GCL_BARRIER_
#define GCL_BARRIER_

#include <stddef.h>
#include <stdexcept>

#include <atomic>
#include <condition_variable>
#include <mutex>
#include <functional>

namespace gcl {

// Allows a set of threads to wait until all threads have reached a
// common point.
class barrier {
 public:
  explicit barrier(std::ptrdiff_t num_threads);

  ~barrier();

  barrier(const barrier&) = delete;
  barrier& operator=(const barrier&) = delete;

  void arrive_and_wait();

  void arrive_and_drop();

 private:
  void check_all_threads_exited();
  bool all_threads_exited();
  bool all_threads_waiting();


  std::mutex mutex_;
  std::condition_variable idle_;
  std::condition_variable ready_;
  std::ptrdiff_t thread_count_;
  std::ptrdiff_t num_waiting_;
  std::atomic<std::ptrdiff_t> num_to_leave_;
};
}
#endif // GCL_BARRIER_
