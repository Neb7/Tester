#include "get_next_line_bonus.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>

int	main(int ac, char **av)
{
	int	i = 1;
	int	fd, fd2;
	int c = 0;
	char	*d;
	char	*d2;

	if (ac)
	{
			fd = open(av[1], O_RDONLY);
			fd2 = open(av[2], O_RDONLY);
			d = get_next_line(fd);
			d2 = get_next_line(fd2);
			while (!(d == NULL && d2 == NULL))
			{
				c++;
				if (d)
				{
					printf("   fd1 : '%s'", d);
					free(d);
					d = get_next_line(fd);
				}
				if (d2)
				{
					printf("   fd2 : '%s'", d2);
					free(d2);
					d2 = get_next_line(fd2);
				}
			}
			printf("\n---------------------\n");
			close(fd);
			close(fd2);
			i++;
	}
}
