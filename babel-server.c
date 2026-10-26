// babel viewer · minimal static file server (mongoose, 裁剪版)
// usage: babel-server [port] [dir]   (defaults: 8000 .)
#include "mongoose.h"

static void fn(struct mg_connection *c, int ev, void *ev_data) {
    if (ev == MG_EV_HTTP_MSG) {
        struct mg_http_serve_opts opts = {.root_dir = (const char *) c->fn_data, .enable_dir_listing = true};
        mg_http_serve_dir(c, ev_data, &opts);   /* 目录列表随 MG_ENABLE_DIRECTORY_LISTING 生效 */
    }
}

int main(int argc, char *argv[]) {
    const char *port = argc > 1 ? argv[1] : "8000";
    const char *dir  = argc > 2 ? argv[2] : ".";
    struct mg_mgr mgr;
    mg_log_set(MG_LL_NONE);                     /* 裁掉日志 */
    mg_mgr_init(&mgr);
    if (mg_http_listen(&mgr, port, fn, (void *) dir) == NULL) {
        fprintf(stderr, "listen on %s failed\n", port);
        return 1;
    }
    fprintf(stderr, "serving %s at http://127.0.0.1:%s\n", dir, port);
    for (;;) mg_mgr_poll(&mgr, 1000);
    return 0;
}
