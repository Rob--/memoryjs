const memoryjs = require('./index');
const processName = 'Test Project.exe';

const processObject = memoryjs.openProcess(processName);
console.log(JSON.stringify(processObject, null, 3));

const modules = memoryjs.getModules(processObject.th32ProcessID);
modules.forEach(module => console.log(module));

/* How to read a "string"
 * C++ code:
 * ```
 * std::string str = "this is a sample string";
 * std::cout << "Address: " << (DWORD) str.c_str() << ", value: " << str.c_str() << std::endl;
 * ```
 * This will create a string and the address of the string and the value of the string.
 */

const str = memoryjs.readMemory(0x69085bc0, memoryjs.STRING);
console.log(str); // "this is a sample string";

/* How to read and write vectors
 * C++ code:
 * ```
 * struct Vector3 { float x, y, z;    };
 * struct Vector4 { float w, x, y, z; };
 * Vector3 vec3 = { 1.0f, 2.0f, 3.0f        };
 * Vector4 vec4 = { 1.0f, 2.0f, 3.0f, 4.0f  };
 * std::cout << "[Vec3] Address: " << &vec3 << std::endl;
 * std::cout << "[Vec4] Address: " << &vec4 << std::endl;
 */

let vec3 = {
  x: 0, y: 0, z: 0,
};
memoryjs.writeMemory(0x000001, vec3, memoryjs.VEC3);
vec3 = memoryjs.readMemory(0x000001, memoryjs.VEC3); // { x, y, z }
console.log(vec3);

let vec4 = {
  w: 0, x: 0, y: 0, z: 0,
};
memoryjs.writeMemory(0x000002, vec4, memoryjs.VEC4);
vec4 = memoryjs.readMemory(0x000002, memoryjs.VEC4); // { w, x, y, z }
console.log(vec4);

memoryjs.closeProcess(processObject.handle);
