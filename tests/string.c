#include <fsl.h>

#define PRE_DATA "This is a test string...!"
#define DATA_LEN str_len("This is a test string...!")

int entry()
{
    string data = allocate(0, 1024);
    
    /* str_append() */
    if(str_append(data, PRE_DATA)) {
        _printf("lulzsec string added to buffer: '%s'\n", data);
    } else {
        println("Failed needle to append to buffer!");
    }
    print("Buffer Data: '"), print(data), print("'\n");

    /* ptr_to_str() */
    char pointer[100];
    ptr_to_str(data, pointer);
    print("Pointer to 'data' variable: "), println(pointer);

    /* str_len() */
    int data_sz = str_len(data);
    print("Data len: "), printi(data_sz), println(NULL);

    /* str_cmp() */
    if(str_cmp(data, "This is a test string...!"))
        println("str_cmp() String Matched!");

    /* mem_cmp() */
    if(mem_cmp(data, "This", 4))
        println("mem_cmp() substring 'This' found @ beginning of the string, first 4 characters!");

    /* find_string() */
    int pos = 0;
    if((pos = find_string(data, "test")) > -1)
    {
        print("find_string() substring 'test' found @ idx: "), printi(pos), print("...!\n");
    }

    /* str_append() */
    str_append_int(data, 5);
    if(find_string(data, "5")) {
        _printf("\"5\" string added to buffer: '%s'\n", data);
    } else {
        println("Failed needle to append to buffer!");
    }

    string arr_test[] = {"arr", "test", NULL};
    str_join(data, (array)arr_test, ' ');
    _printf("str_join(): '%s'\n", data);

    string sub = get_sub_str(data, pos, DATA_LEN - 1);
    _printf("Substr: %s\n", sub);

    string rev = string_reverse(data);
    print("string_reverse(): "), println(rev);
    return 0;
}