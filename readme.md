This C program is a classic example of **Low-Level Systems Programming**. You aren't just using a library; you're using **Direct System Calls** (`SYS_open`, `SYS_read`) to talk to the Linux Kernel and parsing the `/proc` filesystem manually.

Here is a clean, professional `README.md` for your GitHub repo.
---

# 🕵️ Process Monitor (Low-Level C)

A lightweight Linux utility that calculates the lifetime CPU usage and age of a specific process by directly interfacing with the **Linux Kernel via System Calls**.

## 🛠️ How it Works

Unlike standard utilities that use `glibc` wrappers, this tool demonstrates:

* **Direct Syscalls:** Uses `syscall(SYS_open, ...)` and `syscall(SYS_read, ...)` to bypass standard library overhead.
* **Kernel Parsing:** Manually parses the `/proc/[pid]/stat` file—a special "virtual file" created by the kernel to expose process metadata.
* **Clock Tick Conversion:** Uses `sysconf(_SC_CLK_TCK)` (usually **100 HZ**) to convert raw kernel "ticks" into human-readable seconds.

## 📊 Metrics Calculated

1. **User Time (`utime`):** Ticks the process spent executing in user mode.
2. **System Time (`stime`):** Ticks the process spent in kernel mode (handling syscalls).
3. **Start Time:** When the process was born relative to the system boot.
4. **Lifetime CPU Usage:** Calculated using the formula:

$$Usage = \left( \frac{utime + stime}{uptime\_ticks - starttime} \right) \times 100$$




---
