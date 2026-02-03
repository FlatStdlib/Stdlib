#include <fsl.h>

int entry()
{
	array arr = init_array();
	arr = array_append(arr, "test");
	arr = array_append(arr, "skid");

	print("Array: [");
	for(int i = 0; arr[i] != NULL; i++)
	{
		print("\""), print(arr[i]), print("\"");
		arr[i + 1] == NULL ? print("") : print(", ");
	}
	println("]");

	print("Array Index Check: "), printi(array_contains_str(arr, "skid")), print("\n");
	return 0;
}
