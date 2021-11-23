//
// Created by yangkui on 2021/11/17.
//

#ifndef FXCURL_COMMON_H
#define FXCURL_COMMON_H

#include <gtk/gtk.h>
#include <stdlib.h>
#include <string.h>
#include "fx_config.h"

typedef enum {
    //文件夹
    FOLDER,
    //api接口
    API,
    //保留字段
    ALL
} NodeTreeType;


typedef struct {
    //状态
    gboolean status;
    //消息
    gchararray message;
    //待更新数据
    gpointer data;
} UpdateUIRequest;

/**
 * 定义宏释放{@struct UpdateUIRequest},注意该宏不会释放data指针指向的内存,调用者自行释放.
 */
#define FREE_UPDATE_UI_REQUEST(request) \
   if((request)->message!=NULL) {         \
     FX_FREE((request)->message);       \
   }                                    \
    FX_FREE(request);                   \


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


extern UpdateUIRequest *fx_update_ui_request_new(gboolean status, gchararray msg, gpointer data);

extern void set_window_default_icons(GtkWidget *window);

/**
 *
 *
 * 打开输入对话框并将输入结果赋值到{@code buff}参数上
 * @param title  对话框标题
 * @param maxLen 允许最大输入长度
 * @param buff 接受输入数据指正
 * @return 返回真实输入长度
 *
 */
extern guint64 open_input_dialog(const gchar *title, GtkWindow *parent, guint maxLen, gchar *buff);

/**
 *
 * 显示一个错误对话框
 *
 */
extern void show_error_dialog(gchararray title, gchararray context);

/**
 *
 * 添加指定css样式脚本到指定widget上
 *
 */
extern void add_style_sheet_to_widget(GtkWidget *target, gchararray stylesheets, gint prior);


/**
 * 获取1970到当前时间的毫秒数
 */
extern gint64 fx_get_timestamp();

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
