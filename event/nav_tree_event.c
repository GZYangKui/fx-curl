//
// Created by yangkui on 2021/11/16.
//

#include "../include/database.h"
#include "../include/nav_tree_event.h"

GtkWidget *navTree;
GtkTreeModel *treeModel;

MenuItemMeta itemMetas[] = {
        {
                "刷新",
                FOLDER
        },
        {
                "打开",
                API,
        },
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


static void clear_next_node(GtkTreeIter *treeIter);

static void do_create_node(GtkTreeIter *parent, gint type, gint64 parentId);

static void fx_load_node_tree(GtkTreeIter *iter, gint64 parentId);

static void internal_do_create_folder(GtkTreeIter *parent, gint64 id, gint type, gchararray name);

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
                                                  G_TYPE_INT,
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

    fx_load_node_tree(NULL, 0);

}

extern void fx_dy_dir(GtkButton *button, gpointer userData) {
    do_create_node(NULL, FOLDER, 0);
}


void menu_item_select(GtkMenuItem *item, GdkEvent *event, gpointer *userData) {
    GtkTreeIter iter;
    GtkTreePath *path = NULL;
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
    gint64 id = 0;
    gtk_tree_model_get(treeModel, &iter, COL_ID, &id, -1);
    //刷新
    if (select == &itemMetas[0]) {
        fx_load_node_tree(&iter, id);
    }
    //新增请求
    if (select == &itemMetas[2]) {
        do_create_node(&iter, API, id);
    }
    //新增文件夹
    if (select == &itemMetas[3]) {
        do_create_node(&iter, FOLDER, id);
    }
    //打开api接口
    if (select == &itemMetas[1]){

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

//        GList *list = gtk_tree_selection_get_selected_rows(selection, &treeModel);
//
//        if (list == NULL) {
//            return FALSE;
//        }

        gtk_tree_selection_unselect_all(selection);
        gtk_tree_selection_select_path(selection, path);
//        path = list->data;
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

static void do_create_node(GtkTreeIter *parent, gint type, gint64 parentId) {
    GdkPixbuf *pixBuf;
    GtkTreeIter iter;
    GtkTreeStore *treeStore;

    treeStore = GTK_TREE_STORE(treeModel);
    gint maxInputLen = 255;

    gchar buffer[maxInputLen];
    memset(buffer, 0, maxInputLen);
    guint64 len = open_input_dialog("请输入名称", NULL, maxInputLen, buffer);

    //输入为空或者取消不做处理
    if (len <= 0) {
        return;
    }
    gint64 id = 0;
    gboolean ok = insert_tree_node(&id, parentId, type, buffer);
    if (ok) {
        internal_do_create_folder(parent, id, type, buffer);
    } else {
        gchar errMsg[255];
        sprintf(errMsg, "插入数据过程发生未知错误(错误码:%ld)", id);
        show_error_dialog("新建文件夹失败!", errMsg);
    }
}

static void internal_do_create_folder(GtkTreeIter *parent, gint64 id, gint type, gchararray name) {
    GtkTreeIter iter;
    GdkPixbuf *pixBuf;
    if (type == FOLDER) {
        pixBuf = gdk_pixbuf_new_from_resource(GET_INNER_IMG_RESOURCE(folder.svg), NULL);
    } else {
        pixBuf = gdk_pixbuf_new_from_resource(GET_INNER_IMG_RESOURCE(api.svg), NULL);
    }
    gtk_tree_store_append(GTK_TREE_STORE(treeModel), &iter, parent);
    gtk_tree_store_set(GTK_TREE_STORE(treeModel), &iter,
                       COL_ID, id,
                       COL_TYPE, type,
                       TEXT_COLUMN, name,
                       ICON_COLUMN, pixBuf,
                       -1
    );
    g_object_unref(pixBuf);
}

/**
 *
 * 从数据库初始化列表
 *
 */
static void fx_load_node_tree(GtkTreeIter *iter, gint64 parentId) {
    clear_next_node(iter);
    //重新加载新数据
    GList *list = g_list_alloc();
    select_node_by_parent_id(parentId, list);
    guint len = g_list_length(list);
    GList *temp = list;
    for (int i = 0; i < len; ++i) {
        NodeTree *treeNode = temp->data;
        if (treeNode == NULL) {
            continue;
        }
        internal_do_create_folder(iter, treeNode->id, treeNode->type, treeNode->name);
        FX_FREE_TREE_NODE(treeNode, FALSE)
        temp = temp->next;
    }
    if (iter != NULL) {
        GtkTreePath *treePath = NULL;
        treePath = gtk_tree_model_get_path(GTK_TREE_MODEL(treeModel), iter);
        gtk_tree_view_expand_row(GTK_TREE_VIEW(navTree), treePath, FALSE);
        gtk_tree_path_free(treePath);
    }
    g_list_free(list);
}

/**
 *
 * 清除当前迭代器下的节点列表
 *
 */
static void clear_next_node(GtkTreeIter *iter) {
    if (iter == NULL) {
        return;
    }
    gint num = gtk_tree_model_iter_n_children(GTK_TREE_MODEL(treeModel), iter);
    if (num < 0) {
        return;
    }
    gboolean ok = TRUE;
    GtkTreeIter child;
    while (ok) {
        ok = gtk_tree_model_iter_children(GTK_TREE_MODEL(treeModel), &child, iter);
        if (ok) {
            gtk_tree_store_remove(GTK_TREE_STORE(treeModel), &child);
        }
    }
}


