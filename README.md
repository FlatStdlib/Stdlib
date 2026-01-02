<div align="center">
	<h1>clibp v3.0</h1>
	<p>The official minimal C backend alternative to GLIBC</p>
</div>

# Some Info

- The project is still in development!
- The library is a Architecture-targeted freestanding C compiler backend

### What does it do?

- Targets the architecture for built-in tooling (ASM libs)
- Built-in serves as the standard lib, minimal abstractions
- Cross-architect compilation

### What is it used for?

- Creating raw binaries for OS development and building

### Supported Architectures

- x86
- x86_64

# Installation

- Currently; Local install use only for development

Run the following commands to install the library!
```bash
sudo apt install make;
git clone https://github.com/clibplus/clibp_v3_0.git
cd cdlib_v3_0; make; make cloader
```

# Standard Lib Use

```c
#define CLIBP
<headers/init.h>

void entry() {
	print("Hello World");
}
```

# Compile w/ CLIBP

### Linux Linker
```c
./gcc_clibp <*.c> -o <output>
```
