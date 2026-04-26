# NyxC2
C2 project with C &amp; python 



## What did the project teach me ?
* **Low-Level Network Programming (C & WinAPI)**
    * Socket Orchestration: Initializing Winsock library (`WSAStartup`) and managing TCP connections via `sockaddr_in`.
    * Process Redirection: Mastering "Standard Stream Redirection" to link `cmd.exe` streams directly to network sockets.
    * Windows Internal Structures: Hands-on experience with `STARTUPINFO` and `PROCESS_INFORMATION` to control execution environments.

* **Advanced Asynchronous Server Architecture (Python)**
    * I/O Multiplexing: Implementation of a high-performance server using the `select` module for single-threaded concurrency.
    * The Dispatcher Pattern: Development of a non-blocking event loop handling inbound connections, operator inputs (`sys.stdin`), and data reception.
    * Data Management: Streamlining raw byte reception and decoding from remote targets.

* **Red Teaming & Defensive Evasion**
    * Reputation Filtering: Understanding "Mark of the Web" (MotW) and bypassing SmartScreen warnings in lab environments.
    * Cross-Compilation: Forging Windows-compatible binaries from a Linux environment using the `mingw-w64` toolchain.
    * Payload Stealth: Utilizing linker flags (`-mwindows`) to strip GUI consoles and run processes silently in the background.

* **Debugging & Operational Security (OpSec)**
    * Network Troubleshooting: Diagnosing "Silent Crashes" and resolving IP/Port mismatches in virtualized NAT/Bridged environments.
    * Stability Testing: Ensuring persistent C2 communications and handling unexpected client disconnections.

* **Git & GitHub workflow (command line)**
    * Professional version control: Writing descriptive commit messages and managing multi-language repositories via CLI.
    * Conflict Resolution: Mastering synchronization between local and remote branches (Force Push, Reset, Merge).
