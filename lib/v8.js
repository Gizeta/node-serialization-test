const v8 = require('v8');

class V8Serializer {
  static get type() {
    return 'v8';
  }

  static fromBuffer(buffer) {
    return new V8Serializer(v8.deserialize(buffer));
  }

  constructor (data) {
    this.data_ = data;
  }

  get data() {
    return this.data_;
  }

  toBuffer() {
    return v8.serialize(this.data_);
  }
}

module.exports = V8Serializer;
