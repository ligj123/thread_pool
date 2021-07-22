#include "ThreadPool.h"
#include <iostream>

using namespace std;
using namespace utils;

int main() {
  ThreadPool tp("Test_ThreadPool", 1000);
  std::cout << "thread num1: " << tp.GetThreadCount() << std::endl;
  bool bStop = false;
  for (int i = 0; i < 20; i++) {
    tp.AddTask([&bStop]() {
      while (!bStop) {
        this_thread::sleep_for(chrono::milliseconds(1));
      }
    });
  }

  this_thread::sleep_for(chrono::milliseconds(10));
  std::cout << "thread num2: " << tp.GetThreadCount() << std::endl;
  bStop = true;
  this_thread::sleep_for(chrono::milliseconds(1000));
  std::cout << "thread num3: " << tp.GetThreadCount()
            << "   CurrId: " << tp.GetCurrId() << std::endl;

  bStop = false;
  for (int i = 0; i < 20; i++) {
    tp.AddTask([&bStop]() {
      while (!bStop) {
        this_thread::sleep_for(chrono::milliseconds(1));
      }
    });
  }

  this_thread::sleep_for(chrono::milliseconds(10));
  std::cout << "thread num4: " << tp.GetThreadCount() << std::endl;
  bStop = true;

  this_thread::sleep_for(chrono::milliseconds(1000));
  std::cout << "thread num5: " << tp.GetThreadCount()
            << "   CurrId: " << tp.GetCurrId() << std::endl;

  return 0;
}
