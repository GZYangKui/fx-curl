//
// Created by yangkui on 2021/11/16.
//

#include "../include/nav_notebook_event.h"

GtkWidget *notebook;
extern void fx_inti_nav_notebook(GtkBuilder *builder){
    GObject *_notebook;
    _notebook = gtk_builder_get_object(builder, "notebook");


    notebook = GTK_WIDGET(_notebook);

    gtk_notebook_append_page(GTK_NOTEBOOK(notebook), gtk_button_new_with_label("test"), gtk_label_new("test"));

}