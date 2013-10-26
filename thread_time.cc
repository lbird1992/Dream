#include "thread_time.h"
#include <stdint.h>

std::queue<TimeThreadMessage> g_time_thread_message;

unsigned __stdcall TimeThreadFunction(void* time_parameter) {
  uint64_t* time = reinterpret_cast<uint64_t*>(time_parameter);
  DWORD tick1 = GetTickCount();
  DWORD tick = 0;
  //time_parameter = time + sizeof(time);
  while( 1) {
    while( g_time_thread_message.empty() ) {
      Sleep( 100);
      DWORD tick2 = GetTickCount();
      tick += tick2 - tick1;
      tick1 = tick2;
      if( tick >= 1000) {
        tick -= 1000;
        (*time)++;
      }
    }
    g_thread_lock_time.Lock();
    TimeThreadMessage time_thread_message = g_time_thread_message.front();
    g_time_thread_message.pop();
    g_thread_lock_time.Unlock();
    if( time_thread_message.type == TTMT_EXIT)
      break;
  }
  return 0;
}