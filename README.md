# NyxC2
C2 project with C & python 

# 🛠️ Simple Asynchronous C2 Framework

A lightweight Command & Control (C2) proof-of-concept featuring an asynchronous Python server and a Win32-based C implant. This project demonstrates low-level network programming, process redirection, I/O multiplexing, and basic AV evasion techniques.

> [!WARNING]  
> **For educational purposes only.** This project was developed in a controlled lab environment for learning software architecture and security concepts. Unauthorized use against systems you do not own is strictly prohibited.

---

## 🚀 Features

* **Asynchronous Server:** Handles multiple concurrent connections using the `select` module without blocking.
* **Session Management:** Interactive custom prompt to track active targets (`mc list`) and assign human-readable aliases to IP addresses (`mc rename`) using dynamic Python dictionaries.
* **Targeted Execution:** Send commands to a specific implant by Alias/IP, or broadcast to all targets using `*`.
* **AV Evasion & Stealth:** Bypasses major AV engines (like MS Defender and McAfee) using XOR payload obfuscation and stripped compiler symbols. Compiled to run as a background process (no console window).
* **Process Redirection:** Redirects `cmd.exe` standard streams (In/Out/Err) directly to the network socket.

## 📸 Screenshot


https://github.com/user-attachments/assets/3ac12574-9533-48ed-930c-400e308809d0

## 🧠 What did the project teach me?

* **Red Teaming & Defensive Evasion (New)**
    * **Payload Obfuscation:** Implementing bitwise XOR decryption routines in C to hide malicious strings (`cmd.exe`) in memory, bypassing static analysis and YARA rules.
    * **Signature Evasion:** Stripping debug symbols (`-s` compiler flag) to drastically alter the binary's structure and evade Machine Learning AV engines.
    * **Reverse Engineering Basics:** Analyzing Import Address Tables (IAT) and interpreting VirusTotal AI insights to understand how EDRs flag suspicious behavior.
    * **Reputation Filtering:** Understanding "Mark of the Web" (MotW) and bypassing SmartScreen warnings during the delivery phase in lab environments.

* **Advanced Asynchronous Server Architecture (Python)**
    * **State & Session Management:** Architecting a robust session manager mapping raw network socket objects to custom user aliases via Python dictionaries, including graceful disconnect handling.
    * **I/O Multiplexing:** Implementation of a high-performance server using the `select` module for single-threaded concurrency.
    * **The Dispatcher Pattern:** Development of a non-blocking event loop handling inbound connections, operator commands (`sys.stdin`), and data reception simultaneously.

* **Low-Level Network Programming (C & WinAPI)**
    * **Socket Orchestration:** Initializing Winsock library (`WSAStartup`) and managing TCP connections via `sockaddr_in`.
    * **Process Redirection:** Mastering "Standard Stream Redirection" to link `cmd.exe` streams directly to network sockets.
    * **Windows Internal Structures:** Hands-on experience with `STARTUPINFO` and `PROCESS_INFORMATION` to control execution environments.

* **Git & GitHub workflow (command line)**
    * Professional version control: Writing descriptive commit messages and managing multi-language repositories via CLI.
    * Conflict Resolution: Mastering synchronization between local and remote branches (Force Push, Reset, Merge).

---

## 🛠️ Usage (Lab Environment)

### Server Side (Kali Linux)
Start the asynchronous listener and use the interactive prompt:

```bash
python3 Server_main.py
```
**Available Master Commands:**
* `mc list` : Display all active sessions.
* `mc rename <old_name_or_ip> <new_alias>` : Assign an easy-to-read name to a target.

### Client Side (Windows Target)
Cross-compile the stealthy implant from Linux (Includes `-s` for stripping symbols and `-mwindows` for hiding the console):

```bash
x86_64-w64-mingw32-gcc implant.c -o implant.exe -lws2_32 -mwindows -s
```

---

## ⚠️ Troubleshooting

* **Implant connects then dies instantly?** Ensure your XOR key and decryption loop size perfectly match your payload size (avoiding Null Byte XOR issues). 
* **Target disappears from `mc list`?** The server automatically cleans up dead sockets. Ensure the Windows target hasn't lost network connectivity.
* **No output in the console?** Compile *without* the `-mwindows` and `-s` flags to debug and see standard `printf` outputs locally.

## 🗺️ Roadmap (Future Improvements)

* [ ] Implement secure file Upload/Download functionality.
* [ ] Add basic XOR encryption for **network traffic** (Payload is already obfuscated).
* [x] Create a custom interactive shell prompt on the server side to avoid retyping IPs.
