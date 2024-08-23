# AmorFatiOS

A 32-bit hobby operating system for x86 PCs. This project is currently in a very
early state. Stay tuned!

> "Not what happens to you, but how you react to it, is what matters."
> -Epictetus

This quote encapsulates the essence of amor fati—the love of one's fate—by
emphasizing the importance of embracing whatever comes your way and responding
with wisdom and resilience.

## Build

I build the project on Ubuntu 22.04. To ensure you have what you need, run the
following:

```
sudo apt install build-essential
sudo apt install gcc-multilib
sudo apt install nasm
```

To build the project, run:

```
./build
```

The build script will generate an `os.flp` file in the `bin` directory. This
is a virtual floppy disk that can be attached to a VMware Workstation VM for
testing the operating system.
