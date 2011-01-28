#include <stdio.h>
#include <string.h>


int main(void)
{
	FILE *fp;
	char *p = "\x00\x00\x00\x00\x00\x00\x00\x00";
	fp = fopen("fillBytes.txt", "wb+");
	for(int i = 0; i < 288; i++)
		fwrite(p, 1, 8, fp);

	fclose(fp);
	return 0;
}
