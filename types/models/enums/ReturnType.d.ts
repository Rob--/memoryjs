declare enum ReturnType {
    T_VOID = 0x0,
    T_STRING = 0x1,
    T_CHAR = 0x2,
    T_BOOL = 0x3,
    T_INT = 0x4,
    T_DOUBLE = 0x5,
    T_FLOAT = 0x6
}

export declare type ReturnTypeNumber = ReturnType.T_INT | ReturnType.T_DOUBLE | ReturnType.T_FLOAT;
export declare type ReturnTypeBoolean = ReturnType.T_BOOL;
export declare type ReturnTypeString = ReturnType.T_CHAR | ReturnType.T_STRING;
export declare type ReturnTypeVoid = ReturnType.T_VOID;