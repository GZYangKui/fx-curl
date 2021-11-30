//
// Created by yangkui on 2021/11/21.
//
#include "include/common.h"

extern gint64 fx_get_timestamp() {
    GTimeZone *zero = g_time_zone_new_local();
    GDateTime *dateTime = g_date_time_new_now(zero);
    gint64 timestamp = g_date_time_to_unix(dateTime);

    g_time_zone_unref(zero);
    g_date_time_unref(dateTime);

    return timestamp * 1000;
}

extern gchararray fx_trf_stack_to_dump(gchararray src) {
    gchararray des = g_malloc(strlen(src) + 1);
    strcpy(des, src);
    return des;
}

extern gint64 *fx_gint64_tp_dump(gint64 stackVal) {
    gint64 *des = g_malloc(sizeof(gint64));
    *des = stackVal;
}

extern GdkPixbuf *fx_get_tree_icon(NodeTreeType type) {
    GdkPixbuf *pixBuf;
    if (type == FOLDER) {
        pixBuf = gdk_pixbuf_new_from_resource(GET_INNER_IMG_RESOURCE(folder.svg), NULL);
    } else {
        pixBuf = gdk_pixbuf_new_from_resource(GET_INNER_IMG_RESOURCE(api.svg), NULL);
    }
    return pixBuf;
}

extern gchararray fx_get_request_method_str(HttpRequestMethod method) {
    static const gchararray HTTP_REQUEST_METHODS[] = {
            "GET",
            "POST",
            "PUT",
            "PATCH",
            "DELETE",
            "COPY",
            "HEAD",
            "OPTIONS"
    };
    return *(HTTP_REQUEST_METHODS + method);
}

/**
 * 应用logo列表
 */
gchar *logos[] = {
        GET_INNER_IMG_RESOURCE(logo.svg),
        GET_INNER_IMG_RESOURCE(logo2x.svg),
        GET_INNER_IMG_RESOURCE(logo3x.svg)
};

extern UpdateUIRequest *fx_update_ui_request_new(gboolean status, gchararray msg, gpointer data) {
    UpdateUIRequest *request = g_malloc(sizeof(UpdateUIRequest));
    request->data = NULL;
    request->message = NULL;
    request->status = status;
    if (msg != NULL) {
        request->message = fx_trf_stack_to_dump(request->message);
    }
    request->data = data;

    return request;
}

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

extern void add_style_sheet_to_widget(GtkWidget *target, gchararray stylesheets, gint prior) {
    GtkCssProvider *cssProvider;
    GtkStyleContext *styleContext;
    cssProvider = gtk_css_provider_new();
    styleContext = gtk_widget_get_style_context(target);
    gtk_css_provider_load_from_resource(cssProvider, stylesheets);
    gtk_style_context_add_provider(styleContext, GTK_STYLE_PROVIDER(cssProvider), prior);
}

extern void add_style_class_to_widget(GtkWidget *target, gchararray clazz) {
    GtkStyleContext *context = gtk_widget_get_style_context(target);
    gtk_style_context_add_class(context, clazz);
}


extern guint64 open_input_dialog(const gchar *title, GtkWindow *parent, guint maxLen, gchar *buff) {
    GtkWidget *entry;
    GtkWidget *label;
    GtkWidget *dialog;
    GtkWidget *dialogPane;
    GtkWidget *contentPane;
    entry = gtk_entry_new();
    label = gtk_label_new(title);

    contentPane = gtk_box_new(GTK_ORIENTATION_VERTICAL, 15);

    dialog = gtk_dialog_new_with_buttons(
            "输入框",
            parent,
            GTK_DIALOG_MODAL,
            "确定", GTK_RESPONSE_ACCEPT,
            "取消", GTK_RESPONSE_REJECT,
            NULL
    );
    dialogPane = gtk_dialog_get_content_area(GTK_DIALOG(dialog));


    gtk_widget_set_halign(label, GTK_ALIGN_START);
    gtk_container_add(GTK_CONTAINER(contentPane), label);
    gtk_container_add(GTK_CONTAINER(contentPane), entry);

    gtk_container_add(GTK_CONTAINER(dialogPane), contentPane);


    gtk_window_set_resizable(GTK_WINDOW(dialog), FALSE);
    gtk_window_set_default_size(GTK_WINDOW(dialog), 500, 100);
    gtk_widget_show_all(dialog);
    gint rs = gtk_dialog_run(GTK_DIALOG(dialog));
    guint64 len = 0;
    if (rs == GTK_RESPONSE_ACCEPT) {
        const gchar *str = gtk_entry_get_text(GTK_ENTRY(entry));
        guint64 strLen = strlen(str);
        len = (strLen >= maxLen - 1 ? maxLen : strLen);
        strncpy(buff, str, len + 1);
    }
    //销毁对话框
    gtk_widget_destroy(dialog);
    return len;
}

extern void show_error_dialog(gchararray title, gchararray content) {
    GdkPixbuf *pixBuf;
    GtkWidget *errIcon;
    GtkBuilder *builder;
    GtkWidget *errDialog;
    GtkWidget *errHeaderText;
    GtkWidget *errContentText;

    gchararray path = GET_INNER_UI_RESOURCE(widget/ErrorDialog.ui);
    builder = gtk_builder_new_from_resource(path);

    errIcon = GTK_WIDGET(gtk_builder_get_object(builder, "errIcon"));
    errDialog = GTK_WIDGET(gtk_builder_get_object(builder, "errDialog"));
    errHeaderText = GTK_WIDGET(gtk_builder_get_object(builder, "errHeaderText"));
    errContentText = GTK_WIDGET(gtk_builder_get_object(builder, "errContentText"));

    pixBuf = gdk_pixbuf_new_from_resource(GET_INNER_IMG_RESOURCE(error.svg), NULL);

    gtk_label_set_text(GTK_LABEL(errHeaderText), title);
    gtk_image_set_from_pixbuf(GTK_IMAGE(errIcon), pixBuf);
    gtk_label_set_text(GTK_LABEL(errContentText), content);
    gtk_window_set_title(GTK_WINDOW(errDialog), "错误");

    add_style_sheet_to_widget(
            gtk_dialog_get_content_area(GTK_DIALOG(errDialog)),
            GET_INNER_CSS_RESOURCE(ErrDialogStyle.css),
            GTK_STYLE_PROVIDER_PRIORITY_USER
    );


    gtk_dialog_run(GTK_DIALOG(errDialog));

    gtk_widget_destroy(errDialog);
}


