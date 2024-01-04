#ifndef ThreadPool_hpp
#define ThreadPool_hpp

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#include <iostream>
#include <queue>

namespace TSS {
class ThreadJob {
 public:
  void* (*job)(void* arg);
  void* arg;

  ThreadJob(void* (*jobFunc)(void*), void* arg);
};

class ThreadPool {
 public:
  static void* generic_thread_function(void* arg);

  // The number of threads in the pool.
  int num_threads;
  // A control switch for the thread pool.
  int active;
  // A queue to store work.
  std::queue<ThreadJob> work;
  // Mutices for making the pool thread-safe.
  pthread_t* pool;
  pthread_mutex_t lock;
  pthread_cond_t signal;

  ThreadPool(int num_threads);
  ~ThreadPool();

  // A function for safely adding work to the queue.
  void add_work(ThreadJob thread_job);
};
}  // namespace TSS

#endif
