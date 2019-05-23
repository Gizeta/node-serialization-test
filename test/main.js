const assert = require('assert');
const serializers = require('../');

Object.values(serializers).forEach((Serialzier) => {
  describe(`${Serialzier.type} serialize test`, () => {
    function serializeTest(val, deep = false) {
      (deep ? assert.deepEqual : assert.equal)(val, Serialzier.fromBuffer(new Serialzier(val).toBuffer()).data);
    }

    it('should be same number after serialization', () => {
      serializeTest(1);
      serializeTest(99);
      serializeTest(-256);
      serializeTest(0);
  
      serializeTest(0.6);
      serializeTest(2.0);
      serializeTest(-1.333333);
    });
  
    it('should be same string after serialization', () => {
      serializeTest("test");
      serializeTest("中文\b\n\t中文");
      serializeTest("");
    });
  
    it('should be same boolean after serialization', () => {
      serializeTest(true);
      serializeTest(false);
    });

    it('should be null after serialization', () => {
      serializeTest(null);
    });
  
    it('should be same array after serialization', () => {
      serializeTest([], true);
      serializeTest([1, 2, 3], true);
      serializeTest([1, "2", 3.3, true], true);
      serializeTest([1, ["2", 3], 4.4, false], true);
    });
  
    it('should be same object after serialization', () => {
      serializeTest({}, true);
      serializeTest({ a: 1 }, true);
      serializeTest({ a: 1, b: "2", c: false }, true);
      serializeTest({ a: 1, "2": "2", c: [3.3, "4"] }, true);
      serializeTest({ a: 1, "b": "2", c: { d: {}, e: 3, f: 4.4 } }, true);
    });
  });
});
