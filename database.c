//
// Created by yangkui on 2021/11/17.
//

#include <stdio.h>
#include "include/sql_ast.h"
#include "include/common.h"
#include "include/sqlite3.h"
#include "include/database.h"

static sqlite3 *context;


static gboolean check_db_struct();


extern gboolean fx_init_sqlite(gpointer userData, GError **error) {
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
    if (!ok) {
        *error = g_error_new(0, FALSE, "数据库初始化失败!");
    }
    return ok;
}

extern gboolean select_node_by_parent_id(gint id, GList *list) {
    sqlite3_stmt *preStmt = NULL;
    gchararray sql = "SELECT * FROM node_tree WHERE parent_id=?";
    sqlite3_prepare_v3(context, sql, (gint) strlen(sql), SQLITE_PREPARE_PERSISTENT, &preStmt, NULL);
    sqlite3_bind_int(preStmt, 1, id);

    gint count = 0;
    while (sqlite3_step(preStmt) == SQLITE_ROW) {
        NodeTree *nodeTree = g_malloc(sizeof(NodeTree));
        nodeTree->id = sqlite3_column_int(preStmt, 0);
        nodeTree->type = sqlite3_column_int(preStmt, 1);
        nodeTree->parentId = sqlite3_column_int(preStmt, 2);

        gchararray name = sqlite3_column_text(preStmt, 3);

        if (name != NULL) {
            nodeTree->name = fx_trf_stack_to_dump(name);
        } else {
            nodeTree->name = NULL;
        }

        if (count == 0) {
            list->data = nodeTree;
        } else {
            list = g_list_append(list, nodeTree);
        }
        count++;
    }

    sqlite3_finalize(preStmt);
}

extern gboolean insert_tree_node(gint *id, gint parentId, gint type, gchararray name) {
    sqlite3_stmt *preStmt = NULL;
    gchararray sql = "INSERT INTO node_tree(type,parent_id,name,create_time) VALUES(?,?,?,?)";
    sqlite3_prepare_v3(
            context,
            sql,
            (gint) strlen(sql),
            SQLITE_PREPARE_PERSISTENT,
            &preStmt,
            NULL
    );

    sqlite3_bind_int(preStmt, 1, type);
    sqlite3_bind_int(preStmt, 2, parentId);
    sqlite3_bind_text(preStmt, 3, name, -1, SQLITE_STATIC);
    sqlite3_bind_int64(preStmt, 4, fx_get_timestamp());

    //执行sql语句
    gint code = sqlite3_step(preStmt);
    gboolean ok = (code == SQLITE_DONE);
    if (!ok) {
        *id = code;
    }
    sqlite3_finalize(preStmt);

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
            if (sql == NULL) {
                continue;
            }
            gint rs = sqlite3_exec(context, sql, NULL, NULL, &errMsg);
            if (rs != SQLITE_OK) {
                success = FALSE;
                printf("数据库初始化失败:%s,错误sql语句:%s\n", errMsg, sql);
                break;
            } else {
                printf("sql语句执行成功\n");
            }
        }
    }
    g_list_free(list);
    return success;
}





