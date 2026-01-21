import std

// Const variable
const int: n = 3;

// Decalre variable
int: age;

// Declare and initialize a variable
int: age = 10;

// Declare, allocate stack space, and initialize variable
int: position[2] = {10, 3};

// Declare, allocate stack space and Initialize variable
string: foo = "Hello World";

// Declare and allocate memory then Initialize variable
<HEAP(100)>string: name = "Lawlz";

fnc entry(i32 argc, string argv[]) -> bool
{
	printf("Hello World");
}

// Idenifier: <HEAP>
// Description: Declare the following code as heap-based function, enforcing no stack data
// Allocate, Realloc, Free as indication of what the block does with the memory block provided
[HEAP: ALLOCATE, REALLOCATE, FREE]
fnc foo(<HEAP>char: n)
{
	return n + 1; // ((char *)n + 1);
}
