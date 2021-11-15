#include <stdio.h>

#include <gtk/gtk.h>
#include "headers/fx_resources.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define WINDOW_TITLE "CurlFX"

GtkWidget *navTree;
GtkWidget *notebook;

enum {
    ICON_COLUMN = 0,
    TEXT_COLUMN,
    NUM_COLS
};

/**
 * 动态创建文件夹
 */
void fx_dy_dir(GtkWidget *widget, gpointer *userData) {
    //创建迭代器
    GtkTreeIter treeIter;
    GtkTreeModel *treeModel;

    treeModel = gtk_tree_view_get_model(GTK_TREE_VIEW(navTree));

    gtk_tree_store_append(GTK_TREE_STORE(treeModel), &treeIter, NULL);

    //gtk_tree_store_set(GTK_TREE_STORE(treeModel), &treeIter, COLUMN, "New Collection", -1);

}

void tree_mouse_clicked(GtkWidget *widget, GdkEvent *event, gpointer *userData) {
    //右键触发
    if (event->button.button == GDK_BUTTON_SECONDARY) {
        GtkTreeSelection *select = gtk_tree_view_get_selection(GTK_TREE_VIEW(navTree));

        userData = gtk_tree_selection_get_user_data(select);
        GtkTreeModel *treeModel = gtk_tree_view_get_model(GTK_TREE_VIEW(navTree));
        GList *list = gtk_tree_selection_get_selected_rows(select, &treeModel);
        printf("wancehng\n");


    }
}

GtkTreeModel *create_and_fill_model(void) {

    GtkTreeIter toplevel, child;
    GtkTreeStore *treeStore = gtk_tree_store_new(NUM_COLS,
                                                 G_TYPE_OBJECT,
                                                 G_TYPE_STRING);


    gtk_tree_store_append(treeStore, &toplevel, NULL);

   GdkPixbuf *pixBuf = gdk_pixbuf_new_from_resource("/cn/navclub/img/api_folder.svg",NULL);

    gtk_tree_store_set(treeStore, &toplevel,
                       ICON_COLUMN, pixBuf,
                       TEXT_COLUMN,"ddddddddddddddddddd"
                       -1,-1);

    gtk_tree_store_append(treeStore, &child, &toplevel);
    gtk_tree_store_set(treeStore, &child,
                       ICON_COLUMN, pixBuf,
                       TEXT_COLUMN,"fffffffffffffffffffffffffffffff"
                       -1,-1);

    g_object_unref(pixBuf);
//    gtk_tree_store_append(treeStore, &child, &toplevel);
//    gtk_tree_store_set(treeStore, &child,
//                       COLUMN, "Perl",
//                       -1);
//    gtk_tree_store_append(treeStore, &child, &toplevel);
//    gtk_tree_store_set(treeStore, &child,
//                       COLUMN, "PHP",
//                       -1);
//
//    gtk_tree_store_append(treeStore, &toplevel, NULL);
//
//    gtk_tree_store_set(treeStore, &toplevel,
//                       COLUMN, "Compiled languages",
//                       -1);
//
//    gtk_tree_store_append(treeStore, &child, &toplevel);
//    gtk_tree_store_set(treeStore, &child,
//                       COLUMN, "C",
//                       -1);
//
//    gtk_tree_store_append(treeStore, &child, &toplevel);
//    gtk_tree_store_set(treeStore, &child,
//                       COLUMN, "C++",
//                       -1);
//
//    gtk_tree_store_append(treeStore, &child, &toplevel);
//    gtk_tree_store_set(treeStore, &child,
//                       COLUMN, "Java",
//                       -1);

    return GTK_TREE_MODEL(treeStore);
}


void *init_nav_tree_view(GtkBuilder *builder) {

    GtkTreeModel *model;
    GtkTreeViewColumn *col1;
    GtkTreeViewColumn *col2;
    GtkTreeSelection *selection;
    GtkCellRenderer *textRen, *iconRen;

    GObject *_col1 = gtk_builder_get_object(builder, "iconTreeCol");
    GObject *_col2 = gtk_builder_get_object(builder, "navTreeCol");
    GObject *_navTree = gtk_builder_get_object(builder, "navTree");


    navTree = GTK_WIDGET(_navTree);
    col1 = GTK_TREE_VIEW_COLUMN(_col1);
    col2 = GTK_TREE_VIEW_COLUMN(_col2);

    textRen = gtk_cell_renderer_text_new();
    iconRen = gtk_cell_renderer_pixbuf_new();

    model = create_and_fill_model();
    //将单元格添加到列的头部,end方法是向尾部添加
    gtk_tree_view_column_pack_start(col1, iconRen, FALSE);
    gtk_tree_view_column_pack_start(col2, textRen, TRUE);
    gtk_tree_view_column_add_attribute(col1, iconRen, "pixbuf", ICON_COLUMN);
    gtk_tree_view_column_add_attribute(col2,textRen,"text",TEXT_COLUMN);
    gtk_tree_view_set_model(GTK_TREE_VIEW(navTree), model);
    g_object_unref(model);

}

static void activate(GtkApplication *app, gpointer user_data) {
    GtkWidget *window;
    GtkBuilder *builder;
    GtkTreeViewColumn *column;

    window = gtk_application_window_new(app);

    builder = gtk_builder_new_from_resource("/cn/navclub/ui/MainView.ui");

    GObject *obj = gtk_builder_get_object(builder, "main-box");
    GObject *_notebook = gtk_builder_get_object(builder, "notebook");


    notebook = GTK_WIDGET(_notebook);

    gtk_builder_connect_signals(builder, NULL);

    gtk_notebook_append_page(GTK_NOTEBOOK(notebook), gtk_button_new_with_label("test"), gtk_label_new("test"));
    gtk_window_set_title(GTK_WINDOW(window), WINDOW_TITLE);
    gtk_container_add(GTK_CONTAINER(window), GTK_WIDGET(obj));
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_window_set_default_size(GTK_WINDOW(window), WINDOW_WIDTH, WINDOW_HEIGHT);


    init_nav_tree_view(builder);
    gtk_widget_show_all(window);
    gtk_window_present(GTK_WINDOW (window));
}

int main(int argc, char **argv) {
    GtkApplication *app;


    app = gtk_application_new("cn.navclub", G_APPLICATION_FLAGS_NONE);

    g_signal_connect (app, "activate", G_CALLBACK(activate), NULL);

    g_application_run(G_APPLICATION(app), argc, argv);
}
