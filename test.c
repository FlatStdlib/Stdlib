#include <clibp.h>

sArr split_string(string buffer, const char ch, int *idx)
{
    if(!buffer)
        return NULL;

    i32 len = str_len(buffer);
	i32 lines = count_char(buffer, '\n');
    sArr arr = allocate(sizeof(string), lines + 1);
    *idx = 0;
    int _len = 0;

    char LINE[len];
    for(int i = 0; i < len; i++)
    {
        if(buffer[i] == '\0')
            break;

        if(buffer[i] == ch)
        {
            int n = str_len(LINE);
            if(n == 0)
            {
                LINE[0] = ' ';
                LINE[1] = '\0';
            }

            arr[(*idx)++] = str_dup(LINE);

            sArr new_arr = to_heap(arr, sizeof(string) * ((*idx) + 1));
            pfree(arr, 1);
            arr = new_arr;
            if(!arr) println("ERR\n");
            arr[*idx] = NULL;
            LINE[0] = '\0';
            _len = 0;
            continue;
        }

        LINE[_len++] = buffer[i];
        LINE[_len] = '\0';
    }

    if(*idx > 0)
        return arr;

//    free_arr((void *)arr);
    return NULL;
}

sArr split_lines(string buffer, int *idx)
{
	return split_string(buffer, '\n', idx);
}

const string info = "PRETTY_NAME=\"Ubuntu 24.04.3 LTS\"\n"
"NAME=\"Ubuntu\"\n"
"VERSION_ID=\"24.04\"\n"
"VERSION=\"24.04.3 LTS (Noble Numbat)\"\n"
"VERSION_CODENAME=noble\n"
"ID=ubuntu\n"
"ID_LIKE=debian\n"
"HOME_URL=\"https://www.ubuntu.com/\"\n"
"SUPPORT_URL=\"https://help.ubuntu.com/\"\n"
"BUG_REPORT_URL=\"https://bugs.launchpad.net/ubuntu/\"\n"
"PRIVACY_POLICY_URL=\"https://www.ubuntu.com/legal/terms-and-policies/privacy-policy\"\n"
	"UBUNTU_CODENAME=noble\n"
	"LOGO=ubuntu-logo";

int entry(i32 argc, sArr args)
{
	int line_count = 0;
	sArr test = split_lines(info, &line_count);
	print("Lines: "), _printi(line_count), print("\n");

	for(int i = 0; i < line_count; i++)
	{
		char output[150];
		__sprintf(output, "[ %d ]: %s", &i, test[i]);
		println(output);
	}
	return 0;
}
