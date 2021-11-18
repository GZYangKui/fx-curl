//
// Created by yangkui on 2021/11/17.
//

#ifndef FXCURL_DATABASE_H
#define FXCURL_DATABASE_H

#include <gtk/gtk.h>

/**
 *
 * 初始化数据库连接
 *
 */
extern gpointer fx_init_sqlite(gpointer userData);

/**
 *
 * 停止sqlite3数据库
 *
 */
extern void fx_shutdown_sqlite3();

#endif //FXCURL_DATABASE_H
