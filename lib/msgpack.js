const msgpack = require('msgpack-lite');

class MessagePackSerializer {
  static get type() {
    return 'MessagePack';
  }

  static fromBuffer(buffer) {
    return new MessagePackSerializer(msgpack.decode(buffer));
  }

  constructor (data) {
    this.data_ = data;
  }

  get data() {
    return this.data_;
  }

  toBuffer() {
    return msgpack.encode(this.data_);
  }
}

module.exports = MessagePackSerializer;
