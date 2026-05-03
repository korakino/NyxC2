# NyxC2
C2 project with C &amp; python 
# 🛠️ Simple Asynchronous C2 Framework

A lightweight Command & Control (C2) proof-of-concept featuring an asynchronous Python server and a Win32-based C implant. This project demonstrates low-level network programming, process redirection, and I/O multiplexing.

> [!WARNING]  
> **For educational purposes only.** This project was developed in a controlled lab environment for learning software architecture and security concepts. Unauthorized use against systems you do not own is strictly prohibited.

---

## 🚀 Features

* **Asynchronous Server:** Handles multiple concurrent connections using the `select` module without blocking.
* **Targeted Execution:** Send commands to a specific implant by IP or broadcast to all targets using `*`.
* **Stealthy Implant:** Compiled with flags to run as a background process (no console window).
* **Process Redirection:** Redirects `cmd.exe` standard streams (In/Out/Err) directly to the network socket.

## 📸 Screenshot



https://github.com/user-attachments/assets/3ac12574-9533-48ed-930c-400e308809d0



## 🧠 What did the project teach me?

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

---

## 🛠️ Usage (Lab Environment)

### Server Side (Kali Linux)
Start the asynchronous listener:

    python3 Server_main.py

### Client Side (Windows Target)
Cross-compile the stealthy implant from Linux:

    x86_64-w64-mingw32-gcc implant.c -o implant.exe -lws2_32 -mwindows

---

## ⚠️ Troubleshooting

* **Implant connects then dies instantly?** Ensure Windows Defender (Real-time protection) is disabled in your lab environment. It actively kills `cmd.exe` processes spawned by unknown binaries connecting to network sockets.
* **No output in the console?** Compile *without* the `-mwindows` flag to debug and see standard `printf` outputs locally.

## 🗺️ Roadmap (Future Improvements)

* [ ] Implement secure file Upload/Download functionality.
* [ ] Add basic XOR encryption for network traffic.
* [ ] Create a custom interactive shell prompt on the server side to avoid retyping IPs.
