/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tester_printf.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: benpicar <benpicar@student.42mulhouse.fr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/23 10:00:00 by benpicar          #+#    #+#             */
/*   Updated: 2026/01/23 17:32:22 by benpicar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <limits.h>
#include <fcntl.h>

#define RED    "\033[0;31m"
#define GREEN  "\033[0;32m"
#define YELLOW "\033[0;33m"
#define BLUE   "\033[0;34m"
#define CYAN   "\033[0;36m"
#define NC     "\033[0m"

// Variables globales pour statistiques
int failed_tests = 0;
int total_tests = 0;

typedef int (*test_func)(void);

// Fonction pour rediriger stdout vers /dev/null
static inline int	redirect_stdout_to_null(void)
{
	int	saved_stdout;
	int	devnull;

	saved_stdout = dup(STDOUT_FILENO);
	devnull = open("/dev/null", O_WRONLY);
	if (devnull == -1)
		return (-1);
	dup2(devnull, STDOUT_FILENO);
	close(devnull);
	return (saved_stdout);
}

// Fonction pour restaurer stdout
static inline void	restore_stdout(int saved_stdout)
{
	if (saved_stdout != -1)
	{
		dup2(saved_stdout, STDOUT_FILENO);
		close(saved_stdout);
	}
}

// Fonction fork pour exécuter un test avec protection
static inline int	run_test_fork(const char	*test_name, test_func func)
{
	total_tests++;
	printf("%-60s", test_name);
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
	
	int status;
	int timeout = 20; // 2 secondes
	int elapsed = 0;
	
	while (elapsed < timeout)
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

// ============================================================================
// TESTS EDGE CASES
// ============================================================================

static inline int	test_null_format(void)
{
	int	saved_stdout;
	int	ret1;

	saved_stdout = redirect_stdout_to_null();
	ret1 = ft_printf(NULL);
	restore_stdout(saved_stdout);
	if (ret1 != -1)
		return (1);
	return (0);
}

static inline int	test_incomplete_percent(void)
{
	int		saved_stdout;
	int		ret1;
	char	*str;
	int		i;

	str = "test %";
	i = 42;
	saved_stdout = redirect_stdout_to_null();
	ret1 = ft_printf(str, i);
	restore_stdout(saved_stdout);
	if (ret1 != -1)
		return (1);
	return (0);
}

// ============================================================================
// TESTS POUR %c (caractère)
// ============================================================================

static inline int	test_char_simple(void)
{
	int	saved_stdout;
	int	ret1;
	int	ret2;

	saved_stdout = redirect_stdout_to_null();
	ret1 = ft_printf("Test: %c\n", 'A');
	ret2 = printf("Test: %c\n", 'A');

	restore_stdout(saved_stdout);
	if (ret1 != ret2)
		return (1);
	return (0);
}

static inline int	test_char_multiple(void)
{
	int	saved_stdout;
	int	ret1;
	int	ret2;

	saved_stdout = redirect_stdout_to_null();
	ret1 = ft_printf("%c %c %c\n", 'a', 'b', 'c');
	ret2 = printf("%c %c %c\n", 'a', 'b', 'c');

	restore_stdout(saved_stdout);
	if (ret1 != ret2)
		return (1);
	return (0);
}

static inline int	test_char_null(void)
{
	int	saved_stdout;
	int	ret1;
	int	ret2;

	saved_stdout = redirect_stdout_to_null();
	ret1 = ft_printf("Null: %c!\n", '\0');
	ret2 = printf("Null: %c!\n", '\0');

	restore_stdout(saved_stdout);
	if (ret1 != ret2)
		return (1);
	return (0);
}

static inline int	test_char_special(void)
{
	int	saved_stdout;
	int	ret1;
	int	ret2;

	saved_stdout = redirect_stdout_to_null();
	ret1 = ft_printf("Tab: %c, Newline: %c, Space: %c\n", '\t', '\n', ' ');
	ret2 = printf("Tab: %c, Newline: %c, Space: %c\n", '\t', '\n', ' ');

	restore_stdout(saved_stdout);
	if (ret1 != ret2)
		return (1);
	return (0);
}

// ============================================================================
// TESTS POUR %s (string)
// ============================================================================

static inline int	test_string_simple(void)
{
	int	saved_stdout;
	int	ret1;
	int	ret2;

	saved_stdout = redirect_stdout_to_null();
	ret1 = ft_printf("String: %s\n", "Hello");
	ret2 = printf("String: %s\n", "Hello");

	restore_stdout(saved_stdout);
	if (ret1 != ret2)
		return (1);
	return (0);
}

static inline int	test_string_empty(void)
{
	int	saved_stdout;
	int	ret1;
	int	ret2;

	saved_stdout = redirect_stdout_to_null();
	ret1 = ft_printf("Empty: %s!\n", "");
	ret2 = printf("Empty: %s!\n", "");

	restore_stdout(saved_stdout);
	if (ret1 != ret2)
		return (1);
	return (0);
}

static inline int	test_string_null(void)
{
	int	saved_stdout;
	int	ret1;
	int	ret2;

	saved_stdout = redirect_stdout_to_null();
	ret1 = ft_printf("Null: %s!\n", (char *)NULL);
	ret2 = printf("Null: %s!\n", (char *)NULL);

	restore_stdout(saved_stdout);
	if (ret1 != ret2)
		return (1);
	return (0);
}

static inline int	test_string_multiple(void)
{
	int	saved_stdout;
	int	ret1;
	int	ret2;

	saved_stdout = redirect_stdout_to_null();
	ret1 = ft_printf("%s %s %s\n", "Hello", "World", "42");
	ret2 = printf("%s %s %s\n", "Hello", "World", "42");

	restore_stdout(saved_stdout);
	if (ret1 != ret2)
		return (1);
	return (0);
}

static inline int	test_string_long(void)
{
	int	saved_stdout;
	int	ret1;
	int	ret2;
	char	*long_str;

	long_str = "This is a very long string to test how ft_printf handles longer strings";
	saved_stdout = redirect_stdout_to_null();
	ret1 = ft_printf("%s\n", long_str);
	ret2 = printf("%s\n", long_str);

	restore_stdout(saved_stdout);
	if (ret1 != ret2)
		return (1);
	return (0);
}

// ============================================================================
// TESTS POUR %p (pointeur)
// ============================================================================

static inline int	test_pointer_simple(void)
{
	int	saved_stdout;
	int	ret1;
	int	ret2;
	int	x;

	x = 42;
	saved_stdout = redirect_stdout_to_null();
	ret1 = ft_printf("Ptr: %p\n", &x);
	ret2 = printf("Ptr: %p\n", &x);

	restore_stdout(saved_stdout);
	if (ret1 != ret2)
		return (1);
	return (0);
}

static inline int	test_pointer_null(void)
{
	int	saved_stdout;
	int	ret1;
	int	ret2;

	saved_stdout = redirect_stdout_to_null();
	ret1 = ft_printf("Null ptr: %p\n", NULL);
	ret2 = printf("Null ptr: %p\n", NULL);

	restore_stdout(saved_stdout);
	if (ret1 != ret2)
		return (1);
	return (0);
}

static inline int	test_pointer_multiple(void)
{
	int	saved_stdout;
	int	ret1;
	int	ret2;
	int	a;
	int	b;
	int	c;

	a = 1;
	b = 2;
	c = 3;
	saved_stdout = redirect_stdout_to_null();
	ret1 = ft_printf("%p %p %p\n", &a, &b, &c);
	ret2 = printf("%p %p %p\n", &a, &b, &c);
	restore_stdout(saved_stdout);
	if (ret1 != ret2)
		return (1);
	return (0);
}

// ============================================================================
// TESTS POUR %d et %i (entiers décimaux)
// ============================================================================

static inline int	test_int_simple(void)
{
	int	saved_stdout;
	int	ret1;
	int	ret2;

	saved_stdout = redirect_stdout_to_null();
	ret1 = ft_printf("Int: %d\n", 42);
	ret2 = printf("Int: %d\n", 42);

	restore_stdout(saved_stdout);
	if (ret1 != ret2)
		return (1);
	return (0);
}

static inline int	test_int_zero(void)
{
	int	saved_stdout;
	int	ret1;
	int	ret2;

	saved_stdout = redirect_stdout_to_null();
	ret1 = ft_printf("Zero: %d\n", 0);
	ret2 = printf("Zero: %d\n", 0);

	restore_stdout(saved_stdout);
	if (ret1 != ret2)
		return (1);
	return (0);
}

static inline int	test_int_negative(void)
{
	int	saved_stdout;
	int	ret1;
	int	ret2;

	saved_stdout = redirect_stdout_to_null();
	ret1 = ft_printf("Negative: %d\n", -42);
	ret2 = printf("Negative: %d\n", -42);

	restore_stdout(saved_stdout);
	if (ret1 != ret2)
		return (1);
	return (0);
}

static inline int	test_int_max(void)
{
	int	saved_stdout;
	int	ret1;
	int	ret2;

	saved_stdout = redirect_stdout_to_null();
	ret1 = ft_printf("Max: %d\n", INT_MAX);
	ret2 = printf("Max: %d\n", INT_MAX);

	restore_stdout(saved_stdout);
	if (ret1 != ret2)
		return (1);
	return (0);
}

static inline int	test_int_min(void)
{
	int	saved_stdout;
	int	ret1;
	int	ret2;

	saved_stdout = redirect_stdout_to_null();
	ret1 = ft_printf("Min: %d\n", INT_MIN);
	ret2 = printf("Min: %d\n", INT_MIN);

	restore_stdout(saved_stdout);
	if (ret1 != ret2)
		return (1);
	return (0);
}

static inline int	test_int_i_simple(void)
{
	int	saved_stdout;
	int	ret1;
	int	ret2;

	saved_stdout = redirect_stdout_to_null();
	ret1 = ft_printf("Int i: %i\n", 123);
	ret2 = printf("Int i: %i\n", 123);

	restore_stdout(saved_stdout);
	if (ret1 != ret2)
		return (1);
	return (0);
}

static inline int	test_int_multiple(void)
{
	int	saved_stdout;
	int	ret1;
	int	ret2;

	saved_stdout = redirect_stdout_to_null();
	ret1 = ft_printf("%d %d %d\n", 1, -2, 3);
	ret2 = printf("%d %d %d\n", 1, -2, 3);

	restore_stdout(saved_stdout);
	if (ret1 != ret2)
		return (1);
	return (0);
}

// ============================================================================
// TESTS POUR %u (unsigned)
// ============================================================================

static inline int	test_unsigned_simple(void)
{
	int	saved_stdout;
	int	ret1;
	int	ret2;

	saved_stdout = redirect_stdout_to_null();
	ret1 = ft_printf("Unsigned: %u\n", 42);
	ret2 = printf("Unsigned: %u\n", 42);

	restore_stdout(saved_stdout);
	if (ret1 != ret2)
		return (1);
	return (0);
}

static inline int	test_unsigned_zero(void)
{
	int	saved_stdout;
	int	ret1;
	int	ret2;

	saved_stdout = redirect_stdout_to_null();
	ret1 = ft_printf("Zero: %u\n", 0);
	ret2 = printf("Zero: %u\n", 0);

	restore_stdout(saved_stdout);
	if (ret1 != ret2)
		return (1);
	return (0);
}

static inline int	test_unsigned_max(void)
{
	int	saved_stdout;
	int	ret1;
	int	ret2;

	saved_stdout = redirect_stdout_to_null();
	ret1 = ft_printf("Max: %u\n", UINT_MAX);
	ret2 = printf("Max: %u\n", UINT_MAX);

	restore_stdout(saved_stdout);
	if (ret1 != ret2)
		return (1);
	return (0);
}

static inline int	test_unsigned_negative(void)
{
	int	saved_stdout;
	int	ret1;
	int	ret2;

	saved_stdout = redirect_stdout_to_null();
	ret1 = ft_printf("Negative: %u\n", -1);
	ret2 = printf("Negative: %u\n", -1);

	restore_stdout(saved_stdout);
	if (ret1 != ret2)
		return (1);
	return (0);
}

// ============================================================================
// TESTS POUR %x (hexadécimal minuscule)
// ============================================================================

static inline int	test_hex_lower_simple(void)
{
	int	saved_stdout;
	int	ret1;
	int	ret2;

	saved_stdout = redirect_stdout_to_null();
	ret1 = ft_printf("Hex: %x\n", 42);
	ret2 = printf("Hex: %x\n", 42);

	restore_stdout(saved_stdout);
	if (ret1 != ret2)
		return (1);
	return (0);
}

static inline int	test_hex_lower_zero(void)
{
	int	saved_stdout;
	int	ret1;
	int	ret2;

	saved_stdout = redirect_stdout_to_null();
	ret1 = ft_printf("Zero: %x\n", 0);
	ret2 = printf("Zero: %x\n", 0);

	restore_stdout(saved_stdout);
	if (ret1 != ret2)
		return (1);
	return (0);
}

static inline int	test_hex_lower_max(void)
{
	int	saved_stdout;
	int	ret1;
	int	ret2;

	saved_stdout = redirect_stdout_to_null();
	ret1 = ft_printf("Max: %x\n", UINT_MAX);
	ret2 = printf("Max: %x\n", UINT_MAX);

	restore_stdout(saved_stdout);
	if (ret1 != ret2)
		return (1);
	return (0);
}

static inline int	test_hex_lower_letters(void)
{
	int	saved_stdout;
	int	ret1;
	int	ret2;

	saved_stdout = redirect_stdout_to_null();
	ret1 = ft_printf("Letters: %x\n", 0xabcdef);
	ret2 = printf("Letters: %x\n", 0xabcdef);

	restore_stdout(saved_stdout);
	if (ret1 != ret2)
		return (1);
	return (0);
}

// ============================================================================
// TESTS POUR %X (hexadécimal majuscule)
// ============================================================================

static inline int	test_hex_upper_simple(void)
{
	int	saved_stdout;
	int	ret1;
	int	ret2;

	saved_stdout = redirect_stdout_to_null();
	ret1 = ft_printf("Hex: %X\n", 42);
	ret2 = printf("Hex: %X\n", 42);

	restore_stdout(saved_stdout);
	if (ret1 != ret2)
		return (1);
	return (0);
}

static inline int	test_hex_upper_zero(void)
{
	int	saved_stdout;
	int	ret1;
	int	ret2;

	saved_stdout = redirect_stdout_to_null();
	ret1 = ft_printf("Zero: %X\n", 0);
	ret2 = printf("Zero: %X\n", 0);

	restore_stdout(saved_stdout);
	if (ret1 != ret2)
		return (1);
	return (0);
}

static inline int	test_hex_upper_max(void)
{
	int	saved_stdout;
	int	ret1;
	int	ret2;

	saved_stdout = redirect_stdout_to_null();
	ret1 = ft_printf("Max: %X\n", UINT_MAX);
	ret2 = printf("Max: %X\n", UINT_MAX);

	restore_stdout(saved_stdout);
	if (ret1 != ret2)
		return (1);
	return (0);
}

static inline int	test_hex_upper_letters(void)
{
	int	saved_stdout;
	int	ret1;
	int	ret2;

	saved_stdout = redirect_stdout_to_null();
	ret1 = ft_printf("Letters: %X\n", 0xABCDEF);
	ret2 = printf("Letters: %X\n", 0xABCDEF);

	restore_stdout(saved_stdout);
	if (ret1 != ret2)
		return (1);
	return (0);
}

// ============================================================================
// TESTS POUR %% (pourcentage)
// ============================================================================

static inline int	test_percent_simple(void)
{
	int	saved_stdout;
	int	ret1;
	int	ret2;

	saved_stdout = redirect_stdout_to_null();
	ret1 = ft_printf("Percent: %%\n");
	ret2 = printf("Percent: %%\n");

	restore_stdout(saved_stdout);
	if (ret1 != ret2)
		return (1);
	return (0);
}

static inline int	test_percent_multiple(void)
{
	int	saved_stdout;
	int	ret1;
	int	ret2;

	saved_stdout = redirect_stdout_to_null();
	ret1 = ft_printf("%%: %% %% %%\n");
	ret2 = printf("%%: %% %% %%\n");

	restore_stdout(saved_stdout);
	if (ret1 != ret2)
		return (1);
	return (0);
}

// ============================================================================
// TESTS MIXTES
// ============================================================================

static inline int	test_mix_all(void)
{
	int	saved_stdout;
	int	ret1;
	int	ret2;
	int	x;

	x = 42;
	saved_stdout = redirect_stdout_to_null();
	ret1 = ft_printf("Mix: %c %s %p %d %i %u %x %X %%\n", 'A', "test", &x, 42, -42, 123, 0xff, 0xFF);
	ret2 = printf("Mix: %c %s %p %d %i %u %x %X %%\n", 'A', "test", &x, 42, -42, 123, 0xff, 0xFF);
	restore_stdout(saved_stdout);
	if (ret1 != ret2)
		return (1);
	return (0);
}

static inline int	test_mix_complex(void)
{
	int	saved_stdout;
	int	ret1;
	int	ret2;

	saved_stdout = redirect_stdout_to_null();
	ret1 = ft_printf("Result: %d + %d = %d (0x%x)\n", 10, 32, 42, 42);
	ret2 = printf("Result: %d + %d = %d (0x%x)\n", 10, 32, 42, 42);

	restore_stdout(saved_stdout);
	if (ret1 != ret2)
		return (1);
	return (0);
}

static inline int	test_no_format(void)
{
	int	saved_stdout;
	int	ret1;
	int	ret2;

	saved_stdout = redirect_stdout_to_null();
	ret1 = ft_printf("No format specifiers here!\n");
	ret2 = printf("No format specifiers here!\n");

	restore_stdout(saved_stdout);
	if (ret1 != ret2)
		return (1);
	return (0);
}

static inline int	test_empty_string(void)
{
	int	saved_stdout;
	int	ret1;
	int	ret2;

	saved_stdout = redirect_stdout_to_null();
	ret1 = ft_printf("");
	ret2 = printf("");

	restore_stdout(saved_stdout);
	if (ret1 != ret2)
		return (1);
	return (0);
}

// ============================================================================
// FONCTION PRINCIPALE
// ============================================================================

static inline void	print_header(const char	*title)
{
	printf("\n%s╔══════════════════════════════════════════════════════════════════════════╗%s\n", CYAN, NC);
	printf("%s║ %-70s ║%s\n", CYAN, title, NC);
	printf("%s╚══════════════════════════════════════════════════════════════════════════╝%s\n\n", CYAN, NC);
}

static inline void	print_summary(void)
{
	printf("\n%s╔════════════════════════════════════════════════════════════════════════╗%s\n", BLUE, NC);
	printf("%s║ %-70s ║%s\n", BLUE, "SUMMARY", NC);
	printf("%s╠════════════════════════════════════════════════════════════════════════╣%s\n", BLUE, NC);
	
	int passed = total_tests - failed_tests;
	double success_rate = (total_tests > 0) ? (100.0 * passed / total_tests) : 0.0;
	
	char buffer[71];
	snprintf(buffer, sizeof(buffer), "Total tests: %d", total_tests);
	printf("%s║ %-70s ║%s\n", BLUE, buffer, NC);
	
	snprintf(buffer, sizeof(buffer), "Passed: %s%d%s", GREEN, passed, BLUE);
	printf("%s║ %-84s ║%s\n", BLUE, buffer, NC);
	
	snprintf(buffer, sizeof(buffer), "Failed: %s%d%s", RED, failed_tests, BLUE);
	printf("%s║ %-84s ║%s\n", BLUE, buffer, NC);
	
	snprintf(buffer, sizeof(buffer), "Success rate: %.2f%%", success_rate);
	printf("%s║ %-70s ║%s\n", BLUE, buffer, NC);
	
	printf("%s╚════════════════════════════════════════════════════════════════════════╝%s\n\n", BLUE, NC);
	
	if (failed_tests == 0)
	{
		printf("%s🎉 All tests passed! Congratulations! 🎉%s\n\n", GREEN, NC);
	}
	else
	{
		printf("%s⚠️  Some tests failed. Review the output above for details. ⚠️%s\n\n", YELLOW, NC);
	}
}

int	main(void)
{
	printf("\n%s╔══════════════════════════════════════════════════════════════════════════╗%s\n", BLUE, NC);
	printf("%s║                          FT_PRINTF TESTER                              ║%s\n", BLUE, NC);
	printf("%s╚══════════════════════════════════════════════════════════════════════════╝%s\n", BLUE, NC);
	
	// Tests edge cases
	print_header("TESTS: Edge Cases");
	run_test_fork("test_null_format", test_null_format);
	run_test_fork("test_incomplete_percent", test_incomplete_percent);
	
	// Tests %c
	print_header("TESTS: %c (character)");
	run_test_fork("test_char_simple", test_char_simple);
	run_test_fork("test_char_multiple", test_char_multiple);
	run_test_fork("test_char_null", test_char_null);
	run_test_fork("test_char_special", test_char_special);
	
	// Tests %s
	print_header("TESTS: %s (string)");
	run_test_fork("test_string_simple", test_string_simple);
	run_test_fork("test_string_empty", test_string_empty);
	run_test_fork("test_string_null", test_string_null);
	run_test_fork("test_string_multiple", test_string_multiple);
	run_test_fork("test_string_long", test_string_long);
	
	// Tests %p
	print_header("TESTS: %p (pointer)");
	run_test_fork("test_pointer_simple", test_pointer_simple);
	run_test_fork("test_pointer_null", test_pointer_null);
	run_test_fork("test_pointer_multiple", test_pointer_multiple);
	
	// Tests %d et %i
	print_header("TESTS: %d and %i (integers)");
	run_test_fork("test_int_simple", test_int_simple);
	run_test_fork("test_int_zero", test_int_zero);
	run_test_fork("test_int_negative", test_int_negative);
	run_test_fork("test_int_max", test_int_max);
	run_test_fork("test_int_min", test_int_min);
	run_test_fork("test_int_i_simple", test_int_i_simple);
	run_test_fork("test_int_multiple", test_int_multiple);
	
	// Tests %u
	print_header("TESTS: %u (unsigned)");
	run_test_fork("test_unsigned_simple", test_unsigned_simple);
	run_test_fork("test_unsigned_zero", test_unsigned_zero);
	run_test_fork("test_unsigned_max", test_unsigned_max);
	run_test_fork("test_unsigned_negative", test_unsigned_negative);
	
	// Tests %x
	print_header("TESTS: %x (hexadecimal lowercase)");
	run_test_fork("test_hex_lower_simple", test_hex_lower_simple);
	run_test_fork("test_hex_lower_zero", test_hex_lower_zero);
	run_test_fork("test_hex_lower_max", test_hex_lower_max);
	run_test_fork("test_hex_lower_letters", test_hex_lower_letters);
	
	// Tests %X
	print_header("TESTS: %X (hexadecimal uppercase)");
	run_test_fork("test_hex_upper_simple", test_hex_upper_simple);
	run_test_fork("test_hex_upper_zero", test_hex_upper_zero);
	run_test_fork("test_hex_upper_max", test_hex_upper_max);
	run_test_fork("test_hex_upper_letters", test_hex_upper_letters);
	
	// Tests %%
	print_header("TESTS: %% (percent)");
	run_test_fork("test_percent_simple", test_percent_simple);
	run_test_fork("test_percent_multiple", test_percent_multiple);
	
	// Tests mixtes
	print_header("TESTS: Mixed conversions");
	run_test_fork("test_mix_all", test_mix_all);
	run_test_fork("test_mix_complex", test_mix_complex);
	run_test_fork("test_no_format", test_no_format);
	run_test_fork("test_empty_string", test_empty_string);
	
	// Résumé
	print_summary();
	
	return (failed_tests > 0 ? 1 : 0);
}
