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

### Supported Architectures

- x86
- x86_64
- AMD
- RISC-V

### Install

Run the following commands to install the library!
```bash
sudo apt install git make;
git clone https://github.com/libbase/base.git
cd base
make # make tcc (for tcc (not complete, testing stage))
```

### Standard Use

```c
#include <libbase.h>

int entry() {
	println("Hello World");
	return 0;
}
```

# Compile w/ CLIBP

### Linux Linker
```c
Usage: lbg <c_file> <opt> <output>
Use --help for help or more arguments
```
