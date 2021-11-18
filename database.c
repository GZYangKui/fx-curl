//
// Created by yangkui on 2021/11/17.
//

#include <stdio.h>
#include "include/sqlite3.h"
#include "include/database.h"

static sqlite3 *context;

static gpointer check_db_struct(gpointer *userData);


extern gboolean fx_init_sqlite() {
    int rs = sqlite3_open("./data/fxcurl.db", &context);
    gboolean ok = (rs == SQLITE_OK);
    if (!ok) {
        printf("sqlite open failed=%d\n", rs);
    } else {
        //开辟新线程检查数据库是否完整
        g_thread_new("check-db_struct-thread", check_db_struct, NULL);
    }
    return ok;
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
static gpointer check_db_struct(gpointer *data) {

}





