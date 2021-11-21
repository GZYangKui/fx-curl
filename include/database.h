//
// Created by yangkui on 2021/11/17.
//

#ifndef FXCURL_DATABASE_H
#define FXCURL_DATABASE_H

#include <gtk/gtk.h>

typedef struct {
    gint id;
    gint type;
    gint parentId;
    gchararray name;
    gint64 createTime;
} NodeTree;

#define FX_FREE_TREE_NODE(treeNode, freeObj) \
    if((treeNode)->name!=NULL){              \
       FX_FREE((treeNode)->name);            \
    }                                        \
    if(freeObj){                             \
       FX_FREE(treeNode);                    \
    }


/**
 *
 * 初始化数据库连接
 *
 */
extern gboolean fx_init_sqlite(gpointer userData, GError **error);

/**
 *
 * 根据父级节点id查询节点
 *
 */
extern gboolean select_node_by_parent_id(gint id, GList *list);

/**
 *
 * 插入一条新记录
 *
 */
extern gboolean insert_tree_node(gint *id, gint parentId, gint type, gchararray name);

/**
 *
 * 停止sqlite3数据库
 *
 */
extern void fx_shutdown_sqlite3();

#endif //FXCURL_DATABASE_H
