<div align="center">
	<h1>libbase</h1>
	<p>The official minimal C backend alternative to GLIBC</p>

● [Start-up](/README.md)
| [Introduction](doc/intro.md)
| [Quick Doc](/quick_doc.md)
| [Libraries](doc/libraries.md)
| [Todo](https://github.com/libbase/libbase)
</div>

# Some Info

- The project is still in development!
- The library is a minimal C backend with a custom loader to link!
- Currently working on a machine code compiler to bind all FSL into a modernize C-like language in the future!

### Supported Architectures

- x86
- x86_64
- AMD
- RISC-V (Still in development)

### Install

Run the following commands to install the library!
```bash
sudo apt install git make -y
git clone https://github.com/FlatStdlib/Stdlib.git	# FSL-Stdlib
git clone https://github.com/FlatStdlib/fsl.git 	# FSL-GCC Toolchain
cd Stdlib
make # make tcc (for tcc (not complete, testing stage))
```

### Standard Use

```c
#include <fsl.h>

int entry() {
	println("Hello World");
	return 0;
}
```

# Compile w/ CLIBP

### Linux Linker
```c
Usage: fsl <c_file> <opt> <output>
Use --help for help or more arguments
```

# Contribution

Want to help out with the project? You can start by downloading this stdlib and the FSL-GCC toolchain compiler @ https://github.com/FlatStdlib/fsl

# Community 

We have a discord server for the public FSL community
Join here for any questions or help with anything FSL https://discord.gg/3w85CEhN6w

### File Structure Information
```
| - Stdlib				; Stdlib
|
| - headers
	| - fsl.h			; Start of the Stdlib
	| - fsl_gcc.h
	| - asm.h			; Syscall chaining to match names to each architecture for target
	| - architectures 	; Syscall Based Single-header libs
| - src
	| - asm				; Some ASM Wrappers
	| - c 				; Basic C Internals and Stdlib
		| - stdlib		; Type(s) and Functionalities
| - tests				; Tests to run. Indicating Good for Production Use On the target host
	
```
