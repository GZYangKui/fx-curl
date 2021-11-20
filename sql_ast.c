#include "include/sql_ast.h"

#define DEFAULT_AST_SIZE 1024
#define END_FILE_EXCEPTION -1
#define DEFAULT_LOAD_FACTOR 0.25
#define STATEMENT_END_SYMBOL ';'


typedef enum {
    //非注释行
    NONE,
    //单行注释
    SINGLE_LINE,
    //多行注释
    MULTI_LINE
} AstCommentType;

static gint isRowSymbol(gchar, gchar);

static AstCommentType is_comment_line(gchar, gchar);

static gint nextRowSymbol(const gchar *, gsize, gint, AstCommentType);


gboolean get_init_sql_state(GList *list) {
    gchararray scriptPath;
    if (PROJECT_PROFILE == DEV_PROFILE) {
        scriptPath = "../data/sql/fxcurl.sql";
    } else {
        scriptPath = "data/sql/fxcurl.sql";
    }
    gsize size;
    GFile *file;
    GBytes *bytes;
    file = g_file_new_for_path(scriptPath);

    if (file == NULL) {
        return FALSE;
    }

    bytes = g_file_load_bytes(file, NULL, NULL, NULL);

    gint count = 0;
    gchararray str = NULL;
    gchararray buff = NULL;
    str = (gchararray) g_bytes_get_data(bytes, &size);
    for (int i = 0; i < size - 1; ++i) {
        gchar a = *(str + i);
        gchar b = *(str + i + 1);
        //判断当前字符是否换行符号
        gint k = isRowSymbol(a, b);
        if (k >= 1) {
            i += (k - 1);
            continue;
        }
        AstCommentType type = is_comment_line(a, b);
        //单行或者多行注释
        if (type != NONE) {
            gint index = nextRowSymbol(str, size, i + 2, type);
            if (index == END_FILE_EXCEPTION) {
                return FALSE;
            }
            i = index;
            continue;
        }
        if ( (buff == NULL) || (count == DEFAULT_AST_SIZE - 2)) {
            if (buff == NULL) {
                buff = (gchararray)malloc(DEFAULT_AST_SIZE*sizeof (gchar));
                memset(buff, 0, DEFAULT_AST_SIZE);
            } else {
                gint extra = (gint) (count * DEFAULT_LOAD_FACTOR);
                buff = g_realloc(buff, extra);
                memset(buff, count, extra);
            }
        }
        *(buff + count) = a;
        if (b == STATEMENT_END_SYMBOL) {
            *(buff + count + 1) = b;
            if (list->data == NULL){
                list->data = buff;
            }else {
                list = g_list_append(list, buff);
            }
            count = 0;
            buff = NULL;
            i++;
        } else {
            count++;
        }
    }
    g_object_unref(file);
    return TRUE;
}

static AstCommentType is_comment_line(gchar a, gchar b) {
    if (a == '/' && b == '*') {
        return MULTI_LINE;
    }
    if (a == '-' && b == '-') {
        return SINGLE_LINE;
    }
    return NONE;
}

static gint isRowSymbol(gchar a, gchar b) {
    gboolean c = (a == '\n');
    gboolean d = ((b == '\r') && c);
    if (!c && !d) {
        return 0;
    }
    return d ? 2 : 1;
}

/**
 *
 * 查找下一个行号位置
 *
 */
static gint nextRowSymbol(const gchar *const bytes, gsize size, gint start, AstCommentType type) {
    gchar a;
    gchar b;
    gint pos = start;
    gint rsIndex = END_FILE_EXCEPTION;
    while (pos < size) {
        a = *(bytes + pos);
        b = *(bytes + pos + 1);
        if (type == SINGLE_LINE) {
            gint k = isRowSymbol(a, b);
            if (k >= 1) {
                rsIndex = pos + k - 1;
                break;
            }
        } else {
            if (a == '*' && b == '/') {
                rsIndex = pos + 2;
                break;
            }
        }
        pos++;
    }
    if (rsIndex > size - 1) {
        rsIndex = (gint) (size - 1);
    }
    return rsIndex;
}