#include <stdio.h>

#include <gtk/gtk.h>
#include "include/ui_util.h"
#include "include/common.h"
#include "include/database.h"
#include "include/fx_resources.h"

#define WINDOW_WIDTH 700
#define WINDOW_HEIGHT 450

FXWindowContext *splashWinContext;

static gboolean check_async_que(gpointer userData);

/**
 * 应用logo列表
 */
gchar *logos[] = {
        GET_INNER_IMG_RESOURCE(logo.svg),
        GET_INNER_IMG_RESOURCE(logo2x.svg),
        GET_INNER_IMG_RESOURCE(logo3x.svg)
};

/**
 *
 * 设置窗口默认图标
 *
 */
extern void set_window_default_icons(GtkWidget *window) {
    GList *icons;
    icons = g_list_alloc();
    for (int i = 0; i < 3; ++i) {
        gchar *logo;
        GdkPixbuf *pixBuf;
        logo = *(logos + i);
        pixBuf = gdk_pixbuf_new_from_resource(logo, NULL);
        if (i == 0) {
            icons->data = pixBuf;
        } else {
            icons = g_list_append(icons, pixBuf);
        }
    }
    gtk_window_set_icon_list(GTK_WINDOW(window), icons);
    g_list_free(icons);
}

static void activate(GtkApplication *app, gpointer user_data) {
    GtkWidget *window;
    GtkWidget *mainBox;
    GtkBuilder *builder;

    window = gtk_application_window_new(app);
    builder = gtk_builder_new_from_resource(GET_INNER_UI_RESOURCE(SplashView.ui));

    mainBox = GTK_WIDGET(gtk_builder_get_object(builder, "mainBox"));


    add_style_sheet_to_widget(mainBox, GET_INNER_CSS_RESOURCE(SplashStyle.css),1);

    gtk_container_add(GTK_CONTAINER(window), mainBox);

    set_window_default_icons(window);
    gtk_window_set_default_size(GTK_WINDOW(window), WINDOW_WIDTH, WINDOW_HEIGHT);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_window_set_type_hint(GTK_WINDOW(window), GDK_WINDOW_TYPE_HINT_POPUP_MENU);

    gtk_widget_show_all(window);

    splashWinContext = (FXWindowContext *) malloc(sizeof(FXWindowContext));
    splashWinContext->window = window;
    splashWinContext->asyncQueue = g_async_queue_new();

    //检查队列
    g_idle_add(check_async_que, NULL);

    //初始化数据库
    g_thread_new("check-database-thread", fx_init_sqlite, NULL);
}

static gboolean check_async_que(gpointer userData) {
    gpointer *data = g_async_queue_try_pop(splashWinContext->asyncQueue);
    if (data != NULL) {
        QueuePayload *msg = (QueuePayload *) data;
        printf("%s\n", msg->message);
        show_error_dialog("测试","测试");
    }
    return data != NULL;
}

int main(int argc, char **argv) {
    GtkApplication *app;


    app = gtk_application_new("cn.navclub", G_APPLICATION_FLAGS_NONE);

    g_signal_connect (app, "activate", G_CALLBACK(activate), NULL);

    g_application_run(G_APPLICATION(app), argc, argv);
    fx_shutdown_sqlite3();
}


