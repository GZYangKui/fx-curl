//
// Created by yangkui on 2021/11/17.
//
#include <string.h>
#include "include/ui_util.h"


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
        len = (strLen >= maxLen-1 ? maxLen : strLen);
        strncpy(buff, str, len+1);
    }
    //销毁对话框
    gtk_widget_destroy(dialog);
    return len;
}
