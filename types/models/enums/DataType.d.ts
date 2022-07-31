
export declare enum DataType {
    BYTE = "byte",
    INT = "int",
    INT32 = "int32",
    UINT32 = "uint32",
    INT64 = "int64",
    UINT64 = "uint64",
    DWORD = "dword",
    SHORT = "short",
    LONG = "long",
    FLOAT = "float",
    DOUBLE = "double",
    BOOL = "bool",
    BOOLEAN = "boolean",
    PTR = "ptr",
    POINTER = "pointer",
    STR = "str",
    STRING = "string",
    VEC3 = "vec3",
    VECTOR3 = "vector3",
    VEC4 = "vec4",
    VECTOR4 = "vector4"
}

export declare type DataTypeNumber = DataType.BYTE |
    DataType.INT | DataType.INT32 |
    DataType.UINT32 | DataType.UINT32 |
    DataType.INT64 | DataType.UINT64 |
    DataType.DWORD | DataType.SHORT |
    DataType.LONG | DataType.FLOAT | DataType.DOUBLE |
    DataType.PTR | DataType.POINTER;
export declare type DataTypeBoolean = DataType.BOOL | DataType.BOOLEAN;
export declare type DataTypeString = DataType.STR | DataType.STRING;
export declare type DataTypeVector3 = DataType.VEC3 | DataType.VECTOR3;
export declare type DataTypeVector4 = DataType.VEC4 | DataType.VECTOR4;


