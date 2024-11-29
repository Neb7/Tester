
#include "get_next_line.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>


int	main(int ac, char **av)
{
	int	i = 1;
	int	fd, fd2;
	int c = 0;
	char	*d, *d2;
	if (ac == 2)
	{
		fd = open(av[i], O_RDONLY);
		d = get_next_line(fd);
		if (!d)
			printf("(null)");
		while (d != NULL)
		{
			c++;
			printf("%d'%s'", c, d);
			free(d);
			d = get_next_line(fd);
			if (!d)
				printf("(null)");
		}
		close(fd);
	}
	else if (ac == 1)
	{
		d = get_next_line(0);
		if (!d)
			printf("(null)");
		while (d != NULL)
		{
			c++;
			printf("%d'%s'", c, d);
			free(d);
			d = get_next_line(0);
			if (!d)
				printf("(null)");
		}
	}
	else if (ac  == 3)
	{
		fd = open(av[1], O_RDONLY);
		fd2 = open(av[2], O_RDONLY);
		d = get_next_line(fd);
		if (!d)
				printf("(null)");
		d2 = get_next_line(fd2);
		if (!d2)
				printf("(Null)");
		while (!(d == NULL && d2 == NULL))
		{
			c++;
			if (d)
			{
				printf("'%s'", d);
				free(d);
				d = get_next_line(fd);
				if (!d)
					printf("(null)");
			}
			if (d2)
			{
				printf("'%s'", d2);
				free(d2);
				d2 = get_next_line(fd2);
				if (!d2)
					printf("(Null)");
			}
		}
		printf("\n---------------------\n");
		close(fd);
		close(fd2);
	}
}
