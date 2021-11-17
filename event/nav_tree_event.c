//
// Created by yangkui on 2021/11/16.
//

#include "../include/ui_util.h"
#include "../include/nav_tree_event.h"

GtkWidget *navTree;
GtkTreeModel *treeModel;

MenuItemMeta itemMetas[] = {
        {
                "新增请求",
                FOLDER
        },
        {
                "新增目录",
                FOLDER
        },
        {
                "重命名",
                ALL
        },
        {
                "删除",
                ALL
        },
        {
                "导出",
                FOLDER
        }
};

static void do_create_folder(GtkTreeIter *parent);


static void do_popup_menu(GtkTreeView *treeView, gint colType, GdkEventButton *event);


extern void fx_init_nav_tree(GtkBuilder *builder) {
    GtkTreeViewColumn *col1;
    GtkTreeViewColumn *col2;
    GtkCellRenderer *textRen, *iconRen;

    GObject *_col1 = gtk_builder_get_object(builder, "iconTreeCol");
    GObject *_col2 = gtk_builder_get_object(builder, "navTreeCol");
    GObject *_navTree = gtk_builder_get_object(builder, "navTree");


    navTree = GTK_WIDGET(_navTree);
    col1 = GTK_TREE_VIEW_COLUMN(_col1);
    col2 = GTK_TREE_VIEW_COLUMN(_col2);

    treeModel = GTK_TREE_MODEL(gtk_tree_store_new(NUM_COLS,
                                                  G_TYPE_OBJECT,
                                                  G_TYPE_STRING,
                                                  G_TYPE_INT)
    );

    textRen = gtk_cell_renderer_text_new();
    iconRen = gtk_cell_renderer_pixbuf_new();


    //将单元格添加到列的头部,end方法是向尾部添加
    gtk_tree_view_column_pack_start(col1, iconRen, FALSE);
    gtk_tree_view_column_pack_start(col2, textRen, TRUE);

    gtk_tree_view_column_add_attribute(col1, iconRen, "pixbuf", ICON_COLUMN);
    gtk_tree_view_column_add_attribute(col2, textRen, "text", TEXT_COLUMN);
    gtk_tree_view_set_model(GTK_TREE_VIEW(navTree), treeModel);
    g_object_unref(treeModel);


}

extern void fx_dy_dir(GtkButton *button, gpointer userData) {
    do_create_folder(NULL);
}


void menu_item_select(GtkMenuItem *item, GdkEvent *event, gpointer *userData) {
    GtkTreeIter iter;
    GtkTreePath *path;
    GtkTreeSelection *selection;
    selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(navTree));
    GList *list = gtk_tree_selection_get_selected_rows(selection, NULL);
    guint len = g_list_length(list);
    if (len < 1) {
        return;
    }
    MenuItemMeta *select;
    select = (MenuItemMeta *) userData;
    path = list->data;
    gtk_tree_model_get_iter(treeModel, &iter, path);
    //添加请求
    if (select == &itemMetas[0]) {

    }
    //新增文件夹
    if (select == &itemMetas[1]) {
        do_create_folder(&iter);
    }
    g_list_free(list);
}


extern gboolean fx_nav_tree_click(GtkWidget *treeView, GdkEventButton *event, gpointer *userData) {
    if (event->type == GDK_BUTTON_PRESS && event->button == 3) {
        gint x, y;
        GtkTreePath *path;
        GtkTreeSelection *selection;

        x = (gint) (event->x);
        y = (gint) (event->y);
        selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(treeView));
        if (!gtk_tree_view_get_path_at_pos(GTK_TREE_VIEW(treeView), x, y, &path, NULL, NULL, NULL)) {
            return FALSE;
        }

        gtk_tree_selection_unselect_all(selection);
        gtk_tree_selection_select_path(selection, path);

        GtkTreeIter iter;

        gtk_tree_model_get_iter(treeModel, &iter, path);
        gchar *text;
        gint colType;
        gtk_tree_model_get(
                treeModel,
                &iter,
                TEXT_COLUMN, &text,
                COL_TYPE, &colType,
                -1
        );
        gtk_tree_path_free(path);
        do_popup_menu(GTK_TREE_VIEW(treeView), colType, event);
        return TRUE;
    }
    return FALSE;
}


static void do_popup_menu(GtkTreeView *treeView, gint colType, GdkEventButton *event) {
    GtkWidget *menu;
    menu = gtk_menu_new();
    gint index = 0;
    for (int i = 0; i < sizeof(itemMetas) / sizeof(itemMetas[0]); ++i) {
        MenuItemMeta meta = itemMetas[i];
        if (meta.colType != ALL & colType != meta.colType) {
            continue;
        }
        GtkWidget *item = gtk_menu_item_new_with_label(meta.name);
        gtk_widget_show(item);
        gtk_menu_attach(GTK_MENU(menu), item, 0, 1, index, index + 1);
        g_signal_connect(item, "button-release-event", G_CALLBACK(menu_item_select), &itemMetas[i]);
        ++index;
    }
    if (index > 0) {
        gtk_menu_popup_at_pointer(GTK_MENU(menu), NULL);
    }
}

static void do_create_folder(GtkTreeIter *parent) {
    GdkPixbuf *pixBuf;
    GtkTreeIter iter;
    GtkTreeStore *treeStore;

    treeStore = GTK_TREE_STORE(treeModel);
    gint maxInputLen = 255;

    gchar buffer[maxInputLen];
    memset(buffer, 0, maxInputLen);
    guint64 len = open_input_dialog("请输入目录名称", NULL, maxInputLen, buffer);

    //输入为空或者取消不做处理
    if (len <= 0) {
        return;
    }

    pixBuf = gdk_pixbuf_new_from_resource(GET_INNER_IMG_RESOURCE(api_folder.svg), NULL);
    gtk_tree_store_append(treeStore, &iter, parent);
    gtk_tree_store_set(treeStore, &iter,
                       ICON_COLUMN, pixBuf,
                       TEXT_COLUMN, buffer,
                       -1);
    g_object_unref(pixBuf);
}


