#include "napi.h"
#include "flexbuffer.h"

using namespace Napi;

Value FlexBuffer_Serialize(const CallbackInfo& info) {
  Env env = info.Env();
  auto serializer = new FlexBuffer(&env);
  return serializer->serialize(info[0]);
}

Value FlexBuffer_Deserialize(const CallbackInfo& info) {
  Env env = info.Env();
  auto buf = info[0].As<Buffer<uint8_t>>();
  auto serializer = new FlexBuffer(&env);
  return serializer->deserialize(buf.Data(), buf.ByteLength());
}

Object Init(Env env, Object exports) {
  Object flexbuffer = Object::New(env);
  flexbuffer.Set(String::New(env, "serialize"), Function::New(env, FlexBuffer_Serialize));
  flexbuffer.Set(String::New(env, "deserialize"), Function::New(env, FlexBuffer_Deserialize));

  exports.Set(String::New(env, "FlexBuffer"), flexbuffer);

  return exports;
}

NODE_API_MODULE(node_serialize_lib, Init)
