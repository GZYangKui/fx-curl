//
// Created by yangkui on 2021/11/26.
//

#include "include/http_util.h"
#include <libsoup/soup.h>

void send_request(gchararray url, HttpRequestMethod method) {
    assert(url!=NULL);

    SoupSession *session = soup_session_new();
    SoupMessage *msg = soup_message_new(fx_get_request_method_str(method), url);

    soup_session_send(session, msg, NULL, NULL);


    printf("响应状态码:%d\n", msg->status_code);
}
