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
 * 将栈上字符串复制到堆上
 */
extern gchararray fx_trf_stack_to_dump(gchararray src);

/**
 * 释放堆内存并置NULL值
 */
#define FX_FREE(ptr) \
   free(ptr);        \
   ptr = NULL;

#endif //FXCURL_COMMON_H
