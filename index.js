module.exports = {
  JSONSerializer: require('./lib/json'),
  V8Serializer: require('./lib/v8'),
  FlexBufferNativeSerializer: require('./lib/flexbuffer-native'),
  MessagePackSerializer: require('./lib/msgpack'),
  MessagePackNativeSerializer: require('./lib/msgpack-native')
};
