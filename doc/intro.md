<div align="center">
    <h1>clib+ Introduction</h1>
</div>


# Hello World

```c
#include <clibp.h>

int entry()
{
    println("Hello World");
    return 0;
}
```

# Heap / String
This example is a heap-based string
```c
#include <clibp.h>

int entry()
{
    string name = allocate(0, 10);
    mem_cpy(name, "Chris", 5);

    println(name);
    pfree(name, 1);
    return 0;
}
```

# Array
This example is a heap-based array
```c
#include <clibp.h>

int entry()
{
    array arr = allocate(sizeof(char *), 2);

    arr[0] = allocate(0, 10);
    mem_cpy(arr[0], "Chris", 5);
    arr[1] = allocate(0, 10);
    mem_cpy(arr[1], "Chris", 5);

    // Output array elements
    for(int i = 0; i < 2; i++)
    {
        println(arr[i]);
        pfree(arr[i], 1);
    }

    pfree(arr);
    return 0;
}
```

# Map

Coming soon...