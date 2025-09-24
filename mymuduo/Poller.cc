#include "Poller.h"
#include "Channel.h"

Poller::Poller(EventLoop *loop)
  : ownerLoop_(loop)
{
}

Poller::~Poller() = default;  // 必须写，否则链接错误

bool Poller::hasChannel(Channel *channel) const 
{
  auto it = channels_.find(channel->fd());
  return it != channels_.end() && it->second == channel;
}
