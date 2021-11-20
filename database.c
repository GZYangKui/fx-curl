//
// Created by yangkui on 2021/11/17.
//

#include <stdio.h>
#include "include/sql_ast.h"
#include "include/common.h"
#include "include/sqlite3.h"
#include "include/database.h"

static sqlite3 *context;

extern FXWindowContext *splashWinContext;

static gboolean check_db_struct();


extern gpointer fx_init_sqlite(gpointer userData) {
    gchararray sqlitePath;
    if (PROJECT_PROFILE == DEV_PROFILE) {
        sqlitePath = "../data/fxcurl.db";
    } else {
        sqlitePath = "./data/fxcurl.db";
    }
    gint rs = sqlite3_open(sqlitePath, &context);
    gboolean ok = (rs == SQLITE_OK);
    if (!ok) {
        printf("sqlite open failed=%d\n", rs);
    } else {
        ok = check_db_struct();
    }
    QueuePayload *msg = ALLOC_QUEUE_PAYLOAD
    gchararray str = ok ? "初始化成功" : "初始化失败";
    TRA_DUMP_STR(str);

    msg->code = ok;
    msg->message = message;
    msg->type = CHECK_DB_CONFIG;
    msg->data = NULL;

    g_async_queue_push(splashWinContext->asyncQueue, msg);
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
static gboolean check_db_struct() {
    GList *list;
    gboolean success;
    list = g_list_alloc();
    success = get_init_sql_state(list);
    if (success) {
        guint len;
        gchararray sql;
        gchararray errMsg;
        len = g_list_length(list);
        for (int i = 0; i < len; i++) {
            sql = list->data;
            if (sql == NULL){
                continue;
            }
            gint rs = sqlite3_exec(context, sql, NULL, NULL, &errMsg);
            if (rs != SQLITE_OK) {
                success = FALSE;
                printf("数据库初始化失败:%s,错误sql语句:%s\n",errMsg,sql);
                break;
            }
        }
    }
    g_list_free(list);
    return success;
}





