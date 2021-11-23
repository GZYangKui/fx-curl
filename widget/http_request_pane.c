//
// Created by yangkui on 2021/11/23.
//


#include "../include/http_request_pane.h"

typedef enum {
    KEY,
    VALUE,
    DESC,
    COL_NUM
} RequestTreeColumn;

typedef enum {
    //request params
    PARAMS,
    //request headers
    HEADERS
} RequestTreeType;

static GtkWidget *internal_init_tree_view(GtkBuilder *, RequestTreeType);

extern GtkWidget *http_request_pane_new(gint64 id) {
    gchararray path = GET_INNER_UI_RESOURCE(widget/HttpReqPane.ui);
    GtkBuilder *builder = gtk_builder_new_from_resource(path);

    GtkWidget *pTV = internal_init_tree_view(builder, PARAMS);
    GtkWidget *hTV = internal_init_tree_view(builder, HEADERS);

    GtkWidget *pane = GTK_WIDGET(gtk_builder_get_object(builder, "httpReqPane"));


    return pane;
}

static GtkWidget *internal_init_tree_view(GtkBuilder *builder, RequestTreeType type) {
    gchararray treeId;
    if (type == PARAMS) {
        treeId = "pTV";
    } else {
        treeId = "hTV";
    }
    GtkWidget *treeView = GTK_WIDGET(gtk_builder_get_object(builder, treeId));


    GtkTreeModel *pTM = GTK_TREE_MODEL(gtk_tree_store_new(COL_NUM,
                                                          G_TYPE_STRING,
                                                          G_TYPE_STRING,
                                                          G_TYPE_STRING)
    );

    GtkTreeViewColumn *key = GTK_TREE_VIEW_COLUMN(gtk_builder_get_object(builder, type == PARAMS ? "pKey" : "hKey"));
    GtkTreeViewColumn *val = GTK_TREE_VIEW_COLUMN(gtk_builder_get_object(builder, type == PARAMS ? "pVal" : "hVal"));
    GtkTreeViewColumn *dst = GTK_TREE_VIEW_COLUMN(gtk_builder_get_object(builder, type == PARAMS ? "pDst" : "hDst"));

    GtkCellRenderer *pKeyRender = gtk_cell_renderer_text_new();
    GtkCellRenderer *pValRender = gtk_cell_renderer_text_new();
    GtkCellRenderer *pDstRender = gtk_cell_renderer_text_new();

    gtk_tree_view_column_pack_start(key, pKeyRender, FALSE);
    gtk_tree_view_column_pack_start(val, pValRender, FALSE);
    gtk_tree_view_column_pack_start(dst, pDstRender, FALSE);


    gtk_tree_view_column_add_attribute(key, pKeyRender, "text", KEY);
    gtk_tree_view_column_add_attribute(val, pValRender, "text", VALUE);
    gtk_tree_view_column_add_attribute(dst, pDstRender, "text", DESC);


    gtk_tree_view_set_model(GTK_TREE_VIEW(treeView), pTM);

    gtk_widget_show_all(treeView);

    g_object_unref(pTM);

    return treeView;
}


