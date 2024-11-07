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
					printf("'%s'", d);
					free(d);
					d = get_next_line(fd);
				}
				if (d2)
				{
					printf("'%s'", d2);
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

/*



ssize_t	ft_gnl_strchar(char const *str, char to_find)
{
	ssize_t	i;

	i = 0;
	if (!str)
		return (-1);
	while (str[i])
	{
		if (str[i] == to_find)
			return (i);
		i++;
	}
	return (-1);
}

size_t	ft_gnl_strlen(char *str)
{
	size_t	i;

	if (!str)
		return (0);
	i = 0;
	while (str[i] != '\0')
		i++;
	return (i);
}

char	*get_next_line(int fd)
{
	static char	buffer[BUFFER_SIZE + 1];
	char	*d;
	ssize_t		readed;

	if (fd < 0 || read(fd, 0, 0) < 0)
		return (NULL);
	d = NULL;
	readed = read(fd, buffer, BUFFER_SIZE);
	if (!readed)
		return (NULL);
	while (readed)
	{
		buffer[readed] = 0;
		d = ft_gnl_strjoin(d, buffer, 0);
		if (ft_gnl_strchar(d, '\n') != -1)
			break ;
		readed = read(fd, buffer, BUFFER_SIZE);
	}
	return (ft_gnl_suite(d, buffer));
}

char	*ft_gnl_suite(char *d, char *buffer)
{
	t_gnl_list	lst;

	lst.i = ft_gnl_strchar(d, '\n');
	if (d && lst.i != -1 && lst.i != BUFFER_SIZE)
	{
		lst.start = 0;
		lst.len = lst.i;
		d = ft_gnl_substr(d, &lst, buffer, true);
		lst.start = lst.i + 1;
		lst.len = BUFFER_SIZE - lst.i;
		buffer = ft_gnl_substr(d, &lst, buffer, false);
	}
	else if (d)
	{
		lst.start = 0;
		lst.len = BUFFER_SIZE;
		d = ft_gnl_substr(d, &lst, buffer, true);
		lst.len = 1;
		buffer = ft_gnl_substr("", &lst, buffer, false);
	}
	return (d);
}

char	*ft_gnl_substr(char *s, t_gnl_list *lst, char *buffer, bool te)
{
	char	*d;
	size_t	j;

	j = ft_gnl_strlen(&s[lst->start]);
	if ((j > lst->len))
		j = lst->len;
	if (te)
	{
		d = (char *)malloc(sizeof(char) * (j + 1));
		if (!d)
			return (ft_free_all(s));
		d[j] = 0;
		while (j-- > 0)
			d[j] = s[lst->start + j];
		if (s)
			free(s);
		return (d);
	}
	else
	{
		buffer[j] = 0;
		while (j-- > 0)
			buffer[j] = s[lst->start + j];
		return (buffer);
	}
}

char	*ft_free_all(char *s)
{
	if (s)
		free(s);
	return (NULL);
}

char	*ft_gnl_strjoin(char *s1, char *s2, size_t i)
{
	size_t	l1;
	size_t	l2;
	char	*d;

	l1 = ft_gnl_strlen(s1);
	printf("%lu     aaaaaaaaaaaaa\n", l1);
	l2 = ft_gnl_strlen(s2);
	d = (char *)malloc(sizeof(char) * (l1 + l2 + 1));
	if (!d)
		return (ft_free_all(s1));
	while (i < l1)
	{
		d[i] = s1[i];
		i++;
	}
	while (i < l1 + l2)
	{
		d[i] = s2[i - l1];
		i++;
	}
	d[i] = 0;
	if (l1)
		free(s1);
	return (d);
}
*/