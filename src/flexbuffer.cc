#include "flexbuffer.h"

FlexBuffer::FlexBuffer(Env* env) {
  env_ = env;
  isInteger_ = env_->Global().Get("Number").As<Object>().Get("isInteger").As<Function>();
}

#pragma region serialize

Value FlexBuffer::serialize(const Value& val) {
  fbb_.Clear();
  serializeImpl(val);
  if (fbb_.GetSize() == 0 && fbb_.StartVector() == 0) { // empty content
    return env_->Undefined();
  }
  fbb_.Finish();
  return Buffer<uint8_t>::Copy(*env_, (uint8_t*)fbb_.GetBuffer().data(), fbb_.GetSize());
}

void FlexBuffer::serializeImpl(const Value& val) {
  switch (val.Type()) {
    case napi_valuetype::napi_boolean : {
      fbb_.Bool(val.As<Boolean>().Value());
      break;
    }
    case napi_valuetype::napi_null :
    case napi_valuetype::napi_undefined : {
      fbb_.Null();
      break;
    }
    case napi_valuetype::napi_number : {
      fbb_.Double(val.As<Number>().DoubleValue());
      break;
    }
    case napi_valuetype::napi_object : {
      if (val.IsArray()) {
        fbb_.Vector([&] {
          auto arr = val.As<Array>();
          unsigned int len = arr.Length();
          for (unsigned int i = 0; i < len; i++) {
            serializeImpl(arr.Get(i));
          }
        });
      } else {
        fbb_.Map([&] {
          auto obj = val.As<Object>();
          Array names = obj.GetPropertyNames();
          unsigned int len = names.Length();
          for (unsigned int i = 0; i < len; i++) {
            auto key = names.Get(i);
            if (key.IsNumber()) {
              key = key.ToString();
            }
            serializeImpl(obj.Get(key), key.As<String>().Utf8Value().c_str());
          }
        });
      }
      break;
    }
    case napi_valuetype::napi_string : {
      fbb_.String(val.As<String>().Utf8Value());
      break;
    }
  }
}

void FlexBuffer::serializeImpl(const Value& val, const char* key) {
  switch (val.Type()) {
    case napi_valuetype::napi_boolean : {
      fbb_.Bool(key, val.As<Boolean>().Value());
      break;
    }
    case napi_valuetype::napi_null :
    case napi_valuetype::napi_undefined : {
      fbb_.Null(key);
      break;
    }
    case napi_valuetype::napi_number : {
      auto num = val.As<Number>();
      //if (isInteger_.Call({ num }).ToBoolean().Value()) {
      //  fbb_.Int(key, num.Int64Value());
      //} else {
        fbb_.Double(key, num.DoubleValue());
      //}
      break;
    }
    case napi_valuetype::napi_object : {
      if (val.IsArray()) {
        fbb_.Vector(key, [&] {
          auto arr = val.As<Array>();
          unsigned int len = arr.Length();
          for (unsigned int i = 0; i < len; i++) {
            serializeImpl(arr.Get(i));
          }
        });
      } else {
        fbb_.Map(key, [&] {
          auto obj = val.As<Object>();
          Array names = obj.GetPropertyNames();
          unsigned int len = names.Length();
          for (unsigned int i = 0; i < len; i++) {
            Value key = names.Get(i);
            if (key.IsNumber()) {
              key = key.ToString();
            }
            serializeImpl(obj.Get(key), key.As<String>().Utf8Value().c_str());
          }
        });
      }
      break;
    }
    case napi_valuetype::napi_string : {
      fbb_.String(key, val.As<String>().Utf8Value());
      break;
    }
  }
}

#pragma endregion

#pragma region deserialize

Value FlexBuffer::deserialize(const uint8_t* buf, const size_t size) {
  return deserializeImpl(flexbuffers::GetRoot(buf, size));
}

Value FlexBuffer::deserializeImpl(const flexbuffers::Reference fb) {
  switch (fb.GetType()) {
    case flexbuffers::Type::FBT_INT : {
      return Number::New(*env_, fb.AsInt64());
    }
    case flexbuffers::Type::FBT_FLOAT : {
      return Number::New(*env_, fb.AsDouble());
    }
    case flexbuffers::Type::FBT_BOOL : {
      return Boolean::New(*env_, fb.AsBool());
    }
    case flexbuffers::Type::FBT_STRING : {
      return String::New(*env_, fb.AsString().str());
    }
    case flexbuffers::Type::FBT_NULL : {
      return env_->Null();
    }
    case flexbuffers::Type::FBT_VECTOR : {
      auto arr = Array::New(*env_);
      flexbuffers::Vector vec = fb.AsVector();
      size_t len = vec.size();
      for (size_t i = 0; i < len; i++) {
        arr[i] = deserializeImpl(vec[i]);
      }
      return arr;
    }
    case flexbuffers::Type::FBT_MAP : {
      auto obj = Object::New(*env_);
      flexbuffers::Map map = fb.AsMap();
      flexbuffers::TypedVector keys = map.Keys();
      size_t len = keys.size();
      for (size_t i = 0; i < len; i++) {
        auto key = keys[i].AsKey();
        obj.Set(key, deserializeImpl(map[key]));
      }
      return obj;
    }
  }
  return env_->Undefined();
}

#pragma endregion
