#include "centerserver.h"

CenterServer::CenterServer()
{
}

CenterServer::~CenterServer()
{
    event_base_free(base_);
}

void CenterServer::runHttpd() {
    struct evhttp_bound_socket *handle;
    struct evhttp *httpd;

    base_ = event_base_new();
    assert(base_ != nullptr);
    httpd = evhttp_new(base_);

    evhttp_set_allowed_methods(httpd, EVHTTP_REQ_GET | EVHTTP_REQ_POST | EVHTTP_REQ_HEAD);
    evhttp_set_timeout(httpd, 5 /* timeout in seconds */);

    /*evhttp_set_cb(httpd, "/",               CenterServer::httpdServerStatus, this);
    evhttp_set_cb(httpd, "/worker_status",  CenterServer::httpdGetWorkerStatus, this);
    evhttp_set_cb(httpd, "/worker_status/", CenterServer::httpdGetWorkerStatus, this);
    evhttp_set_cb(httpd, "/flush_db_time",  CenterServer::httpdGetFlushDBTime, this);*/

    handle = evhttp_bind_socket_with_handle(httpd, httpdHost_.c_str(), httpdPort_);
    if (!handle) {
        LOG(ERROR) << "couldn't bind to port: " << httpdPort_ << ", host: " << httpdHost_ << ", exiting.";
        return;
    }
    event_base_dispatch(base_);
}