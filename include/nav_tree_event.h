//
// Created by yangkui on 2021/11/16.
//

#ifndef FXCURL_NAV_TREE_EVENT_H
#define FXCURL_NAV_TREE_EVENT_H

#include <gtk/gtk.h>



typedef enum {
    //api sets or folder
    FOLDER,
    //api
    API,

    ALL
} ColType;

typedef struct {
    gchar *name;
    ColType colType;
} MenuItemMeta;

typedef enum {
    /**
     * 图标列
     */
    ICON_COLUMN,
    /**
     * 标题列
     */
    TEXT_COLUMN,
    /**
     *column type
     */
    COL_TYPE,
    /**
     * 总列数
     */
    NUM_COLS
} TreeViewMeta;

/**
 *
 * init nav tree view
 *
 */
extern void fx_init_nav_tree(GtkBuilder *builder);

/**
 * create a empty folder
 */
extern void fx_dy_dir(GtkButton *button,gpointer userData);

/**
 *
 * 注册导航树节点点击事件
 *@return 返回事件是否被消费，该返回值决定了事件是否向上传播
 */
extern gboolean fx_nav_tree_click(GtkWidget *treeView, GdkEventButton *event, gpointer *userData);

#endif //FXCURL_NAV_TREE_EVENT_H
