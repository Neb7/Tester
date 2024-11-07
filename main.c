#include "get_next_line.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>


int	main(int ac, char **av)
{
	int	i = 1;
	int	fd;
	int c = 0;
	char	*d;
	if (ac)
	{
		while (i < ac)
		{
			fd = open(av[i], O_RDONLY);
			d = get_next_line(fd);
			while (d != NULL)
			{
				c++;
				printf("%d'%s'", c, d);
				free(d);
				d = get_next_line(fd);
			}
			printf("\n---------------------\n");
			close(fd);
			i++;
		}
	}
}
