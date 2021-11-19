//
// Created by yangkui on 2021/11/17.
//
#include <string.h>
#include "include/common.h"
#include "include/ui_util.h"

extern void add_style_sheet_to_widget(GtkWidget *target, gchararray stylesheets, gint prior) {
    GtkCssProvider *cssProvider;
    GtkStyleContext *styleContext;
    cssProvider = gtk_css_provider_new();
    styleContext = gtk_widget_get_style_context(target);
    gtk_css_provider_load_from_resource(cssProvider, stylesheets);
    gtk_style_context_add_provider(styleContext, GTK_STYLE_PROVIDER(cssProvider), prior);
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

extern void show_error_dialog(gchararray title, gchararray context) {
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
    errContentText = GTK_WIDGET(gtk_builder_get_object(builder,"errContentText"));

    pixBuf = gdk_pixbuf_new_from_resource(GET_INNER_IMG_RESOURCE(error.svg), NULL);

    gtk_label_set_text(GTK_LABEL(errHeaderText), title);
    gtk_image_set_from_pixbuf(GTK_IMAGE(errIcon), pixBuf);
    gtk_label_set_text(GTK_LABEL(errContentText),context);
    gtk_window_set_title(GTK_WINDOW(errDialog), "错误");

    add_style_sheet_to_widget(errDialog, GET_INNER_CSS_RESOURCE(ErrDialogStyle.css),1);

    gtk_dialog_run(GTK_DIALOG(errDialog));

    gtk_widget_destroy(errDialog);
}
