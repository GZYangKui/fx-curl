//
// Created by yangkui on 2021/11/17.
//

#ifndef FXCURL_DATABASE_H
#define FXCURL_DATABASE_H

#include <gtk/gtk.h>

typedef struct {
    gint id;
    gint16 type;
    gint parentId;
    gchararray name;
    gchararray createTime;
} NodeTree;


/**
 *
 * 初始化数据库连接
 *
 */
extern gboolean fx_init_sqlite(gpointer userData, GError **error);

/**
 *
 * 停止sqlite3数据库
 *
 */
extern void fx_shutdown_sqlite3();

#endif //FXCURL_DATABASE_H
