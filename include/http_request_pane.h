//
// Created by yangkui on 2021/11/23.
//

#ifndef FXCURL_HTTP_REQUEST_PANE_H
#define FXCURL_HTTP_REQUEST_PANE_H

#include "common.h"

/**
 * 代表一个Http请求tab
 */
typedef struct {
    /************************************
     *               UI数据              *
     ************************************/
    GtkWidget *pTV;
    GtkWidget *hTV;
    GtkWidget *body;
    GtkWidget *respStack;
    //整个widget面板
    GtkWidget *content;
    /*************************************
     *               业务数据              *
     *************************************/
    //当前数据对应数据库id
    gint64 id;
    //所属最外层notebook中位置
    gint index;
    //请求notebook中位置
    gint current;
    //http请求url
    gchararray url;
    //请求方法
    HttpRequestMethod method;
} HttpRequestPane;


/**
 *
 * 创建一个新的Http请求面板
 *
 */
extern HttpRequestPane *http_request_pane_new(gint64 id);

#endif //FXCURL_HTTP_REQUEST_PANE_H
