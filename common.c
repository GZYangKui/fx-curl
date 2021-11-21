//
// Created by yangkui on 2021/11/21.
//
#include "include/common.h"

extern gchararray fx_trf_stack_to_dump(gchararray src) {
    gchararray des = g_malloc(strlen(src) + 1);
    strcpy(des, src);
    return des;
}
