//
// Created by yangkui on 2021/11/17.
//

#ifndef FXCURL_COMMON_H
#define FXCURL_COMMON_H

#define FX_RESOURCE_PREFIX "/cn/navclub/"
#define FX_RESOURCE_UI_PREFIX "/cn/navclub/ui/"
#define FX_RESOURCE_IMG_PREFIX "/cn/navclub/img/"
#define FX_RESOURCE_CSS_PREFIX "/cn/navclub/css/"

/**
 * 通过宏拼接资源前缀
 */
#define GET_INNER_RESOURCE_PATH(resource) \
      FX_RESOURCE_PREFIX#resource

#define GET_INNER_UI_RESOURCE(resource) \
    FX_RESOURCE_UI_PREFIX#resource

#define GET_INNER_IMG_RESOURCE(resource) \
     FX_RESOURCE_IMG_PREFIX#resource

#define GET_INNER_CSS_RESOURCE(resource) \
     FX_RESOURCE_CSS_PREFIX#resource

#endif //FXCURL_COMMON_H
