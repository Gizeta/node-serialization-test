const Benchmark = require('benchmark');
const data = require('./data');
const serializers = require('..');

const cache = new Map();

console.log('Serialize Test');
const serializeTest = new Benchmark.Suite();
Object.values(serializers).forEach((Serialize) => {
  serializeTest.add(Serialize.type, () => {
    const buf = new Serialize(data).toBuffer();
    cache.set(`${Serialize.type}_serialized`, buf);
  });
});
serializeTest
  .on('cycle', function (event) {
    console.log(String(event.target));
  })
  .on('complete', function () {
    console.log('Fastest is ' + this.filter('fastest').map('name'));
  })
  .run();

console.log('----------------');

console.log('Deserialize Test');
const deserializeTest = new Benchmark.Suite();
Object.values(serializers).forEach((Serialize) => {
  const buf = cache.get(`${Serialize.type}_serialized`);
  deserializeTest.add(Serialize.type, () => {
    Serialize.fromBuffer(buf);
  });
});
deserializeTest
  .on('cycle', function (event) {
    console.log(String(event.target));
  })
  .on('complete', function () {
    console.log('Fastest is ' + this.filter('fastest').map('name'));
  })
  .run();
