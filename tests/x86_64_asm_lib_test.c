/* x86_64 ASM Lib TEST */
#include <fsl_asm.h>

void _start()
{
	char BUFF[100];

	int bytes;
	while(1)
	{
		__write__(1, "> ", 2);
		if((bytes = __read__(0, BUFF, 100)) <= 0)
			continue;

		if(BUFF[0] == 'q')
			break;

		if(BUFF[0] == 'h' && BUFF[1] == 'i')
			__write__(1, "hi\n", 3);

		BUFF[0] = '\0';
	}
	__write__(1, "dick\n", 5);
	__exit__(0);
}
