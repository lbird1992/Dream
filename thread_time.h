#ifndef _DREAM_THREAD_TIME_
#define _DREAM_THREAD_TIME_

#include <queue>
#include "thread_lock.h"

enum TimeThreadMessageType {
  TTMT_EXIT = 0,
};

struct TimeThreadMessage {
  TimeThreadMessageType type;
  void* parameter;
};

extern std::queue<TimeThreadMessage> g_time_thread_message;

unsigned __stdcall TimeThreadFunction( void* time_parameter);

#endif