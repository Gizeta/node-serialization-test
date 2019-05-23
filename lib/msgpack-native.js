const msgpack = require('msgpack');

class MessagePackNativeSerializer {
  static get type() {
    return 'MessagePack(native)';
  }

  static fromBuffer(buffer) {
    return new MessagePackNativeSerializer(msgpack.unpack(buffer));
  }

  constructor (data) {
    this.data_ = data;
  }

  get data() {
    return this.data_;
  }

  toBuffer() {
    return msgpack.pack(this.data_);
  }
}

module.exports = MessagePackNativeSerializer;
