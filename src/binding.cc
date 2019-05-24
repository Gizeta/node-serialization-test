#include "flexbuffer.h"

#pragma region NAPI
#ifdef USE_NAPI

#include "napi.h"
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

#endif
#pragma endregion

#pragma region NAN
#ifdef USE_NAN

#include "nan.h"
using namespace Nan;

NAN_METHOD(FlexBuffer_Serialize) {
  auto serializer = new FlexBuffer();
  info.GetReturnValue().Set(serializer->serialize(info[0]));
}

NAN_METHOD(FlexBuffer_Deserialize) {
  unsigned char* buf = (unsigned char*)node::Buffer::Data(info[0]->ToObject(Nan::GetCurrentContext()).FromMaybe(v8::Local<v8::Object>()));
  unsigned int size = info[1]->Uint32Value(Nan::GetCurrentContext()).FromJust();
  auto serializer = new FlexBuffer();
  v8::Local<v8::Value> ret = serializer->deserialize(buf, size);
  info.GetReturnValue().Set(ret);
}

NAN_MODULE_INIT(Init) {
  v8::Local<v8::Object> flexbuffer = New<v8::Object>();
  Set(
    flexbuffer,
    New<v8::String>("serialize").ToLocalChecked(),
    New<v8::FunctionTemplate>(FlexBuffer_Serialize)->GetFunction()
  );
  Set(
    flexbuffer,
    New<v8::String>("deserialize").ToLocalChecked(),
    New<v8::FunctionTemplate>(FlexBuffer_Deserialize)->GetFunction()
  );

  Set(
    target,
    New<v8::String>("FlexBuffer").ToLocalChecked(),
    flexbuffer
  );
}

NODE_MODULE(node_serialize_lib, Init)

#endif
#pragma endregion
