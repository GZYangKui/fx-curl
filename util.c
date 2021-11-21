//
// Created by yangkui on 2021/11/20.
//

#include "include/util.h"

extern gint64 fx_get_timestamp() {
    GTimeZone *zero = g_time_zone_new_local();
    GDateTime *dateTime = g_date_time_new_now(zero);
    gint64 timestamp = g_date_time_to_unix(dateTime);

    g_time_zone_unref(zero);
    g_date_time_unref(dateTime);

    return timestamp*1000;
}

