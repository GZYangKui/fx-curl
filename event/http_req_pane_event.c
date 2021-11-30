#include "../include/http_req_pane_event.h"

static gboolean inter_update_status(gpointer);

void update_http_request_status(HttpRequestPane *pane) {
    g_idle_add(inter_update_status, pane);
}

void fx_cancel_http_request(gpointer userData){
    HttpRequestPane *pane = userData;
    if (!pane->cancellable){
        return;
    }
    g_cancellable_cancel(pane->cancellable);
    //reduce ref count
    g_object_unref(pane->cancellable);
    pane->step = REGULAR;
    //Update show conent
    inter_update_status(userData);
}

static gboolean inter_update_status(gpointer userData) {
    HttpRequestPane *pane = userData;
    gchararray name = "";
    if (pane->step == LOADING) {
        name = "RespLoading";
    } else if (pane->step == RESPONSE) {
        name = "RespDetail";
    } else {
        name = "RespPlaceHolder";
    }
    //switch show content
    gtk_stack_set_visible_child_name(GTK_STACK(pane->footPane), name);
}

