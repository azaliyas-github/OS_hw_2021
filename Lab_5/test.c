#include <stdio.h>

int main()
{
    int nbr;
	int i;
	char buf[8];

	nbr = 42;
	while (nbr > 0)
	{
	  nbr = read(0, buf, 8);
	  i = 0;
	  while (i < nbr)
		 printf("%x ", buf[i++]);
	  printf("\n");
	}
}