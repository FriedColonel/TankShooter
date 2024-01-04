#include <socket/ThreadPool.hpp>

TSS::ThreadPool::ThreadPool(int num_threads) {
  this->num_threads = num_threads;
  this->active = 1;
  this->work = std::queue<ThreadJob>();
  this->lock = PTHREAD_MUTEX_INITIALIZER;
  this->signal = PTHREAD_COND_INITIALIZER;
  pthread_mutex_lock(&this->lock);
  this->pool = new pthread_t[num_threads];
  for (int i = 0; i < num_threads; i++) {
    pthread_create(&this->pool[i], NULL, &generic_thread_function, this);
  }
  pthread_mutex_unlock(&this->lock);
}

TSS::ThreadPool::~ThreadPool() {
  active = 0;
  for (int i = 0; i < num_threads; i++) {
    pthread_cond_signal(&signal);
  }
  for (int i = 0; i < num_threads; i++) {
    pthread_join(pool[i], NULL);
  }
  delete[] pool;
  std::queue<ThreadJob> empty;
  std::swap(work, empty);
}

TSS::ThreadJob::ThreadJob(void *(*jobFunc)(void *), void *arg) {
  this->job = jobFunc;
  this->arg = arg;
}

void TSS::ThreadPool::add_work(ThreadJob thread_job) {
  pthread_mutex_lock(&lock);
  work.push(thread_job);
  pthread_cond_signal(&signal);
  pthread_mutex_unlock(&lock);
}

void *TSS::ThreadPool::generic_thread_function(void *arg) {
  ThreadPool *thread_pool = (ThreadPool *)arg;

  while (thread_pool->active == 1) {
    // Lock the work queue.
    pthread_mutex_lock(&thread_pool->lock);
    // Wait for work to be available or for the thread pool to be inactive.
    while (thread_pool->work.empty() && thread_pool->active) {
      pthread_cond_wait(&thread_pool->signal, &thread_pool->lock);
    }

    // Get the job from the queue.
    if (!thread_pool->work.empty()) {
      ThreadJob thread_job = thread_pool->work.front();
      // Unlock the queue.
      thread_pool->work.pop();
      pthread_mutex_unlock(&thread_pool->lock);
      // Execute the job.
      if (thread_job.job) {
        thread_job.job(thread_job.arg);
      }
    } else {
      pthread_mutex_unlock(&thread_pool->lock);
    }
  }

  // std::cout << "Threadd active " << thread_pool->work.size() << std::endl;
  return NULL;
}