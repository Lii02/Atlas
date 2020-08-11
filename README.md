# Atlas
Alpha build of the Atlas Operating System. Still working on bootloader and kernel functionality. The operating system itself supports a basic command line VGA terminal (almost complete). The generated image file for the operating system is formatted under a custom (BCFS) filesystem. This operating system is in no way considered complete, and is currently under heavy development.

## Getting Started
The following steps will serve as a guide for setting up and running the Atlas operating system on a linux machine.

### Prerequisites
The following software is required to compile and test the Atlas Operating System. All software provided has been formally tested on the Ubuntu 18.04 Operating System.

Bochs Packages:
```bochs, bochs-sdl, bochs-x```

QEMU Packages:
```qemu, qemu-i386, qemu-x86_64```

Compiler and Linker Packages:
```ld, gcc, make```

### Compiling and Testing
The project folder is equipped with makefiles that compile and prepare the operating system for testing. Running QEMU on the image file can be used to run the operating system, while bochs will allow you to use a step debugger for the bootloader for compiled kernel. There are multiple bash scripts included with the operating system as well that run compiling and testing commands automatically.

Make - Full Build:
```make all```

Make - Clear Build:
```make clear```

Make - Clear all Intermediate Object Data:
```make clearsrc```

Test Build:
```[qemu-system-i386 -drive format=raw,file=img/atlas.img] OR [bash exec.sh]```

Run Debugger:
```bochs```

Full Build and Test:
```bash fullbuild.sh```

Full Build and Run Step Debugger:
```bash debug.sh```

## Configuring and Developing Operating System
The following steps describe the process of modifying or configuring the operating system to run as desired.

### The BCFS Filesystem
The operating system itself uses a custom filesystem called BCFS (block-chain filesystem). The image file is created by the program 'mkbcfs' which is in the main project directory. If you want to add a file or folder to the operating system's filesystem, simply add said file or folder to the BCFS directory in the main project folder. This folder serves as the root directory of the filesystem, and is used by mkbcfs to create the image file for the operating system. For information on the format specfication of BCFS, read the 'desc.txt' file in the mkbcfs directory.

### Developing the Operating System
All source files for the operating system are located in the 'src' directory, which is subdivided into the 'bootloader' and 'kernel' directory. The bootloader is written in assembly, and can be modified without having to change compile specs. The 'modules' folder in the kernel source directory contains all C files that are compiled for the operating system. If a file is added, the kernel makefile will need to be updated. In the event that another process must be added, the C function that starts the kernel is called `start_kernel(void)` and is located in the folder 'src/kernel/modules/core/kernel_init.c'.

## Additional Notes
1. To provide appropriate git messages, please consider running `make clearsrc` to remove all unnecessary object files before pushing to the repository.
2. If script line ending errors occur, run `dos2unix scriptname`, you must have the `dos2unix` package installed first.