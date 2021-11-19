//
// Created by yangkui on 2021/11/17.
//

#ifndef FXCURL_COMMON_H
#define FXCURL_COMMON_H

#include <gtk/gtk.h>
#include <stdlib.h>
#include <string.h>

#define FX_RESOURCE_PREFIX "/cn/navclub/"
#define FX_RESOURCE_UI_PREFIX "/cn/navclub/ui/"
#define FX_RESOURCE_IMG_PREFIX "/cn/navclub/img/"
#define FX_RESOURCE_CSS_PREFIX "/cn/navclub/css/"

/**
 * 通过宏拼接资源前缀
 */
#define GET_INNER_RESOURCE_PATH(resource) \
      FX_RESOURCE_PREFIX#resource

#define GET_INNER_UI_RESOURCE(resource) \
    FX_RESOURCE_UI_PREFIX#resource

#define GET_INNER_IMG_RESOURCE(resource) \
     FX_RESOURCE_IMG_PREFIX#resource

#define GET_INNER_CSS_RESOURCE(resource) \
     FX_RESOURCE_CSS_PREFIX#resource

typedef struct {
    GtkWidget *window;
    GAsyncQueue *asyncQueue;
} FXWindowContext;

//队列消息成功状态
#define QUEUE_MSG_OK 1
//队列消息失败
#define QUEUE_MSG_FAIL 0

/**
 *
 * 异步消息队列实体结构
 *
 */
typedef struct {
    /**
     * 状态码
     */
    gint code;
    /**
     * 消息
     */
    gchararray message;
    /**
     * 队列数据
     */
    gpointer data;
} QueuePayload;

#define ALLOC_QUEUE_PAYLOAD (QueuePayload*) malloc(sizeof(QueuePayload));

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
