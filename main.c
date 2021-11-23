#include <stdio.h>

#include <gtk/gtk.h>
#include "include/common.h"
#include "include/database.h"
#include "include/fx_curl.h"
#include "include/fx_resources.h"

#define WINDOW_WIDTH 700
#define WINDOW_HEIGHT 450


static GtkApplication *app;
static GtkWidget *appWindow;
static gint checkTaskNum = 1;
static GtkWidget *progressBar;
static gint alreadyCheckTask = 0;


static void apply_global_style();

static gboolean update_ui(gpointer userData);

static void fx_init_program(gpointer userData);

static void requestUpdateUI(gboolean ok, GError *error);

static void activate(GtkApplication *_app, gpointer user_data) {
    GtkWidget *mainBox;
    GtkBuilder *builder;

    appWindow = gtk_application_window_new(app);
    builder = gtk_builder_new_from_resource(GET_INNER_UI_RESOURCE(SplashView.ui));

    mainBox = GTK_WIDGET(gtk_builder_get_object(builder, "mainBox"));
    progressBar = GTK_WIDGET(gtk_builder_get_object(builder, "progressBar"));

    add_style_sheet_to_widget(mainBox, GET_INNER_CSS_RESOURCE(SplashStyle.css), 1);

    gtk_container_add(GTK_CONTAINER(appWindow), mainBox);

    gtk_window_set_default_size(GTK_WINDOW(appWindow), WINDOW_WIDTH, WINDOW_HEIGHT);
    gtk_window_set_position(GTK_WINDOW(appWindow), GTK_WIN_POS_CENTER);
    gtk_window_set_type_hint(GTK_WINDOW(appWindow), GDK_WINDOW_TYPE_HINT_POPUP_MENU);

    gtk_widget_show_all(appWindow);

    apply_global_style();

    //初始化数据库
    g_thread_new("check-database-thread", fx_init_program, NULL);
}

void apply_global_style() {
    GdkScreen *screen;
    GtkCssProvider *cssProvider;
    screen = gdk_screen_get_default();
    cssProvider = gtk_css_provider_new();
    gtk_css_provider_load_from_resource(cssProvider, GET_INNER_CSS_RESOURCE(GlobalStyle.css));
    gtk_style_context_add_provider_for_screen(screen, GTK_STYLE_PROVIDER(cssProvider),
                                              GTK_STYLE_PROVIDER_PRIORITY_USER);
}

static void fx_init_program(gpointer userData) {
    gboolean ok = FALSE;
    GError *error = NULL;
    ok = fx_init_sqlite(userData, &error);
    requestUpdateUI(ok, error);
    if (ok) {
        //执行下一步初始化
    }
}

static void requestUpdateUI(gboolean ok, GError *error) {
    gchararray msg = NULL;
    if (!ok && error != NULL) {
        msg = error->message;
    }
    UpdateUIRequest *request = fx_update_ui_request_new(ok, msg, NULL);
    g_idle_add(update_ui, request);
}

static gboolean update_ui(gpointer userData) {
    UpdateUIRequest *request = userData;
    if (!request->status) {
        show_error_dialog("程序初始失败!",request->message);
        g_application_quit(G_APPLICATION(app));
    } else {
        alreadyCheckTask++;
        gfloat fraction = ((alreadyCheckTask) / (checkTaskNum * 1.0f));
        gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(progressBar), fraction);
        if (fraction == 1) {
            open_fx_curl();
            gtk_widget_hide(appWindow);
        }
    }
    FREE_UPDATE_UI_REQUEST(request)
    return FALSE;
}
/**
 * 退出当前程序
 */
extern void fx_quit_curl(){
    gtk_widget_destroy(appWindow);
}

int main(int argc, char **argv) {

    app = gtk_application_new("cn.navclub", G_APPLICATION_FLAGS_NONE);

    g_signal_connect (app, "activate", G_CALLBACK(activate), NULL);

    g_application_run(G_APPLICATION(app), argc, argv);
    fx_shutdown_sqlite3();
}


