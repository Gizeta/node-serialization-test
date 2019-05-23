const { FlexBuffer } = require('bindings')('node-serialize-lib');

class FlexBufferNativeSerializer {
  static get type() {
    return 'FlexBuffer(native)';
  }

  static fromBuffer(buffer) {
    return new FlexBufferNativeSerializer(FlexBuffer.deserialize(buffer));
  }

  constructor (data) {
    this.data_ = data;
  }

  get data() {
    return this.data_;
  }

  toBuffer() {
    return FlexBuffer.serialize(this.data_);
  }
}

module.exports = FlexBufferNativeSerializer;
