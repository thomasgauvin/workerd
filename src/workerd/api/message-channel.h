// Copyright (c) 2017-2022 Cloudflare, Inc.
// Licensed under the Apache 2.0 license found in the LICENSE file or at:
//     https://opensource.org/licenses/Apache-2.0

#pragma once

#include <workerd/api/basics.h>
#include <workerd/jsg/jsg.h>

#include <kj/array.h>

namespace workerd::api {

// Implements MessagePort web-spec
// Ref: https://html.spec.whatwg.org/multipage/web-messaging.html#message-ports
class MessagePort: public jsg::Object {
 public:
  MessagePort() = default;
  KJ_DISALLOW_COPY(MessagePort);

  static jsg::Ref<MessagePort> constructor();

  struct StructuredSerializeOptions {
    kj::Array<jsg::Object> transfer{};

    JSG_STRUCT(transfer);
  };

  void postMessage(jsg::Lock& js,
      jsg::Value message,
      kj::OneOf<kj::Maybe<StructuredSerializeOptions>, kj::Array<jsg::Value>> options);
  void start(jsg::Lock& js);
  void stop(jsg::Lock& js);

  using OnmessageHandler = jsg::Function<void(jsg::Object)>;
  using OnmessageerrorHandler = jsg::Function<void(jsg::Object)>;

  kj::Maybe<OnmessageHandler> getOnmessage() {
    KJ_IF_SOME(handler, onmessage) {
      return handler;
    }
    return kj::none;
  }

  void setOnMessage(OnmessageHandler handler) {
    onmessage = handler;
  }

  kj::Maybe<OnmessageHandler> getOnmessageerror() {
    KJ_IF_SOME(handler, onmessageerror) {
      return handler;
    }
    return kj::none;
  }

  void setOnMessageerror(OnmessageHandler handler) {
    onmessageerror = handler;
  }

  JSG_RESOURCE_TYPE(MessagePort) {
    JSG_NESTED_TYPE(EventTarget);
    JSG_METHOD(postMessage);
    JSG_METHOD(start);
    JSG_METHOD(stop);

    JSG_INSTANCE_PROPERTY(onmessage, getOnmessage, setOnmessage);
    JSG_INSTANCE_PROPERTY(onmessageerror, getOnmessageerror, setOnmessageerror);
  }

 private:
  bool receiving_messages = false;

  kj::Maybe<OnmessageHandler> onmessage;
  kj::Maybe<OnmessageerrorHandler> onmessageerror;
};

// Implements MessageChannel web-spec
// Ref: https://html.spec.whatwg.org/multipage/web-messaging.html#message-channels
class MessageChannel: public jsg::Object {
 public:
  explicit MessageChannel(jsg::Lock& js);

  static jsg::Ref<MessageChannel> constructor(jsg::Lock& js);

  jsg::Ref<MessagePort> getPort1() {
    return port1.addRef();
  }

  jsg::Ref<MessagePort> getPort2() {
    return port2.addRef();
  }

  JSG_RESOURCE_TYPE(MessageChannel) {
    JSG_READONLY_PROTOTYPE_PROPERTY(port1, getPort1);
    JSG_READONLY_PROTOTYPE_PROPERTY(port2, getPort2);
  }

  void visitForMemoryInfo(jsg::MemoryTracker& tracker) const {
    tracker.trackField("port1", port1);
    tracker.trackField("port2", port2);
  }

 private:
  jsg::Ref<MessagePort> port1;
  jsg::Ref<MessagePort> port2;
};

#define EW_MESSAGE_CHANNEL_ISOLATE_TYPES                                                           \
  api::MessageChannel, api::MessagePort, api::MessagePort::StructuredSerializeOptions

}  // namespace workerd::api
