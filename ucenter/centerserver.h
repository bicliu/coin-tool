#ifndef UCENTER_SERVER_H
#define UCENTER_SERVER_H

#include <event2/event.h>
#include <event2/buffer.h>
#include <event2/bufferevent.h>
#include <event2/listener.h>

class CenterServer{
    // libevent2
    struct event_base *base_;
    struct event *signal_event_;

public:
    CenterServer();
    ~CenterServer();
}

#endif // UCENTER_SERVER_H