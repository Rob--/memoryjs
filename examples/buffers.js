// https://github.com/deoxxa/concentrate - library to create buffer
const Concentrate = require('concentrate');

// https://github.com/deoxxa/dissolve - library to parse buffer
const Dissolve = require('dissolve');

const memoryjs = require('../index');
const processObject = memoryjs.openProcess('Testing.exe');
const structAddress = 0x00DDFDA8;

// Step 1: Create a generic struct to write
const vector = {
  x: 1.23,
  y: 4.56,
  z: 7.89,
};

// Step 2: Create a buffer from the generic struct
const writeBuffer = Concentrate()
  .floatle(vector.x)
  .floatle(vector.y)
  .floatle(vector.z)
  .result();

// Step 3: Write the buffer to memory
memoryjs.writeBuffer(processObject.handle, structAddress, writeBuffer);

// Step 4: Read the buffer from memory that we've just written to memory, remember to supply the correct buffer length
const readBuffer = memoryjs.readBuffer(processObject.handle, structAddress, writeBuffer.length);

// Step 5: Create the buffer parser using the `dissolve` library
var parser = Dissolve().loop(function(end) {
  this
    .floatle("x")
    .floatle("y")
    .floatle("z")
    .tap(function() {
      this.push(this.vars);
      this.vars = {};
    });
});
  
parser.on("readable", function() {
  let e;
  while (e = parser.read()) {
    console.log(e);
  }
});

// Step 6: Write the buffer to the parser to consume
parser.write(readBuffer);