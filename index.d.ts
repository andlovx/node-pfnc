/// <reference types="node" />

declare module "pfnc" {
  /**
   * The origin of referred port.
   */
  export type Origin = "local" | "remote" | "both";

  /**
   * Input option for scanner.
   */
  export type Options = {
    origin: Origin;
    port: number;
  };

  /**
   * Result array from scanner.
   */
  export type Result = {
    origin: string;
    port: number;
    pid: number;
    path: string;
  }[];

  /**
   * The scanner function.
   * @param options Input options for scanning.
   */
  export function scan(options: Options): Result;
}
