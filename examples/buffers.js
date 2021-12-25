/**
 * This example uses the following structs defined in C++:
  struct vector {
    float x, y, z;
  };

  struct player {
    vector position;
    double health;
    std::string name;
    float distance;
    bool alive;
  };
*/

// https://github.com/LordVonAdel/structron
const Struct = require('structron');
const memoryjs = require('../index');

const processObject = memoryjs.openProcess('Testing.exe');
const structAddress = 0x000000DEADBEEF;

// -- Step 1: define the structures

// Custom double consumer/producer (since it's not yet implemented in `structron`)
const double = {
  read(buffer, offset) {
    return buffer.readDoubleLE(offset);
  },
  write(value, context, offset) {
    context.buffer.writeDoubleLE(value, offset);
  },
  SIZE: 8,
};

// Use string consumer/producer provided by the library (custom implementation for `std::string`),
// pass process handle and base address of structure so the library can read/write the string,
// also requires passing the platform architecture to determine the structure of `std::string`
const string = memoryjs.STRUCTRON_TYPE_STRING(processObject.handle, structAddress, '64');

// Define vector structure
const Vector = new Struct()
  .addMember(Struct.TYPES.FLOAT, 'x') // 4 bytes
  .addMember(Struct.TYPES.FLOAT, 'y') // 4 bytes
  .addMember(Struct.TYPES.FLOAT, 'z'); // 4 bytes

// Define player structure
const Player = new Struct()
  .addMember(Vector, 'position') // 12 bytes
  .addMember(Struct.TYPES.SKIP(4), 'unused') // compiler padding to put member on 8 byte boundary
  .addMember(double, 'health') // 8 bytes
  .addMember(string, 'name') // 32 bytes (in 64bit process, 24 bytes in 32bit process)
  .addMember(Struct.TYPES.FLOAT, 'distance') // 4 bytes
  .addMember(Struct.TYPES.BYTE, 'alive'); // 1 byte

// -- Step 2: create object to write to memory
const object = {
  position: {
    x: 1.23, y: 4.56, z: 7.89,
  },
  health: 80.12,
  name: 'Example Name 1234567890',
  distance: 4.20,
  alive: false,
};

// -- Step 3: create buffer from object and write to memory
let context = Player.write(object);
memoryjs.writeBuffer(processObject.handle, structAddress, context.buffer);

// -- Step 4: read buffer from memory and parse
const buffer = memoryjs.readBuffer(processObject.handle, structAddress, context.buffer.length);

context = Player.readContext(buffer);

if (!context.hasErrors()) {
  console.log(context.data);
}
