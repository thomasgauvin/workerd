#include "message-channel.h"

#include <workerd/jsg/jsg.h>

namespace workerd::api {

jsg::Ref<MessagePort> MessagePort::constructor() {
  return jsg::alloc<MessagePort>();
}

void MessagePort::postMessage(jsg::Lock &js,
    jsg::Value message,
    kj::OneOf<kj::Maybe<StructuredSerializeOptions>, kj::Array<jsg::Value>> options) {
  // Implement this
}

void MessagePort::start(jsg::Lock &js) {
  receiving_messages = true;
}

void MessagePort::stop(jsg::Lock &js) {
  receiving_messages = false;
}

MessageChannel::MessageChannel(jsg::Lock &js)
    : port1(MessagePort::constructor()),
      port2(MessagePort::constructor()) {}

jsg::Ref<MessageChannel> MessageChannel::constructor(jsg::Lock &js) {
  return jsg::alloc<MessageChannel>(js);
}

}  // namespace workerd::api