//
// Created by yangkui on 2021/11/16.
//

#include "../include/nav_notebook_event.h"
#include "../include/http_request_pane.h"

static GtkWidget *notebook;
//使用hash表存储TreeNode与page映射关系
static GHashTable *hashTable;


static GtkWidget *fx_create_tab_label(gchararray name, gint64 id, NodeTreeType type);

extern void fx_inti_nav_notebook(GtkBuilder *builder) {
    GObject *_notebook;
    _notebook = gtk_builder_get_object(builder, "notebook");

    notebook = GTK_WIDGET(_notebook);

    hashTable = g_hash_table_new(g_int64_hash, g_int64_equal);

    gtk_notebook_set_show_tabs(GTK_NOTEBOOK(notebook), TRUE);
}


extern void create_note_page(gchararray name, gint64 id, NodeTreeType type) {
    gint *value = (gint *) g_hash_table_lookup(hashTable, &id);
    //page已经存在直接切换到到该page
    if (value != NULL) {
        gtk_notebook_set_current_page(GTK_NOTEBOOK(notebook), *value);
        return;
    }

    GtkWidget *tabLabel = fx_create_tab_label(name, id, type);

    HttpRequestPane *pane = http_request_pane_new(id);
    pane->index= gtk_notebook_append_page(GTK_NOTEBOOK(notebook),
                                          pane->content,
                                          tabLabel
    );

    g_hash_table_insert(hashTable, fx_gint64_tp_dump(id), pane);
    gtk_widget_show_all(notebook);

}

static void close_btn_clicked(GtkButton *btn, gpointer data) {
    HttpRequestPane *pane = g_hash_table_lookup(hashTable, data);
    if (pane != NULL) {
        gtk_notebook_remove_page(GTK_NOTEBOOK(notebook), pane->index);
        g_hash_table_remove(hashTable, data);
    }
    gtk_widget_show_all(notebook);
    FX_FREE(data)
}

static GtkWidget *fx_create_tab_label(gchararray name, gint64 id, NodeTreeType type) {
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);

    GtkWidget *img = gtk_image_new();
    GtkWidget *label = gtk_label_new(name);
    GtkWidget *closeBtn = gtk_button_new();
    GdkPixbuf *pixBuf = fx_get_tree_icon(type);

    GdkPixbuf *iconPixbuf = gdk_pixbuf_new_from_resource(
            GET_INNER_IMG_RESOURCE(close.svg),
            NULL
    );

    add_style_class_to_widget(box,"tab-box");
    add_style_class_to_widget(closeBtn, "tab-close-btn");
    gtk_button_set_image(GTK_BUTTON(closeBtn), gtk_image_new_from_pixbuf(iconPixbuf));


    gtk_image_set_from_pixbuf(GTK_IMAGE(img), pixBuf);

    gtk_widget_set_can_focus(closeBtn,FALSE);
    gtk_box_set_spacing(GTK_BOX(box),3);
    gtk_container_add(GTK_CONTAINER(box), img);
    gtk_container_add(GTK_CONTAINER(box), label);
    gtk_container_add(GTK_CONTAINER(box), closeBtn);

    gtk_widget_show_all(box);

    //注册关闭事件
    g_signal_connect(
            closeBtn,
            "clicked",
            close_btn_clicked,
            fx_gint64_tp_dump(id)
    );


    g_object_unref(pixBuf);
    g_object_unref(iconPixbuf);

    return box;
}