<div align="center">
	<h1>clibp v3.0</h1>
	<p>The official minimal C backend alternative to GLIBC</p>
</div>

# Some Info

- The project is still in development!
- The library is a minimal C backend with a custom loader to link!

# Things clib+ doesn't support yet

- No argument syntax checking<br />
	``Expects: <input> -o <output>``<br />
	``Optional flags: --strip --c``
- Multiple files
- C flags
- Linker flags

### Supported Architectures

- x86
- x86_64
- AMD
- ARM v5/v7
- Aarch64
- RISC-V
- WIN64

# Installation

- Currently; Local install use only for development

Run the following commands to install the library!
```bash
sudo apt install git make;
git clone https://github.com/clibplus/clibp_v3_0.git
cd cdlib_v3_0
make # make tcc (for tcc (not complete, testing stage))
```

# Standard Lib Use

```c
#include <clibp.h>

int entry() {
	println("Hello World");
	return 0;
}
```

# Compile w/ CLIBP

### Linux Linker
```c
./gcc_clibp <*.c> -o <output>
```
