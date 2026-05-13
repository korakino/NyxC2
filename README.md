# NyxC2
C2 project with C & Python 

# 🛠️ Simple Asynchronous C2 Framework

A lightweight Command & Control (C2) proof-of-concept featuring an asynchronous Python server and a Win32-based C implant. This project demonstrates low-level network programming, process redirection, I/O multiplexing, and basic AV evasion techniques.

> [!WARNING]  
> **For educational purposes only.** This project was developed in a controlled lab environment for learning software architecture and security concepts. Unauthorized use against systems you do not own is strictly prohibited.

---

## 🚀 Features

* **Asynchronous Server:** Handles multiple concurrent connections using the `select` module without blocking.
* **Interactive Operator Shell:** Custom `NyxC2 >` prompt featuring real-time data parsing to filter Windows command echoes, handle TCP fragmentation, and provide a clean Red Team operator experience.
* **Resilient Session Management:** Automatic detection of dead sockets and graceful state cleanup to prevent server crashes during brutal target disconnections. Assign human-readable aliases to IP addresses (`mc rename`).
* **Targeted Execution:** Send commands to a specific implant by Alias/IP, or broadcast to all targets using `*`.
* **AV Evasion & Stealth:** Bypasses major AV engines using XOR string obfuscation and dynamically loaded Windows APIs (IAT unhooking). Compiled to run as a background process (no console window).
* **Process Redirection:** Redirects `cmd.exe` standard streams (In/Out/Err) directly to the network socket.

## 📸 Screenshot

https://github.com/user-attachments/assets/3ac12574-9533-48ed-930c-400e308809d0

## 🧠 What did the project teach me?

* **Red Teaming & Defensive Evasion**
    * **Payload Obfuscation:** Implementing bitwise XOR decryption routines in C to hide malicious strings (`cmd.exe`) in memory, bypassing static analysis and YARA rules.
    * **Advanced Defensive Evasion (Dynamic API Loading):** Concealing critical Windows API calls (like `CreateProcessA` and `WSAStartup`) from the Import Address Table by resolving them dynamically at runtime using `GetModuleHandleA` and `GetProcAddress`.
    * **Signature Evasion:** Stripping debug symbols (`-s` compiler flag) to drastically alter the binary's structure and evade Machine Learning AV engines.
    * **Reputation Filtering:** Understanding "Mark of the Web" (MotW) and bypassing SmartScreen warnings.

* **Advanced Asynchronous Server Architecture (Python)**
    * **I/O Multiplexing & Dispatcher Pattern:** Implementation of a high-performance, single-threaded concurrent server using the `select` module to handle inbound connections, operator commands (`sys.stdin`), and data reception simultaneously.
    * **Data Parsing & Stream Sanitization:** Handling TCP packet fragmentation and cleaning raw `cmd.exe` stream outputs (filtering prompts and echoes) for a seamless UI experience.
  
* **Operational Security (OpSec) & Repository Management**
    * **Clean Version Control:** Implementing strict `.gitignore` rules to prevent accidental leaks of compiled malicious binaries (`.exe`) to public repositories.
    * **Safe Artifact Handling:** Ensuring the repository remains a safe, educational space without distributing weaponized code.

* **Low-Level Network Programming (C & WinAPI)**
    * **Socket Orchestration:** Initializing Winsock library (`WSAStartup`) and managing TCP connections via `sockaddr_in`.
    * **Process Redirection:** Mastering "Standard Stream Redirection" to link `cmd.exe` streams directly to network sockets via `STARTUPINFO` and `PROCESS_INFORMATION` structures.

---

## 🛠️ Usage (Lab Environment)

### Server Side (Kali Linux)
Start the asynchronous listener and use the interactive prompt:

```bash
python3 Server_main.py
```

**Available Master Commands (`mc`):**
* `mc list`               : Show all active connected targets
* `mc rename <old> <new>` : Rename a target's alias/IP
* `mc connect <target>`   : Test connection to a specific target
* `mc help`               : Display the help menu
* `mc kill`               : Close the server safely
* `<target|*> <command>`  : Send a command to target(s) (e.g., `* whoami`)

### Client Side (Windows Target)
Cross-compile the stealthy implant from Linux (Includes `-s` for stripping symbols and `-mwindows` for hiding the console):

```bash
x86_64-w64-mingw32-gcc implant.c -o implant.exe -lws2_32 -mwindows -s
```

---

## ⚠️ Troubleshooting

* **Implant connects then dies instantly?** Ensure your XOR key and decryption loop size perfectly match your payload size (avoiding Null Byte XOR issues). 
* **Target disappears from `mc list`?** The server automatically cleans up dead sockets. Ensure the Windows target hasn't lost network connectivity or been killed by an EDR.
* **No output in the console?** Compile *without* the `-mwindows` and `-s` flags to debug and see standard `printf` outputs locally.

## 🗺️ Roadmap (Future Improvements)

* [x] **Custom Interactive Shell:** Create a prompt (`NyxC2 >`) on the server side with data sanitization.
* [x] **Payload Obfuscation:** XOR string encryption and IAT Unhooking.
* [ ] **Network Traffic Encryption:** Implement AES or TLS/SSL to encrypt TCP communications and prevent Blue Team packet sniffing.
* [ ] **Data Exfiltration:** Add secure `upload` and `download` commands for file transfers.
* [ ] **Beaconing & Sleep:** Refactor the implant to sleep and check in periodically instead of maintaining a constant, suspicious TCP connection.
* [ ] **Persistence Mechanisms:** Add functionality to write to Windows Registry Keys or create Scheduled Tasks to survive reboots.