#ifndef __WATCH_DOG_H
#define __WATCH_DOG_H
#include <main.h>


typedef struct
{
  void (*vWatchDog_Init)(void);   // 看门狗初始化
  void (*vWatchDog_Feed)(void); // 喂狗
}WatchDog_t;


extern WatchDog_t WatchDog;

#endif
