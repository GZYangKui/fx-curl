//
// Created by yangkui on 2021/11/23.
//

#include <libsoup/soup.h>
#include "../include/http_util.h"
#include "../include/http_request_pane.h"

#define RENDER_EXTRA_PROP "bind-column"


typedef enum {
    OPT,
    KEY,
    VALUE,
    DESC,
    COL_NUM
} RequestTreeColumn;

typedef enum {
    //request params
    PARAMS,
    //request headers
    HEADERS
} RequestTreeType;


static void fx_add_form_row(GtkWidget *, gpointer);

static void fx_remove_form_row(GtkWidget *, gpointer);

static void http_request_url_change(GtkEntry *, gpointer);

static void http_request_method_change(GtkComboBoxText *, gpointer);

static void http_send_http_request(GtkButton *, gpointer);

static void http_request_async_callback(GObject *, GAsyncResult *, gpointer);

static GtkWidget *internal_init_tree_view(GtkBuilder *, RequestTreeType);

static void fx_tree_view_edited(GtkCellRendererText *, gchar *, gchar *, gpointer);


static void *internal_init_btn(GtkBuilder *builder, HttpRequestPane *requestPane, RequestTreeType type);


extern HttpRequestPane *http_request_pane_new(gint64 id) {
    HttpRequestPane *requestPane = g_malloc(sizeof(HttpRequestPane));

    requestPane->id = id;
    requestPane->url = NULL;
    requestPane->current = 0;

    gchararray path = GET_INNER_UI_RESOURCE(widget/HttpReqPane.ui);
    GtkBuilder *builder = gtk_builder_new_from_resource(path);

    requestPane->pTV = internal_init_tree_view(builder, PARAMS);
    requestPane->hTV = internal_init_tree_view(builder, HEADERS);

    internal_init_btn(builder, requestPane, PARAMS);
    internal_init_btn(builder, requestPane, HEADERS);

    GtkButton *btn = GTK_BUTTON(gtk_builder_get_object(builder, "sendBtn"));
    GtkEntry *textField = GTK_ENTRY(gtk_builder_get_object(builder, "textField"));
    GtkComboBox *mdComBox = GTK_COMBO_BOX(gtk_builder_get_object(builder, "mdComBox"));

    g_signal_connect(btn, "clicked", http_send_http_request, requestPane);
    g_signal_connect(mdComBox, "changed", http_request_method_change, requestPane);
    g_signal_connect(textField, "changed", http_request_url_change, requestPane);

    requestPane->content = GTK_WIDGET(gtk_builder_get_object(builder, "httpReqPane"));

    return requestPane;
}

static GtkWidget *internal_init_tree_view(GtkBuilder *builder, RequestTreeType type) {
    gchararray treeId;
    if (type == PARAMS) {
        treeId = "pTV";
    } else {
        treeId = "hTV";
    }
    GtkWidget *treeView = GTK_WIDGET(gtk_builder_get_object(builder, treeId));


    GtkTreeModel *pTM = GTK_TREE_MODEL(gtk_tree_store_new(COL_NUM,
                                                          G_TYPE_BOOLEAN,
                                                          G_TYPE_STRING,
                                                          G_TYPE_STRING,
                                                          G_TYPE_STRING)
    );

    GtkCellRenderer *pKeyRender = gtk_cell_renderer_text_new();
    GtkCellRenderer *pValRender = gtk_cell_renderer_text_new();
    GtkCellRenderer *pDstRender = gtk_cell_renderer_text_new();
    GtkCellRenderer *optRender = gtk_cell_renderer_toggle_new();


    g_object_set(pKeyRender, "editable", TRUE, NULL);
    g_object_set(pValRender, "editable", TRUE, NULL);
    g_object_set(pDstRender, "editable", TRUE, NULL);

    g_signal_connect(pKeyRender, "edited", fx_tree_view_edited, treeView);
    g_signal_connect(pValRender, "edited", fx_tree_view_edited, treeView);
    g_signal_connect(pDstRender, "edited", fx_tree_view_edited, treeView);


    GtkTreeViewColumn *key = GTK_TREE_VIEW_COLUMN(gtk_builder_get_object(builder, type == PARAMS ? "pKey" : "hKey"));
    GtkTreeViewColumn *val = GTK_TREE_VIEW_COLUMN(gtk_builder_get_object(builder, type == PARAMS ? "pVal" : "hVal"));
    GtkTreeViewColumn *dst = GTK_TREE_VIEW_COLUMN(gtk_builder_get_object(builder, type == PARAMS ? "pDst" : "hDst"));
    GtkTreeViewColumn *opt = GTK_TREE_VIEW_COLUMN(gtk_builder_get_object(builder, type == PARAMS ? "pOpt" : "hOpt"));


    g_object_set_data(G_OBJECT(optRender), RENDER_EXTRA_PROP, GUINT_TO_POINTER(OPT));
    g_object_set_data(G_OBJECT(pKeyRender), RENDER_EXTRA_PROP, GUINT_TO_POINTER(KEY));
    g_object_set_data(G_OBJECT(pValRender), RENDER_EXTRA_PROP, GUINT_TO_POINTER(VALUE));
    g_object_set_data(G_OBJECT(pDstRender), RENDER_EXTRA_PROP, GUINT_TO_POINTER(DESC));


    gtk_tree_view_column_pack_start(key, pKeyRender, FALSE);
    gtk_tree_view_column_pack_start(val, pValRender, FALSE);
    gtk_tree_view_column_pack_start(dst, pDstRender, FALSE);
    gtk_tree_view_column_pack_start(opt, optRender, FALSE);

    gtk_tree_view_column_add_attribute(opt, optRender, "active", OPT);
    gtk_tree_view_column_add_attribute(key, pKeyRender, "text", KEY);
    gtk_tree_view_column_add_attribute(val, pValRender, "text", VALUE);
    gtk_tree_view_column_add_attribute(dst, pDstRender, "text", DESC);


    gtk_tree_view_set_model(GTK_TREE_VIEW(treeView), pTM);

    gtk_widget_show_all(treeView);

    g_object_unref(pTM);

    return treeView;
}

static void *internal_init_btn(GtkBuilder *builder, HttpRequestPane *requestPane, RequestTreeType type) {

    GtkWidget *addBtn = GTK_WIDGET(gtk_builder_get_object(builder, type == PARAMS ? "pAD" : "hAD"));
    GtkWidget *helpBtn = GTK_WIDGET(gtk_builder_get_object(builder, type == PARAMS ? "pHP" : "hHP"));
    GtkWidget *deleteBtn = GTK_WIDGET(gtk_builder_get_object(builder, type == PARAMS ? "pDL" : "hDL"));


    GdkPixbuf *addIcon = gdk_pixbuf_new_from_resource(GET_INNER_IMG_RESOURCE(add_round.svg), NULL);
    GdkPixbuf *helpIcon = gdk_pixbuf_new_from_resource(GET_INNER_IMG_RESOURCE(help.svg), NULL);
    GdkPixbuf *deleteIcon = gdk_pixbuf_new_from_resource(GET_INNER_IMG_RESOURCE(delete.svg), NULL);


    gtk_button_set_image(GTK_BUTTON(addBtn), gtk_image_new_from_pixbuf(addIcon));
    gtk_button_set_image(GTK_BUTTON(helpBtn), gtk_image_new_from_pixbuf(helpIcon));
    gtk_button_set_image(GTK_BUTTON(deleteBtn), gtk_image_new_from_pixbuf(deleteIcon));

    GtkWidget *treeView = (type == PARAMS ? requestPane->pTV : requestPane->hTV);

    g_signal_connect(addBtn, "clicked", fx_add_form_row, treeView);
    g_signal_connect(deleteBtn, "clicked", fx_remove_form_row, treeView);

    g_object_unref(addIcon);
    g_object_unref(helpIcon);
    g_object_unref(deleteIcon);
}

/**
 *
 * http请求方法改变将会回调当前函数
 *
 */
static void http_request_method_change(GtkComboBoxText *widget, gpointer data) {
    gint index = gtk_combo_box_get_active(GTK_COMBO_BOX(widget));
    if (index == -1) {
        return;
    }
    HttpRequestPane *pane = (HttpRequestPane *) data;
    pane->method = (HttpRequestMethod) index;
    printf("%d\n", pane->method);
}

/**
 *
 * 当用户输入请求url时回调当前函数
 *
 */
static void http_request_url_change(GtkEntry *entry, gpointer data) {
    HttpRequestPane *pane = (HttpRequestPane *) data;
    if (pane->url != NULL) {
        FX_FREE(pane->url);
    }
    pane->url = fx_trf_stack_to_dump(gtk_entry_get_text(entry));
}

/**
 *
 * 表单数据新增一行
 *
 */
static void fx_add_form_row(GtkWidget *widget, gpointer data) {
    GtkTreeIter iter;
    GtkTreeView *treeView = data;
    GtkTreeModel *treeModel = gtk_tree_view_get_model(treeView);
    GtkTreeSelection *selection = gtk_tree_view_get_selection(treeView);


    gtk_tree_store_append(GTK_TREE_STORE(treeModel), &iter, NULL);
    gtk_tree_store_set(
            GTK_TREE_STORE(treeModel), &iter,
            OPT, TRUE,
            KEY, "Key",
            VALUE, "Value",
            DESC, "",
            -1
    );

    gtk_tree_selection_select_iter(selection, &iter);
}

static void fx_remove_form_row(GtkWidget *widget, gpointer data) {
    GtkTreeIter iter;
    GtkTreeView *treeView = data;
    GtkTreeModel *treeModel = gtk_tree_view_get_model(treeView);
    GtkTreeSelection *selection = gtk_tree_view_get_selection(treeView);

    gboolean ok = gtk_tree_selection_get_selected(selection, &treeModel, &iter);
    if (ok) {
        GtkTreeIter *pre = gtk_tree_iter_copy(&iter);
        //选中最后一个节点
        if (gtk_tree_model_iter_previous(treeModel, pre)) {
            gtk_tree_selection_select_iter(selection, pre);
        }
        //释放复制对象
        gtk_tree_iter_free(pre);
        //移除目标节点
        gtk_tree_store_remove(GTK_TREE_STORE(treeModel), &iter);
    }
}

static void fx_tree_view_edited(GtkCellRendererText *cell,
                                gchar *pathStr,
                                gchar *newStr,
                                gpointer userData) {
    GtkTreeView *treeView = userData;
    GtkTreeIter iter;
    GtkTreePath *path = gtk_tree_path_new_from_string(pathStr);
    GtkTreeModel *treeModel = gtk_tree_view_get_model(GTK_TREE_VIEW(treeView));
    gboolean ok = gtk_tree_model_get_iter(treeModel, &iter, path);
    if (!ok) {
        return;
    }
    gint column = GPOINTER_TO_UINT(g_object_get_data(G_OBJECT(cell), RENDER_EXTRA_PROP));
    gtk_tree_store_set(GTK_TREE_STORE(treeModel), &iter, column, newStr, -1);
}

/**
 * 点击按钮发起http请求
 */
static void http_send_http_request(GtkButton *btn, gpointer data) {
    HttpRequestPane *pane = data;

    SoupSession *session = soup_session_new();
    SoupMessage *msg = soup_message_new(fx_get_request_method_str(GET), pane->url);


    GtkTreeModel *pModel = gtk_tree_view_get_model(GTK_TREE_VIEW(pane->pTV));
    GtkTreeModel *hModel = gtk_tree_view_get_model(GTK_TREE_VIEW(pane->hTV));

    GtkTreeIter iter;

    gboolean ok = gtk_tree_model_get_iter_first(pModel, &iter);

    while (ok) {
        gboolean opt;
        gchararray key;
        gchararray value;
        gtk_tree_model_get(
                hModel, &iter,
                KEY, &key,
                VALUE, &value,
                OPT, &opt,
                -1
        );
        if (opt) {
            soup_message_headers_append(msg->request_headers, key, value);
        }
        ok = gtk_tree_model_iter_next(pModel, &iter);
    }

    soup_session_send_async(session, msg, NULL, http_request_async_callback, pane);

}

static void http_request_async_callback(GObject *object, GAsyncResult *res, gpointer user_data) {
    GInputStream *stream;
    GError *error = NULL;
    SoupSession *session = SOUP_SESSION(object);
//    SoupMessage *ms = soup_session_get_async_result_message(session,res);
    stream = soup_session_send_finish(session, res, &error);
    GObject *obj = g_async_result_get_source_object(res);
    guint8 body[1024];
    gsize size = g_input_stream_read(stream, &body, 1024, NULL, NULL);

    printf("%s\n", body);

}
