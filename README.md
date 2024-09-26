# AmorFatiOS

A 32-bit hobby operating system for x86 PCs. This project is currently in a very
early state. Stay tuned!

> "Not what happens to you, but how you react to it, is what matters."
> -Epictetus

This quote encapsulates the essence of amor fati—the love of one's fate—by
emphasizing the importance of embracing whatever comes your way and responding
with wisdom and resilience.

#### Watch the demo video on YouTube:
[![Watch the video](https://img.youtube.com/vi/daBceW_YN5Y/maxresdefault.jpg)](https://www.youtube.com/watch?v=daBceW_YN5Y)

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
- [ ] Memory Manager (started)
  - [x] Dynamic Memory Allocation (quick throwaway implementation)
  - [ ] Dynamic Memory Allocation (real implementation)
  - [ ] Virtual Memory
- [ ] TCP/IP Stack
  - [ ] Intel 82545EM Driver (started)
  - [ ] Ethernet Send/Receive (started)
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

## VMware Workstation

Currently, AmorFatiOS works best in VMware Workstation, so that's the platform I recommend testing with.

At the time of writing, VMware Workstation is freely available for personal use.

### Windows Installer
https://softwareupdate.vmware.com/cds/vmw-desktop/ws/17.6.0/24238078/windows/core/VMware-workstation-17.6.0-24238078.exe.tar

### Linux Installer
https://softwareupdate.vmware.com/cds/vmw-desktop/ws/17.6.0/24238078/linux/core/VMware-Workstation-17.6.0-24238078.x86_64.bundle.tar
