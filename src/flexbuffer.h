#ifndef NODE_SERIALIZE_LIB_FLEXBUFFER_H
#define NODE_SERIALIZE_LIB_FLEXBUFFER_H

#include "flatbuffers/flexbuffers.h"

#pragma region NAPI
#ifdef USE_NAPI

#include "napi.h"
using namespace Napi;

class FlexBuffer {
  public:
    FlexBuffer(Env* env);
    Value serialize(const Value& val);
    Value deserialize(const uint8_t* buf, const size_t size);
  private:
    void serializeImpl(const Value& val);
    void serializeImpl(const Value& val, const char* key);
    Value deserializeImpl(const flexbuffers::Reference fb);

    flexbuffers::Builder fbb_;
    Env* env_;
    Function isInteger_;
};

#endif
#pragma endregion

#pragma region NAN
#ifdef USE_NAN

#include "nan.h"
using namespace Nan;

class FlexBuffer {
  public:
    FlexBuffer();
    v8::Local<v8::Value> serialize(const v8::Local<v8::Value>& val);
    v8::Local<v8::Value> deserialize(const uint8_t* buf, const size_t size);
  private:
    void serializeImpl(const v8::Local<v8::Value>& val);
    void serializeImpl(const v8::Local<v8::Value>& val, const char* key);
    v8::Local<v8::Value> deserializeImpl(const flexbuffers::Reference fb);

    flexbuffers::Builder fbb_;
};

#endif
#pragma endregion

#endif
