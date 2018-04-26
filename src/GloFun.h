/**************************
*    全局函数
*  实现一些通用的函数
*   
*   2017/12/29 by cjf[xmu]
***************************/
#include<sys/time.h>

#ifndef _GG_
#define _GG_
#define UseFillWhiteHole
//#define DEBUG
//defination
inline long long g_gettime();   //读取毫秒时间
static long long g_time_start=-1;
static long long g_time_end;
#ifdef DEBUG
#define   TIME_START    if(g_time_start!=-1)printf("[!!]未处理的时间节点]\n");g_time_start=g_gettime();
#define   TIME_SHOW  printf("[*时间]%lld ms\n",g_gettime()-g_time_start);
#define   SHOW_TIME(s)  printf("[%s时间]%lld ms\n",s,g_gettime()-g_time_start);g_time_start=-1
#else
#define TIME_START
#define TIME_SHOW
#define SHOW_TIME(s)
#endif
inline long long g_gettime(){
    struct timeval t;
    gettimeofday( &t, 0 );
    return 1000ll*t.tv_sec+t.tv_usec/1000;
}

#endif
//implementation
