/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tester_gnl.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: benpicar <benpicar@student.42mulhouse.fr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 10:05:34 by benpicar          #+#    #+#             */
/*   Updated: 2026/01/23 16:23:08 by benpicar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

#define RED    "\033[0;31m"
#define GREEN  "\033[0;32m"
#define YELLOW "\033[0;33m"
#define NC     "\033[0m"

// Variables globales pour statistiques
int failed_tests = 0;
int total_tests = 0;

typedef int (*test_func)(void);

// Fonction fork pour exécuter un test avec protection
int	run_test_fork(const char *test_name, test_func func)
{
	total_tests++;
	printf("%s: ", test_name);
	fflush(stdout);
	
	pid_t pid = fork();
	if (pid == -1)
	{
		printf("%s[ERROR]%s\n", RED, NC);
		failed_tests++;
		return (0);
	}
	
	if (pid == 0)
	{
		int result = func();
		exit(result);
	}
    
	int	status;
	int	timeout = 5; // 5 secondes pour GNL
	int	elapsed = 0;
	
	while (elapsed < timeout * 10)
	{
		int ret = waitpid(pid, &status, WNOHANG);
		if (ret == pid)
		{
			if (WIFEXITED(status))
			{
				int exit_code = WEXITSTATUS(status);
				if (exit_code == 0)
				{
					printf("%s[OK]%s\n", GREEN, NC);
					return (1);
				}
				else
				{
					printf("%s[KO]%s\n", RED, NC);
					failed_tests++;
					return (0);
				}
			}
			else if (WIFSIGNALED(status))
			{
				int sig = WTERMSIG(status);
				printf("%s[CRASH: Signal %d (%s)]%s\n", RED, sig,
						sig == SIGSEGV ? "SEGFAULT" :
						sig == SIGBUS ? "BUS ERROR" :
						sig == SIGABRT ? "ABORT" : "UNKNOWN", NC);
				failed_tests++;
				return (0);
			}
		}
		else if (ret == -1)
		{
			printf("%s[ERROR]%s\n", RED, NC);
			failed_tests++;
			return (0);
		}
		usleep(100000); // 100ms
		elapsed++;
	}
	
	kill(pid, SIGKILL);
	waitpid(pid, NULL, 0);
	printf("%s[TIMEOUT]%s\n", YELLOW, NC);
	failed_tests++;
	return (0);
}

// Test basique : lecture d'un fichier ligne par ligne
int	test_basic_3_lines(void)
{
	const char	*expected_lines[] = {"Hello world\n", "42\n", "Test\n"};
	int			fd = open("asset/test1.txt", O_RDONLY);
	if (fd < 0)
	{
		printf("  Error: impossible d'ouvrir test1.txt\n");
		return (1);
	}
	for (int i = 0; i < 3; ++i)
	{
		char	*line = get_next_line(fd);
		if (!line || strcmp(line, expected_lines[i]) != 0)
		{
			printf("  Error ligne %d: attendu '%s', obtenu '%s'\n",
					i+1, expected_lines[i], line ? line : "(null)");
			free(line);
			close(fd);
			return (1);
		}
		free(line);
	}
	char	*last = get_next_line(fd);
	if (last)
	{
		printf("  Error: attendu NULL, obtenu '%s'\n", last);
		free(last);
		close(fd);
		return (1);
	}
	close(fd);
	return (0);
}

int	test_empty_file(void)
{
	int	fd = open("asset/empty.txt", O_RDONLY);
	if (fd < 0)
	{
		printf("  Error: impossible d'ouvrir empty.txt\n");
		return (1);
	}
	char	*line = get_next_line(fd);
	if (line != NULL)
	{
		printf("  Error: fichier vide devrait retourner NULL\n");
		free(line);
		close(fd);
		return (1);
	}
	close(fd);
	return (0);
}

int	test_single_no_nl(void)
{
	int	fd = open("asset/single_no_nl.txt", O_RDONLY);
	if (fd < 0)
		return (1);
	char	*line = get_next_line(fd);
	if (!line || strcmp(line, "Unique line") != 0)
	{
		printf("  Error: attendu 'Unique line', obtenu '%s'\n", line ? line : "(null)");
		free(line);
		close(fd);
		return (1);
	}
	free(line);
	char	*last = get_next_line(fd);
	if (last)
	{
		free(last);
		close(fd);
		return (1);
	}
	close(fd);
	return (0);
}

int	test_single_with_nl(void)
{
	int	fd = open("asset/single_with_nl.txt", O_RDONLY);
	if (fd < 0)
		return (1);
	char	*line = get_next_line(fd);
	if (!line || strcmp(line, "Unique line\n") != 0)
	{
		printf("  Error: attendu 'Unique line\\n', obtenu '%s'\n", line ? line : "(null)");
		free(line);
		close(fd);
		return (1);
	}
	free(line);
	close(fd);
	return (0);
}

int	test_long_line(void)
{
	char	expected[53249];
	memset(expected, 'A', 53248);
	expected[53248] = '\0';
	
	int	fd = open("asset/long_line.txt", O_RDONLY);
	if (fd < 0)
		return (1);
	char	*line = get_next_line(fd);
	if (!line || strcmp(line, expected) != 0)
	{
		printf("  Error: ligne longue incorrecte (len=%zu)\n", line ? strlen(line) : 0);
		free(line);
		close(fd);
		return (1);
	}
	free(line);
	close(fd);
	return (0);
}

int	test_empty_lines(void)
{
	int	fd = open("asset/empty_lines.txt", O_RDONLY);
	if (fd < 0)
		return (1);
	for (int i = 0; i < 10; ++i)
	{
		char	*line = get_next_line(fd);
		if (!line || strcmp(line, "\n") != 0)
		{
			printf("  Error ligne vide %d: attendu '\\n', obtenu '%s'\n",
					i+1, line ? line : "(null)");
			free(line);
			close(fd);
			return (1);
		}
		free(line);
	}
	close(fd);
	return (0);
}

int	test_no_nl_final(void)
{
	int	fd = open("asset/no_nl_final.txt", O_RDONLY);
	if (fd < 0)
		return (1);
	char	*line = get_next_line(fd);
	if (!line || strcmp(line, "No newline at end") != 0)
	{
		printf("  Error: attendu 'No newline at end', obtenu '%s'\n", line ? line : "(null)");
		free(line);
		close(fd);
		return (1);
	}
	free(line);
	close(fd);
	return (0);
}

int	test_multi_fd(void)
{
	int	fd1 = open("asset/test1.txt", O_RDONLY);
	int	fd2 = open("asset/single_with_nl.txt", O_RDONLY);
	if (fd1 < 0 || fd2 < 0)
		return (1);
	
	char	*l1 = get_next_line(fd1);
	char	*l2 = get_next_line(fd2);
	
	int	error = 0;
	if (!l1 || strcmp(l1, "Hello world\n") != 0)
	{
		printf("  Error fd1: attendu 'Hello world\\n', obtenu '%s'\n", l1 ? l1 : "(null)");
		error = 1;
	}
	if (!l2 || strcmp(l2, "Unique line\n") != 0)
	{
		printf("  Error fd2: attendu 'Unique line\\n', obtenu '%s'\n", l2 ? l2 : "(null)");
		error = 1;
	}
	
	free(l1);
	free(l2);
	close(fd1);
	close(fd2);
	return (error);
}

int	test_invalid_fd(void)
{
	char	*line = get_next_line(-1);
	if (line != NULL)
	{
		printf("  Error: fd invalide devrait retourner NULL\n");
		free(line);
		return (1);
	}
	return (0);
}

int	test_stress(void)
{
	int	fd = open("asset/stress.txt", O_RDONLY);
	if (fd < 0)
	{
		printf("  Error: impossible d'ouvrir asset/stress.txt\n");
		return (1);
	}
	int		count = 0;
	char	*line;
	while ((line = get_next_line(fd)))
	{
		free(line);
		count++;
	}
	close(fd);
	if (count != 1000)
	{
		printf("  Error: attendu 1000 lignes, obtenu %d\n", count);
		return (1);
	}
	return (0);
}

int	main(void)
{
	printf("=== GET_NEXT_LINE TESTER (BUFFER_SIZE=%d) ===\n\n", BUFFER_SIZE);
	
	run_test_fork("Test basique (3 lignes)", test_basic_3_lines);
	run_test_fork("Test fichier vide", test_empty_file);
	run_test_fork("Test une ligne sans newline", test_single_no_nl);
	run_test_fork("Test une ligne avec newline", test_single_with_nl);
	run_test_fork("Test longue ligne (53248 chars)", test_long_line);
	run_test_fork("Test 10 lignes vides", test_empty_lines);
	run_test_fork("Test pas de newline final", test_no_nl_final);
	run_test_fork("Test multi-fd", test_multi_fd);
	run_test_fork("Test fd invalide", test_invalid_fd);
	run_test_fork("Stress test 1000 lignes", test_stress);
	
	printf("Total: %d tests\n", total_tests);
	printf("Passed: %d\n", total_tests - failed_tests);
	
	if (failed_tests > 0)
	{
		printf("Failed: %d\n", failed_tests);
		return (1);
	}
	else
	{
		return (0);
	}
}
