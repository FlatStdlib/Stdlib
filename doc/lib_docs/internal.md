# Definitions
```c
#define clibp_panic(msg) 	\
			__clibp_panic(msg, __FILE__, __LINE__);
```

# Functions
```c
/* internal.c */
fn		toggle_debug_mode();
fn 		__exit(int code);
fn 		execute(string app, sArr args);
fn 		print_sz(const string buffer, int sz);
fn		printc(const char ch);
fn 		printi(int num);
fn 		_printi(int value);
fn 		print(const string buff);
fn		println(const string buff);
fn 		printsz(const string buff, int sz);
fn 		print_args(sArr arr);
ptr		to_heap(ptr p, i32 sz);
fn		__clibp_panic(string msg, string file, int line);
```