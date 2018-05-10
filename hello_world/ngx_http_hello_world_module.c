#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>

/* 命令结构 */
static ngx_commond_t ngx_http_hello_module_commands[] = {
    { ngx_string("hello_world"),
        NGX_HTTP_LOC_CONF|NGX_CONF_NOARGS,
        ngx_http_hello_world,
        0,
        0,
        NULL },
    ngx_null_command
}

/* 上下文结构 */
static ngx_http_module_t ngx_http_world_module_ctx = {
    NULL,                                  /* preconfiguration */
    NULL,                                  /* postconfiguration */
    NULL,                                  /* create main configuration */
    NULL,                                  /* init main configuration */
    NULL,                                  /* create server configuration */
    NULL,                                  /* merge server configuration */
    NULL,                                  /* create location configuration */
    NULL                                   /* merge location configuration */
};

/* 定义模块 */
ngx_module_t ngx_http_hello_world_module = {
    NGX_MODULE_V1,
    &ngx_http_hello_world_module_ctx,      /* module context */
    ngx_http_hello_world_commands,         /* module directives,定义模块的配置文件参数，作用在nginx.conf文件解析  */
    NGX_HTTP_MODULE,                       /* 表示这是一个HTTP模块 */
    NULL,                                  /* init master */
    NULL,                                  /* init module */
    NULL,                                  /* init process */
    NULL,                                  /* init thread */
    NULL,                                  /* exit thread */
    NULL,                                  /* exit process */
    NULL,                                  /* exit master */
    NGX_MODULE_V1_PADDING
};



/* 回调函数 */
static char * ngx_http_hello_world(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    ngx_http_core_loc_conf_t *clcf;
    clcf = ngx_http_conf_get_module_loc_conf(cf, ngx_http_core_module);
    clcf->handler = ngx_http_hello_world_handler;
    return NGX_CONF_OK;
}

/* 具体处理方法handler */
static ngx_int_t ngx_http_test_handler(ngx_http_request_t *r)
{
    ngx_int_t rc = ngx_http_discard_request_body(r);
    if (rc != NGX_OK) {
        return rc;
    }

    ngx_str_t type = ngx_string("text/plain");
    ngx_str_t response = ngx_string("Hello World");
    r->headers_out.status = NGX_HTTP_OK;
    r->headers_out.content_length_n = response.len;
    r->headers_out.content_type = type;

    rc = ngx_http_send_header(r); // 发送头部
    if (rc == NGX_ERROR || rc > NGX_OK || r->header_only) {
        return rc;
    }

    ngx_buf_t *b;
    b = ngx_create_temp_buf(r->pool, response.len); //异步发送，要用堆内存空间
    if (b == NULL) {
        return NGX_HTTP_INTERNAL_SERVER_ERROR;
    }

    ngx_memcpy(b->pos, response.data, response.len);
    b->last = b.pos + response.len;
    b->last_buf = 1;

    ngx_chain_t out;
    out.buf = b;
    out.next = NULL;

    return ngx_http_output_filter(r, &out); // 向用户发送响应包
}
