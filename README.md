# AmorFatiOS

A 32-bit hobby operating system for x86 PCs. This project is currently in a very
early state. Stay tuned!

> "Not what happens to you, but how you react to it, is what matters."
> -Epictetus

This quote encapsulates the essence of amor fati—the love of one's fate—by
emphasizing the importance of embracing whatever comes your way and responding
with wisdom and resilience.

## Goals

- [x] Floppy Disk Bootloader
- [x] Protected Mode
- [x] Interrupt Handling
- [ ] Preemptive Task Switching
  - [x] Round Robin
  - [ ] Priority Based
- [ ] Keyboard Driver (started)
  - [x] Read and Process Key Presses
  - [ ] Handle Caps Lock and Num Lock
- [ ] Mouse Driver (started)
- [x] Text-Mode VGA
- [x] Virtual Consoles
- [ ] PCI Device Enumeration (started)
- [x] Userspace
- [x] System Call Interface
- [ ] Virtual Memory
- [ ] Dynamic Memory Allocation
- [ ] TCP/IP Stack
  - [ ] Intel 82545EM Driver (started)
  - [ ] Ethernet Send/Receive
  - [ ] IP Send/Receive
  - [ ] TCP/UDP Send/Receive
  - [ ] DHCP Client
  - [ ] DNS Client
- [ ] Machine Support
  - [ ] Virtual Machines / Emulators
    - [x] VMware Workstation
    - [ ] Microsoft Hyper-V
    - [ ] Oracle VirtualBox
    - [ ] QEMU
    - [ ] Bochs
  - [ ] Real Hardware
    - [ ] ?
- [ ] Documentation

## Build

I currently build the project on Ubuntu 22.04 running on WSL2 (on Windows),
using GCC 11.4.0 and NASM 2.15.05.

To ensure you have what you need, run the following:

```
sudo apt install build-essential
sudo apt install gcc-multilib
sudo apt install nasm
```

To build the project, run:

```
./build.sh
```

The build script generates the following raw floppy disk image:

```
bin/AmorFatiOS.flp
```

## Run

The OS can be tested in VMware Workstation using the included virtual machine file:

```
vm/AmorFatiOS.vmx
```

This VM has a floppy drive that points to the `bin/AmorFatiOS.flp` image.
