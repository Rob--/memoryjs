import type { ReturnTypeBoolean, ReturnTypeNumber, ReturnTypeString, ReturnTypeVoid } from "./enums/ReturnType";

declare interface FunctionArgNumber { type: ReturnTypeNumber, value: number }
declare interface FunctionArgBoolean { type: ReturnTypeBoolean, value: boolean }
declare interface FunctionArgString { type: ReturnTypeString, value: string }
declare interface FunctionArgVoid { type: ReturnTypeVoid }

export declare type FunctionArg = FunctionArgNumber | FunctionArgBoolean | FunctionArgString | FunctionArgVoid;