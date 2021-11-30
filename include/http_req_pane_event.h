#ifndef HTTP_REQ_PAN_EVENT_H
#include "common.h"
#include "http_request_pane.h"

/*
 *
 * enum http request step status
 *
 */
typedef enum {
    REGULAR,
    LOADING,
    RESPONSE
}RequestStatus;

/**
 *
 * Update http request status
 *
 */
void update_http_request_status(HttpRequestPane *pane);

/**
 *
 * Cancel current http request
 *
 */
void fx_cancel_http_request(gpointer userData);


#endif