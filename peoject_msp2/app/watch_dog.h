#ifndef __WATCH_DOG_H
#define __WATCH_DOG_H
#include <main.h>


typedef struct
{
  void (*vWatchDog_Init)(void);   // ���Ź���ʼ��
  void (*vWatchDog_Feed)(void); // ι��
}WatchDog_t;


extern WatchDog_t WatchDog;

#endif
