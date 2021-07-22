#include "ThreadPool.h"

namespace utils {
thread_local string ThreadPool::_threadName = "main";

ThreadPool::ThreadPool(string threadPrefix, uint32_t maxQueueSize,
                       int minThreads, int maxThreads)
    : _threadPrefix(threadPrefix), _maxQueueSize(maxQueueSize),
      _stopThreads(false), _minThreads(minThreads), _maxThreads(maxThreads),
      _currId(0) {
  if (_minThreads < 1 || _minThreads > _maxThreads || _maxThreads < 1) {
    throw exception("Please set min threads and max thread in right range!");
  }
  for (int i = 0; i < minThreads; ++i) {
    CreateThread(++_currId);
  }
}

void ThreadPool::CreateThread(int id) {
  thread *t = new thread([this, id]() {
    std::unique_lock<std::mutex> queue_lock(_task_mutex, std::defer_lock);
    _threadName = _threadPrefix + "_" + to_string(id);

    while (true) {
      queue_lock.lock();
      _taskCv.wait_for(queue_lock, 500ms, [&]() -> bool {
        return !_tasks.empty() || _stopThreads;
      });

      if (_tasks.empty()) {
        queue_lock.unlock();
        std::unique_lock<std::mutex> thread_lock(_threadMutex);
        if (_mapThread.size() > _minThreads || _stopThreads) {
          break;
        } else {
          continue;
        }
      }

      auto temp_task = std::move(_tasks.front());
      _tasks.pop();
      queue_lock.unlock();

      (*temp_task)();
    }

    std::unique_lock<std::mutex> thread_lock(_threadMutex);

    if (!_stopThreads) {
      thread *t = _mapThread[id];
      _mapThread.erase(id);
      t->detach();
      delete t;
    }
  });

  std::unique_lock<std::mutex> thread_lock(_threadMutex);
  _mapThread.insert({id, t});
}

ThreadPool::~ThreadPool() {
  {
    std::unique_lock<std::mutex> thread_lock(_threadMutex);
    _stopThreads = true;
  }
  _taskCv.notify_all();

  for (auto iter = _mapThread.begin(); iter != _mapThread.end(); iter++) {
    iter->second->join();
    delete iter->second;
  }

  _mapThread.clear();
}
} // namespace utils
