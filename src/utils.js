const SIZEOF_STDSTRING_32BIT = 24;
const SIZEOF_STDSTRING_64BIT = 32;
const STDSTRING_LENGTH_OFFSET = 0x10;

/**
 * Custom string consumer/producer for Structron (due to complexity of `std::string`)
 * `std::string` is a container for a string which makes reading/writing to it tricky,
 * it will either store the string itself, or a pointer to the string, based on the
 * length of the string. When we want to read from or write to a buffer, we need
 * to determine if the string is in the buffer itself, or if the buffer
 * just contains a pointer to the string. Based on one of these options,
 * we can read from or write to the string.
 *
 * @param handle the handle to the process
 * @param structAddress the base address of the structure in memory
 * @param platform the architecture of the process, either "32" or "64"
 * @param encoding the encoding type of the string
 */
const STRUCTRON_TYPE_STRING = memoryjs => (handle, structAddress, platform, encoding = 'utf8') => ({
  read(buffer, offset) {
    // get string length from `std::string` container
    const length = buffer.readUInt32LE(offset + STDSTRING_LENGTH_OFFSET);

    // if length > 15, `std::string` has a pointer to the string
    if (length > 15) {
      const pointer = platform === '64' ? buffer.readBigInt64LE(offset) : buffer.readUInt32LE(offset);
      return memoryjs.readMemory(handle, Number(pointer), memoryjs.STRING);
    }

    // if length <= 15, `std::string` directly contains the string
    return buffer.toString(encoding, offset, offset + length);
  },
  write(value, context, offset) {
    // address containing the length of the string
    const lengthAddress = structAddress + offset + STDSTRING_LENGTH_OFFSET;

    // get existing `std::string` buffer
    const bufferSize = platform === '64' ? SIZEOF_STDSTRING_64BIT : SIZEOF_STDSTRING_32BIT;
    const existingBuffer = memoryjs.readBuffer(handle, structAddress + offset, bufferSize);

    // fetch length of string in memory (to determine if it's pointer based)
    const length = memoryjs.readMemory(handle, lengthAddress, memoryjs.INT);

    if ((length > 15 && value.length <= 15) || (length <= 15 && value.length > 15)) {
      // there are two ways strings are stored: directly or with a pointer,
      // we can't go from one to the other (without introducing more complexity),
      // so just skip the bytes to prevent crashing. if a pointer is used, we could
      // technically write any length, but the next time we try writing, we will read
      // the length and assume it's not stored via pointer and will lead to crashes

      // write existing buffer without changes
      existingBuffer.copy(context.buffer, offset);
      return;
    }

    // write new length
    memoryjs.writeMemory(handle, lengthAddress, value.length, memoryjs.UINT32);
    existingBuffer.writeUInt32LE(value.length, STDSTRING_LENGTH_OFFSET);

    if (length > 15 && value.length > 15) {
      // write new string in memory
      const pointer = memoryjs.readMemory(handle, structAddress + offset, memoryjs.POINTER);
      memoryjs.writeMemory(handle, pointer, value, memoryjs.STRING);
    } else if (length <= 15 && value.length <= 15) {
      // write new string directly into buffer
      existingBuffer.write(value, encoding);
    }

    // write our new `std::string` buffer into the buffer we are creating
    existingBuffer.copy(context.buffer, offset);
  },
  SIZE: platform === '64' ? SIZEOF_STDSTRING_64BIT : SIZEOF_STDSTRING_32BIT,
});

module.exports = { STRUCTRON_TYPE_STRING };
