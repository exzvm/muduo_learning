#include "Poller.h"
#include "Channel.h"

Poller::Poller(EventLoop *loop)
  : ownerLoop_(loop)
{
}

bool Poller::hasChannel(Channel *channel) const 
{
  auto it = channels_.find(channel->fd());
  return it != channels_.end() && it->second == channel;
}

// 基类不能引用派生类
/*
#include "PollPoller.h"
#include "EpollPoller.h"
Poller* newDefaultPoller(EventLoop* loop)
{

}
*/