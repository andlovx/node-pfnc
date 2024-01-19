/// <reference types="node" />

declare module "@xertified/pfnc" {
  /**
   * The origin of referred port.
   */
  export type Origin = "local" | "remote" | "both";

  /**
   * Input option for scanner.
   */
  export type Options = {
    /**
     * The origin of referred port.
     */
    origin: Origin;
    /**
     * The UDP/TCP network port to inquire connection information for.
     */
    port: number;
  };

  /**
   * Result array from scanner.
   */
  export type Result = {
    /**
     * The origin of port information.
     */
    origin: string;
    /**
     * The other port of peer connection.
     */
    port: number;
    /**
     * The process ID.
     */
    pid: number;
    /**
     * The resolved absolute path of application utilizing the inquired port.
     */
    path: string;
  }[];

  /**
   * The scanner function. Call this function to get information on related
   * peer network connection.
   *
   * @param options Input options for scanning.
   * @returns Array of result objects containing i.e. port, pid and program path.
   */
  export function scan(options: Options): Result;
}
