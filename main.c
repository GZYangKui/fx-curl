#include <stdio.h>

#include <gtk/gtk.h>
#include "include/fx_curl.h"
#include "include/fx_resources.h"

#define WINDOW_WIDTH 1300
#define WINDOW_HEIGHT 800
#define WINDOW_TITLE "CurlFX"

static void activate(GtkApplication *app, gpointer user_data) {
    GtkWidget *window;
    GtkBuilder *builder;

    window = gtk_application_window_new(app);

    builder = gtk_builder_new_from_resource("/cn/navclub/ui/MainView.ui");

    GObject *obj = gtk_builder_get_object(builder, "main-box");


    gtk_builder_connect_signals(builder, NULL);

    gtk_window_set_title(GTK_WINDOW(window), WINDOW_TITLE);
    gtk_container_add(GTK_CONTAINER(window), GTK_WIDGET(obj));
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_window_set_default_size(GTK_WINDOW(window), WINDOW_WIDTH, WINDOW_HEIGHT);

    fx_init_nav_tree(builder);
    fx_inti_nav_notebook(builder);

    gtk_widget_show_all(window);
}

int main(int argc, char **argv) {
    GtkApplication *app;


    app = gtk_application_new("cn.navclub", G_APPLICATION_FLAGS_NONE);

    g_signal_connect (app, "activate", G_CALLBACK(activate), NULL);

    g_application_run(G_APPLICATION(app), argc, argv);
}
