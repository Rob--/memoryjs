import type { Process } from "./models/Process";
import type { Module } from "./models/Module";
import type { Callback } from "./models/Callback";
import type { DataTypeBoolean, DataTypeNumber, DataTypeString, DataTypeVector3, DataTypeVector4 } from "./models/enums/DataType";
import type { Vector3, Vector4 } from './models/Vector';
import type { ProtectionType } from "./models/enums/ProtectionType";
import type { SignatureType } from "./models/enums/SignatureType";
import type { FunctionArg } from "./models/FunctionArg";
import type { ReturnTypeBoolean, ReturnTypeNumber, ReturnTypeString, ReturnTypeVoid } from "./models/enums/ReturnType";
import type { ReturnObject } from "./models/ReturnObject";
import type { TriggerType } from "./models/enums/TriggerType.d.";

declare namespace MemoryJs {

    function openProcess(pid: number): Process;
    function openProcess(pid: number, callback: Callback<Process>): void;

    function getProcesses(): Process[];
    function getProcesses(callback: Callback<Process[]>): void;

    function findModule(moduleName: string, pid: number): Module;
    function findModule(moduleName: string, callback: Callback<Module[]>): void;

    function getModules(pid: number): Module[];
    function getModules(pid: number, callbacK: Callback<Module[]>): void;

    function readMemory(handle: number, address: number, dataType: DataTypeNumber): number;
    function readMemory(handle: number, address: number, dataType: DataTypeString): string;
    function readMemory(handle: number, address: number, dataType: DataTypeBoolean): boolean;
    function readMemory(handle: number, address: number, dataType: DataTypeVector3): Vector3;
    function readMemory(handle: number, address: number, dataType: DataTypeVector4): Vector4;

    function readMemory(handle: number, address: number, dataType: DataTypeNumber, callback: Callback<number>): void;
    function readMemory(handle: number, address: number, dataType: DataTypeString, callback: Callback<string>): void;
    function readMemory(handle: number, address: number, dataType: DataTypeBoolean, callback: Callback<boolean>): void;
    function readMemory(handle: number, address: number, dataType: DataTypeVector3, callback: Callback<Vector3>): void;
    function readMemory(handle: number, address: number, dataType: DataTypeVector4, callback: Callback<Vector4>): void;

    function readBuffer(handlle: number, address: number, size: number): Buffer;
    function readBuffer(handlle: number, address: number, size: number, callback: Callback<Buffer>): void;

    function writeMemory(handle: number, address: number, value: number, dataType: DataTypeNumber): void;
    function writeMemory(handle: number, address: number, value: string, dataType: DataTypeString): void;
    function writeMemory(handle: number, address: number, value: boolean, dataType: DataTypeBoolean): void;
    function writeMemory(handle: number, address: number, value: Vector3, dataType: DataTypeVector3): void;
    function writeMemory(handle: number, address: number, value: Vector4, dataType: DataTypeVector4): void;

    function writeBuffer(handle: number, address: number, buffer: Buffer): void;

    function getRegions(handle: number): any;
    function getRegions(handle: number, callback: Callback<any>): any;

    function virtualProtectEx(handle: number, address: number, size: number, protection: ProtectionType): ProtectionType;

    function findPattern(handle: number, moduleName: string, signature: any, signatureType?: SignatureType, patternOffset?: number, addressOffset?: number): number;
    function findPattern(handle: number, moduleName: string, signature: any, signatureType: SignatureType, patternOffset: number, addressOffset: number, callback: Callback<number>): void;


    function callFunction(handle: number, args: FunctionArg[], returnType: ReturnTypeBoolean, address: number): ReturnObject<boolean>;
    function callFunction(handle: number, args: FunctionArg[], returnType: ReturnTypeNumber, address: number): ReturnObject<number>;
    function callFunction(handle: number, args: FunctionArg[], returnType: ReturnTypeString, address: number): ReturnObject<string>;
    function callFunction(handle: number, args: FunctionArg[], returnType: ReturnTypeVoid, address: number): ReturnObject<void>;

    function callFunction(handle: number, args: FunctionArg[], returnType: ReturnTypeBoolean, address: number, callback: Callback<ReturnObject<boolean>>): void;
    function callFunction(handle: number, args: FunctionArg[], returnType: ReturnTypeNumber, address: number, callback: Callback<ReturnObject<number>>): void;
    function callFunction(handle: number, args: FunctionArg[], returnType: ReturnTypeString, address: number, callback: Callback<ReturnObject<string>>): void;
    function callFunction(handle: number, args: FunctionArg[], returnType: ReturnTypeVoid, address: number, callback: Callback<ReturnObject<void>>): void;

    function attackDebugger(pid: number, exitOnDetatch: boolean): any;
    function detatchDebugger(pid: number): any;

    function handleDebugEvent(pid: number, threadId: number);
    function setHardwareBreakpoint(pid: number, address: number, hardwareRegister, trigger: TriggerType, length: number);

    function removeHardwareBreakpoint(pid: number, hardwareRegister);

}

export = MemoryJs;