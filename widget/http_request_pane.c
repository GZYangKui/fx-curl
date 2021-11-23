//
// Created by yangkui on 2021/11/23.
//


#include "../include/http_request_pane.h"


extern GtkWidget *http_request_pane_new(gint64 id) {
    GtkBuilder *builder = gtk_builder_new_from_resource(GET_INNER_UI_RESOURCE(widget/HttpReqPane.ui));
    GtkWidget *pane = GTK_WIDGET(gtk_builder_get_object(builder, "httpReqPane"));
    printf("%p\n",pane);
    return pane;
}


