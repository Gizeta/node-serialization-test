#ifndef NODE_SERIALIZE_LIB_FLEXBUFFER_H
#define NODE_SERIALIZE_LIB_FLEXBUFFER_H

#include "flatbuffers/flexbuffers.h"
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
