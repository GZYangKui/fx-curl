//
// Created by yangkui on 2021/11/16.
//

#ifndef FXCURL_NAV_NOTEBOOK_EVENT_H
#define FXCURL_NAV_NOTEBOOK_EVENT_H

#include "common.h"

/**
 *
 * init nav notebook
 *
 */
extern void fx_inti_nav_notebook(GtkBuilder *builder);

/**
 *
 *
 * 根据{@code GtkTreeView}中传递过来的参数创建Note页
 *
 */
extern void create_note_page(gchararray name, gint64 id, NodeTreeType type);

#endif //FXCURL_NAV_NOTEBOOK_EVENT_H
