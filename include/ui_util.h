//
// Created by yangkui on 2021/11/17.
//

#ifndef FXCURL_UI_UTIL_H
#define FXCURL_UI_UTIL_H

#include <gtk/gtk.h>

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
extern guint64 open_input_dialog(const gchar *title,GtkWindow *parent,guint maxLen,gchar *buff);

#endif //FXCURL_UI_UTIL_H
