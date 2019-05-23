class JSONSerializer {
  static get type() {
    return 'JSON';
  }

  static fromBuffer(buffer) {
    return new JSONSerializer(JSON.parse(buffer.toString()));
  }

  constructor (data) {
    this.data_ = data;
  }

  get data() {
    return this.data_;
  }

  toBuffer() {
    return Buffer.from(JSON.stringify(this.data_));
  }
}

module.exports = JSONSerializer;
