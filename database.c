//
// Created by yangkui on 2021/11/17.
//

#include <stdio.h>
#include "include/common.h"
#include "include/sqlite3.h"
#include "include/database.h"

static sqlite3 *context;

static gboolean check_db_struct(gpointer *userData);

extern FXWindowContext *splashWinContext;

extern gpointer fx_init_sqlite(gpointer userData) {
    gint rs = sqlite3_open("./data/fxcurl.db", &context);
    gboolean ok = (rs == SQLITE_OK);
    if (!ok) {
        printf("sqlite open failed=%d\n", rs);
    } else {
        ok = check_db_struct(userData);
    }
    QueueMessage *msg = ALLOC_QUEUE_MSG;
    msg->code = QUEUE_MSG_OK;
    msg->message = "初始化成功";

    g_async_queue_push(splashWinContext->asyncQueue,msg);
}

extern void fx_shutdown_sqlite3() {
    if (context == NULL) {
        return;
    }
    gint ok = sqlite3_shutdown();
    gchar text[255];
    memset(text, 0, 255);
    gchar *pointer = text;
    if (ok == SQLITE_OK) {
        strncpy(pointer, "Success shutdown sqlite database!", 254);
    } else {
        sprintf(pointer, "Failed shutdown sqlite database failed code:%d", ok);
    }
    printf("%s\n", text);
}

/**
 * 检查数据库表结构
 */
static gboolean check_db_struct(gpointer *data) {

    return TRUE;
}





