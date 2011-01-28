#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int HexToDec(char *Str)
{
	int result=0;
	int i=0,szLen=0,nHex;
	szLen = strlen(Str);
	nHex = '0';
	for (; i < szLen; i++)
	{
		if ((Str[i] >= '0') && (Str[i] <= '9'))
			nHex = '0';
		else if ((Str[i] >= 'A') && (Str[i] <= 'F'))
			nHex = 'A' - 10;
		else if ((Str[i] >= 'a') && (Str[i] <= 'f'))
			nHex = 'a' - 10;
		else
			return 0;
		result = result * 16 + Str[i] - nHex;
	}
	return result;
} 

int main(void)
{
	int t=0;
	t = HexToDec("a821");
	return 0;
}
