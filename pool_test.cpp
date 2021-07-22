#include "ThreadPool.h"
#include <iostream>

using namespace std;
using namespace utils;

class A {
public:
  void test_task(int para) {
    // std::cout << "start task; para=" << para << std::endl;
  }
};

int main() {
  A a;
  ThreadPool tp("Test_ThreadPool", 1000);
  this_thread::sleep_for(chrono::milliseconds(1));
  tp.AddTask(&A::test_task, &a, 100);

  std::cout << "thread num1: " << tp.GetThreadCount()
            << "  free:" << tp.GetFreeThreads() << std::endl;
  bool bStop = false;
  for (int i = 0; i < 20; i++) {
    tp.AddTask([&bStop]() {
      while (!bStop) {
        this_thread::sleep_for(chrono::milliseconds(1));
      }
    });
  }

  this_thread::sleep_for(chrono::milliseconds(10));
  std::cout << "thread num2: " << tp.GetThreadCount()
            << "  free:" << tp.GetFreeThreads() << std::endl;
  bStop = true;
  this_thread::sleep_for(chrono::milliseconds(1000));
  std::cout << "thread num3: " << tp.GetThreadCount()
            << "   CurrId: " << tp.GetCurrId()
            << "  free:" << tp.GetFreeThreads() << std::endl;

  bStop = false;
  for (int i = 0; i < 20; i++) {
    tp.AddTask([&bStop]() {
      while (!bStop) {
        this_thread::sleep_for(chrono::milliseconds(1));
      }
    });
  }

  this_thread::sleep_for(chrono::milliseconds(10));
  std::cout << "thread num4: " << tp.GetThreadCount()
            << "  free:" << tp.GetFreeThreads() << std::endl;
  bStop = true;

  this_thread::sleep_for(chrono::milliseconds(1000));
  std::cout << "thread num5: " << tp.GetThreadCount()
            << "   CurrId: " << tp.GetCurrId()
            << "  free:" << tp.GetFreeThreads() << std::endl;

  return 0;
}
