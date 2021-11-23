//
// Created by yangkui on 2021/11/16.
//

#ifndef FXCURL_NAV_TREE_EVENT_H
#define FXCURL_NAV_TREE_EVENT_H

#include "common.h"


typedef struct {
    gchar *name;
    NodeTreeType colType;
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
     * column id
     */
    COL_ID,
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

#endif //FXCURL_NAV_TREE_EVENT_H
