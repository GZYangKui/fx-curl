//
// Created by yangkui on 2021/11/17.
//

#ifndef FXCURL_COMMON_H
#define FXCURL_COMMON_H

#include <gtk/gtk.h>
#include <stdlib.h>
#include <string.h>
#include "util.h"
#include "fx_config.h"

/**
 * 将目标字符串转换为堆字符串
 */
#define TRA_DUMP_STR(str) \
 gchararray message = (gchararray)malloc(strlen(str)+1); \
 strcpy(message,str);


/**
 * 释放堆内存并置NULL值
 */
#define FX_FREE(ptr) \
   free(ptr);        \
   ptr = NULL;

#endif //FXCURL_COMMON_H
