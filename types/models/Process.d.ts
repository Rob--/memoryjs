export declare interface Process {
    dwSize: number;
    th32ProcessID: number;
    cntThreads: number;
    th32ParentProcessID: number;
    pcPriClassBase: number;
    szExeFile: string;
    modBaseAddr: number;
    handle: number;
}