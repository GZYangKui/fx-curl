//
// Created by yangkui on 2021/11/18.
//
#include "./include/ui_util.h"
#include "./include/fx_curl.h"
#include "./include/database.h"
#include "./include/nav_tree_event.h"
#include "./include/nav_notebook_event.h"

#define WINDOW_WIDTH 1300
#define WINDOW_HEIGHT 800
#define WINDOW_TITLE "FxCurl"


static void window_destroy(GtkWidget *object, gpointer user_data);

extern void open_fx_curl() {
    GtkWidget *window;
    GtkBuilder *builder;

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

    builder = gtk_builder_new_from_resource(GET_INNER_UI_RESOURCE(MainView.ui));

    GObject *obj = gtk_builder_get_object(builder, "main-box");


    gtk_builder_connect_signals(builder, NULL);

    set_window_default_icons(window);

    gtk_window_set_title(GTK_WINDOW(window), WINDOW_TITLE);
    gtk_container_add(GTK_CONTAINER(window), GTK_WIDGET(obj));
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_window_set_default_size(GTK_WINDOW(window), WINDOW_WIDTH, WINDOW_HEIGHT);

    fx_init_nav_tree(builder);
    fx_inti_nav_notebook(builder);

    g_signal_connect(window, "destroy", window_destroy, NULL);

    gtk_widget_show_all(window);
}

static void window_destroy(GtkWidget *object, gpointer user_data) {
    extern void  fx_quit_curl();

    fx_quit_curl();
}
