//
// Created by yangkui on 2021/11/20.
//

#ifndef FXCURL_UTIL_H
#define FXCURL_UTIL_H

#include <gtk/gtk.h>
#include <string.h>
/**
 * 利用宏定义减少字符串拷贝魔板代码书写
 */
#define SIMPLE_CPY_STR(src)   \
    gint size = strlen(src)+1;\
    gchar desStr[size];       \
    memset(desStr,0,size);    \
    strcpy(desStr,src);       \

#endif //FXCURL_UTIL_H
