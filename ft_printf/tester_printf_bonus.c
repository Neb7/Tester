/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tester_printf_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: benpicar <benpicar@student.42mulhouse.fr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/23 17:00:00 by benpicar          #+#    #+#             */
/*   Updated: 2026/01/23 17:30:52 by benpicar         ###   ########.fr       */
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
#define GRAY   "\033[0;90m"
#define NC     "\033[0m"

// Variables globales pour statistiques
int	failed_tests = 0;
int	total_tests = 0;

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
	printf("%-70s", test_name);
	fflush(stdout);
	
	pid_t	pid = fork();
	if (pid == -1)
	{
		printf("%s[ERROR]%s\n", RED, NC);
		failed_tests++;
		return (0);
	}
	
	if (pid == 0)
	{
		int	result = func();
		exit(result);
	}
	
	int	status;
	int	timeout = 20;
	int	elapsed = 0;
	
	while (elapsed < timeout)
	{
		int	ret = waitpid(pid, &status, WNOHANG);
		if (ret == pid)
		{
			if (WIFEXITED(status))
			{
				int	exit_code = WEXITSTATUS(status);
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
				int	sig = WTERMSIG(status);
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
		usleep(100000);
		elapsed++;
	}
	
	kill(pid, SIGKILL);
	waitpid(pid, NULL, 0);
	printf("%s[TIMEOUT]%s\n", YELLOW, NC);
	failed_tests++;
	return (0);
}

// ============================================================================
// TESTS POUR FLAG - (justification à gauche)
// ============================================================================

static inline int	test_flag_minus_string(void)
{
	int	saved_stdout;
	int	ret1;
	int	ret2;

	saved_stdout = redirect_stdout_to_null();
	ret1 = ft_printf("%-10s|\n", "test");
	ret2 = printf("%-10s|\n", "test");
	restore_stdout(saved_stdout);
	if (ret1 != ret2)
		return (1);
	return (0);
}

static inline int	test_flag_minus_int(void)
{
	int	saved_stdout;
	int	ret1;
	int	ret2;

	saved_stdout = redirect_stdout_to_null();
	ret1 = ft_printf("%-10d|\n", 42);
	ret2 = printf("%-10d|\n", 42);
	restore_stdout(saved_stdout);
	if (ret1 != ret2)
		return (1);
	return (0);
}

static inline int	test_flag_minus_negative(void)
{
	int	saved_stdout;
	int	ret1;
	int	ret2;

	saved_stdout = redirect_stdout_to_null();
	ret1 = ft_printf("%-10d|\n", -42);
	ret2 = printf("%-10d|\n", -42);
	restore_stdout(saved_stdout);
	if (ret1 != ret2)
		return (1);
	return (0);
}

static inline int	test_flag_minus_char(void)
{
	int	saved_stdout;
	int	ret1;
	int	ret2;

	saved_stdout = redirect_stdout_to_null();
	ret1 = ft_printf("%-5c|\n", 'A');
	ret2 = printf("%-5c|\n", 'A');
	restore_stdout(saved_stdout);
	if (ret1 != ret2)
		return (1);
	return (0);
}

static inline int	test_flag_minus_hex(void)
{
	int	saved_stdout;
	int	ret1;
	int	ret2;

	saved_stdout = redirect_stdout_to_null();
	ret1 = ft_printf("%-10x|\n", 255);
	ret2 = printf("%-10x|\n", 255);
	restore_stdout(saved_stdout);
	if (ret1 != ret2)
		return (1);
	return (0);
}

// ============================================================================
// TESTS POUR FLAG 0 (padding avec zéros)
// ============================================================================

static inline int	test_flag_zero_int(void)
{
	int	saved_stdout;
	int	ret1;
	int	ret2;

	saved_stdout = redirect_stdout_to_null();
	ret1 = ft_printf("%010d|\n", 42);
	ret2 = printf("%010d|\n", 42);
	restore_stdout(saved_stdout);
	if (ret1 != ret2)
		return (1);
	return (0);
}

static inline int	test_flag_zero_negative(void)
{
	int	saved_stdout;
	int	ret1;
	int	ret2;

	saved_stdout = redirect_stdout_to_null();
	ret1 = ft_printf("%010d|\n", -42);
	ret2 = printf("%010d|\n", -42);
	restore_stdout(saved_stdout);
	if (ret1 != ret2)
		return (1);
	return (0);
}

static inline int	test_flag_zero_hex(void)
{
	int	saved_stdout;
	int	ret1;
	int	ret2;

	saved_stdout = redirect_stdout_to_null();
	ret1 = ft_printf("%010x|\n", 255);
	ret2 = printf("%010x|\n", 255);
	restore_stdout(saved_stdout);
	if (ret1 != ret2)
		return (1);
	return (0);
}

static inline int	test_flag_zero_unsigned(void)
{
	int	saved_stdout;
	int	ret1;
	int	ret2;

	saved_stdout = redirect_stdout_to_null();
	ret1 = ft_printf("%010u|\n", 42);
	ret2 = printf("%010u|\n", 42);
	restore_stdout(saved_stdout);
	if (ret1 != ret2)
		return (1);
	return (0);
}

// ============================================================================
// TESTS POUR LARGEUR DE CHAMP
// ============================================================================

static inline int	test_width_string(void)
{
	int	saved_stdout;
	int	ret1;
	int	ret2;

	saved_stdout = redirect_stdout_to_null();
	ret1 = ft_printf("%20s|\n", "Hello");
	ret2 = printf("%20s|\n", "Hello");
	restore_stdout(saved_stdout);
	if (ret1 != ret2)
		return (1);
	return (0);
}

static inline int	test_width_int(void)
{
	int	saved_stdout;
	int	ret1;
	int	ret2;

	saved_stdout = redirect_stdout_to_null();
	ret1 = ft_printf("%10d|\n", 42);
	ret2 = printf("%10d|\n", 42);
	restore_stdout(saved_stdout);
	if (ret1 != ret2)
		return (1);
	return (0);
}

static inline int	test_width_char(void)
{
	int	saved_stdout;
	int	ret1;
	int	ret2;

	saved_stdout = redirect_stdout_to_null();
	ret1 = ft_printf("%5c|\n", 'X');
	ret2 = printf("%5c|\n", 'X');
	restore_stdout(saved_stdout);
	if (ret1 != ret2)
		return (1);
	return (0);
}

static inline int	test_width_hex(void)
{
	int	saved_stdout;
	int	ret1;
	int	ret2;

	saved_stdout = redirect_stdout_to_null();
	ret1 = ft_printf("%15x|\n", 42);
	ret2 = printf("%15x|\n", 42);
	restore_stdout(saved_stdout);
	if (ret1 != ret2)
		return (1);
	return (0);
}

static inline int	test_width_pointer(void)
{
	int	saved_stdout;
	int	ret1;
	int	ret2;
	int	x;

	x = 42;
	saved_stdout = redirect_stdout_to_null();
	ret1 = ft_printf("%20p|\n", &x);
	ret2 = printf("%20p|\n", &x);
	restore_stdout(saved_stdout);
	if (ret1 != ret2)
		return (1);
	return (0);
}

// ============================================================================
// TESTS POUR PRÉCISION
// ============================================================================

static inline int	test_precision_string(void)
{
	int	saved_stdout;
	int	ret1;
	int	ret2;

	saved_stdout = redirect_stdout_to_null();
	ret1 = ft_printf("%.5s|\n", "HelloWorld");
	ret2 = printf("%.5s|\n", "HelloWorld");
	restore_stdout(saved_stdout);
	if (ret1 != ret2)
		return (1);
	return (0);
}

static inline int	test_precision_int(void)
{
	int	saved_stdout;
	int	ret1;
	int	ret2;

	saved_stdout = redirect_stdout_to_null();
	ret1 = ft_printf("%.10d|\n", 42);
	ret2 = printf("%.10d|\n", 42);
	restore_stdout(saved_stdout);
	if (ret1 != ret2)
		return (1);
	return (0);
}

static inline int	test_precision_zero(void)
{
	int	saved_stdout;
	int	ret1;
	int	ret2;

	saved_stdout = redirect_stdout_to_null();
	ret1 = ft_printf("%.0d|\n", 0);
	ret2 = printf("%.0d|\n", 0);
	restore_stdout(saved_stdout);
	if (ret1 != ret2)
		return (1);
	return (0);
}

static inline int	test_precision_hex(void)
{
	int	saved_stdout;
	int	ret1;
	int	ret2;

	saved_stdout = redirect_stdout_to_null();
	ret1 = ft_printf("%.8x|\n", 42);
	ret2 = printf("%.8x|\n", 42);
	restore_stdout(saved_stdout);
	if (ret1 != ret2)
		return (1);
	return (0);
}

static inline int	test_precision_negative(void)
{
	int	saved_stdout;
	int	ret1;
	int	ret2;

	saved_stdout = redirect_stdout_to_null();
	ret1 = ft_printf("%.10d|\n", -42);
	ret2 = printf("%.10d|\n", -42);
	restore_stdout(saved_stdout);
	if (ret1 != ret2)
		return (1);
	return (0);
}

// ============================================================================
// TESTS POUR FLAG # (forme alternative)
// ============================================================================

static inline int	test_flag_hash_hex_lower(void)
{
	int	saved_stdout;
	int	ret1;
	int	ret2;

	saved_stdout = redirect_stdout_to_null();
	ret1 = ft_printf("%#x|\n", 42);
	ret2 = printf("%#x|\n", 42);
	restore_stdout(saved_stdout);
	if (ret1 != ret2)
		return (1);
	return (0);
}

static inline int	test_flag_hash_hex_upper(void)
{
	int	saved_stdout;
	int	ret1;
	int	ret2;

	saved_stdout = redirect_stdout_to_null();
	ret1 = ft_printf("%#X|\n", 42);
	ret2 = printf("%#X|\n", 42);
	restore_stdout(saved_stdout);
	if (ret1 != ret2)
		return (1);
	return (0);
}

static inline int	test_flag_hash_zero(void)
{
	int	saved_stdout;
	int	ret1;
	int	ret2;

	saved_stdout = redirect_stdout_to_null();
	ret1 = ft_printf("%#x|\n", 0);
	ret2 = printf("%#x|\n", 0);
	restore_stdout(saved_stdout);
	if (ret1 != ret2)
		return (1);
	return (0);
}

static inline int	test_flag_hash_with_width(void)
{
	int	saved_stdout;
	int	ret1;
	int	ret2;

	saved_stdout = redirect_stdout_to_null();
	ret1 = ft_printf("%#10x|\n", 42);
	ret2 = printf("%#10x|\n", 42);
	restore_stdout(saved_stdout);
	if (ret1 != ret2)
		return (1);
	return (0);
}

// ============================================================================
// TESTS POUR FLAG + (signe)
// ============================================================================

static inline int	test_flag_plus_positive(void)
{
	int	saved_stdout;
	int	ret1;
	int	ret2;

	saved_stdout = redirect_stdout_to_null();
	ret1 = ft_printf("%+d|\n", 42);
	ret2 = printf("%+d|\n", 42);
	restore_stdout(saved_stdout);
	if (ret1 != ret2)
		return (1);
	return (0);
}

static inline int	test_flag_plus_negative(void)
{
	int	saved_stdout;
	int	ret1;
	int	ret2;

	saved_stdout = redirect_stdout_to_null();
	ret1 = ft_printf("%+d|\n", -42);
	ret2 = printf("%+d|\n", -42);
	restore_stdout(saved_stdout);
	if (ret1 != ret2)
		return (1);
	return (0);
}

static inline int	test_flag_plus_zero(void)
{
	int	saved_stdout;
	int	ret1;
	int	ret2;

	saved_stdout = redirect_stdout_to_null();
	ret1 = ft_printf("%+d|\n", 0);
	ret2 = printf("%+d|\n", 0);
	restore_stdout(saved_stdout);
	if (ret1 != ret2)
		return (1);
	return (0);
}

static inline int	test_flag_plus_with_width(void)
{
	int	saved_stdout;
	int	ret1;
	int	ret2;

	saved_stdout = redirect_stdout_to_null();
	ret1 = ft_printf("%+10d|\n", 42);
	ret2 = printf("%+10d|\n", 42);
	restore_stdout(saved_stdout);
	if (ret1 != ret2)
		return (1);
	return (0);
}

// ============================================================================
// TESTS POUR FLAG ESPACE
// ============================================================================

static inline int	test_flag_space_positive(void)
{
	int	saved_stdout;
	int	ret1;
	int	ret2;

	saved_stdout = redirect_stdout_to_null();
	ret1 = ft_printf("% d|\n", 42);
	ret2 = printf("% d|\n", 42);
	restore_stdout(saved_stdout);
	if (ret1 != ret2)
		return (1);
	return (0);
}

static inline int	test_flag_space_negative(void)
{
	int	saved_stdout;
	int	ret1;
	int	ret2;

	saved_stdout = redirect_stdout_to_null();
	ret1 = ft_printf("% d|\n", -42);
	ret2 = printf("% d|\n", -42);
	restore_stdout(saved_stdout);
	if (ret1 != ret2)
		return (1);
	return (0);
}

static inline int	test_flag_space_zero(void)
{
	int	saved_stdout;
	int	ret1;
	int	ret2;

	saved_stdout = redirect_stdout_to_null();
	ret1 = ft_printf("% d|\n", 0);
	ret2 = printf("% d|\n", 0);
	restore_stdout(saved_stdout);
	if (ret1 != ret2)
		return (1);
	return (0);
}

static inline int	test_flag_space_with_width(void)
{
	int	saved_stdout;
	int	ret1;
	int	ret2;

	saved_stdout = redirect_stdout_to_null();
	ret1 = ft_printf("% 10d|\n", 42);
	ret2 = printf("% 10d|\n", 42);
	restore_stdout(saved_stdout);
	if (ret1 != ret2)
		return (1);
	return (0);
}

// ============================================================================
// TESTS COMBINÉS
// ============================================================================

static inline int	test_combo_width_precision(void)
{
	int	saved_stdout;
	int	ret1;
	int	ret2;

	saved_stdout = redirect_stdout_to_null();
	ret1 = ft_printf("%10.5d|\n", 42);
	ret2 = printf("%10.5d|\n", 42);
	restore_stdout(saved_stdout);
	if (ret1 != ret2)
		return (1);
	return (0);
}

static inline int	test_combo_minus_width(void)
{
	int	saved_stdout;
	int	ret1;
	int	ret2;

	saved_stdout = redirect_stdout_to_null();
	ret1 = ft_printf("%-10.5d|\n", 42);
	ret2 = printf("%-10.5d|\n", 42);
	restore_stdout(saved_stdout);
	if (ret1 != ret2)
		return (1);
	return (0);
}

static inline int	test_combo_zero_width(void)
{
	int	saved_stdout;
	int	ret1;
	int	ret2;

	saved_stdout = redirect_stdout_to_null();
	ret1 = ft_printf("%010.5d|\n", 42);
	ret2 = printf("%010.5d|\n", 42);
	restore_stdout(saved_stdout);
	if (ret1 != ret2)
		return (1);
	return (0);
}

static inline int	test_combo_plus_width_precision(void)
{
	int	saved_stdout;
	int	ret1;
	int	ret2;

	saved_stdout = redirect_stdout_to_null();
	ret1 = ft_printf("%+10.5d|\n", 42);
	ret2 = printf("%+10.5d|\n", 42);
	restore_stdout(saved_stdout);
	if (ret1 != ret2)
		return (1);
	return (0);
}

static inline int	test_combo_hash_width_hex(void)
{
	int	saved_stdout;
	int	ret1;
	int	ret2;

	saved_stdout = redirect_stdout_to_null();
	ret1 = ft_printf("%#10x|\n", 42);
	ret2 = printf("%#10x|\n", 42);
	restore_stdout(saved_stdout);
	if (ret1 != ret2)
		return (1);
	return (0);
}

static inline int	test_combo_hash_precision_hex(void)
{
	int	saved_stdout;
	int	ret1;
	int	ret2;

	saved_stdout = redirect_stdout_to_null();
	ret1 = ft_printf("%#.8x|\n", 42);
	ret2 = printf("%#.8x|\n", 42);
	restore_stdout(saved_stdout);
	if (ret1 != ret2)
		return (1);
	return (0);
}

static inline int	test_combo_minus_plus(void)
{
	int	saved_stdout;
	int	ret1;
	int	ret2;

	saved_stdout = redirect_stdout_to_null();
	ret1 = ft_printf("%-+10d|\n", 42);
	ret2 = printf("%-+10d|\n", 42);
	restore_stdout(saved_stdout);
	if (ret1 != ret2)
		return (1);
	return (0);
}

static inline int	test_combo_space_width(void)
{
	int	saved_stdout;
	int	ret1;
	int	ret2;

	saved_stdout = redirect_stdout_to_null();
	ret1 = ft_printf("% 10d|\n", 42);
	ret2 = printf("% 10d|\n", 42);
	restore_stdout(saved_stdout);
	if (ret1 != ret2)
		return (1);
	return (0);
}

// ============================================================================
// TESTS AVANCÉS
// ============================================================================

static inline int	test_advanced_string_width_precision(void)
{
	int	saved_stdout;
	int	ret1;
	int	ret2;

	saved_stdout = redirect_stdout_to_null();
	ret1 = ft_printf("%20.10s|\n", "Hello World!");
	ret2 = printf("%20.10s|\n", "Hello World!");
	restore_stdout(saved_stdout);
	if (ret1 != ret2)
		return (1);
	return (0);
}

static inline int	test_advanced_negative_width_precision(void)
{
	int	saved_stdout;
	int	ret1;
	int	ret2;

	saved_stdout = redirect_stdout_to_null();
	ret1 = ft_printf("%10.5d|\n", -42);
	ret2 = printf("%10.5d|\n", -42);
	restore_stdout(saved_stdout);
	if (ret1 != ret2)
		return (1);
	return (0);
}

static inline int	test_advanced_multiple_flags(void)
{
	int	saved_stdout;
	int	ret1;
	int	ret2;

	saved_stdout = redirect_stdout_to_null();
	ret1 = ft_printf("%-+10.5d|%#010x|% 10d|\n", 42, 255, 123);
	ret2 = printf("%-+10.5d|%#010x|% 10d|\n", 42, 255, 123);
	restore_stdout(saved_stdout);
	if (ret1 != ret2)
		return (1);
	return (0);
}

static inline int	test_advanced_hex_upper_combo(void)
{
	int	saved_stdout;
	int	ret1;
	int	ret2;

	saved_stdout = redirect_stdout_to_null();
	ret1 = ft_printf("%#-15.8X|\n", 42);
	ret2 = printf("%#-15.8X|\n", 42);
	restore_stdout(saved_stdout);
	if (ret1 != ret2)
		return (1);
	return (0);
}

static inline int	test_advanced_int_max_min(void)
{
	int	saved_stdout;
	int	ret1;
	int	ret2;

	saved_stdout = redirect_stdout_to_null();
	ret1 = ft_printf("%+20.15d|%+20.15d|\n", INT_MAX, INT_MIN);
	ret2 = printf("%+20.15d|%+20.15d|\n", INT_MAX, INT_MIN);
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
	printf("\n%s╔═══════════════════════════════════════════════════════════════════════════════╗%s\n", CYAN, NC);
	printf("%s║ %-77s ║%s\n", CYAN, title, NC);
	printf("%s╚═══════════════════════════════════════════════════════════════════════════════╝%s\n\n", CYAN, NC);
}

static inline void	print_summary(void)
{
	printf("\n%s╔═══════════════════════════════════════════════════════════════════════════════╗%s\n", BLUE, NC);
	printf("%s║ %-77s ║%s\n", BLUE, "SUMMARY", NC);
	printf("%s╠═══════════════════════════════════════════════════════════════════════════════╣%s\n", BLUE, NC);
	
	int	passed = total_tests - failed_tests;
	double	success_rate = (total_tests > 0) ? (100.0 * passed / total_tests) : 0.0;
	
	char	buffer[78];
	snprintf(buffer, sizeof(buffer), "Total tests: %d", total_tests);
	printf("%s║ %-77s ║%s\n", BLUE, buffer, NC);
	
	snprintf(buffer, sizeof(buffer), "Passed: %s%d%s", GREEN, passed, BLUE);
	printf("%s║ %-91s ║%s\n", BLUE, buffer, NC);
	
	snprintf(buffer, sizeof(buffer), "Failed: %s%d%s", RED, failed_tests, BLUE);
	printf("%s║ %-91s ║%s\n", BLUE, buffer, NC);
	
	snprintf(buffer, sizeof(buffer), "Success rate: %.2f%%", success_rate);
	printf("%s║ %-77s ║%s\n", BLUE, buffer, NC);
	
	printf("%s╚═══════════════════════════════════════════════════════════════════════════════╝%s\n\n", BLUE, NC);
	
	if (failed_tests == 0)
	{
		printf("%s🎉 All bonus tests passed! Perfect implementation! 🎉%s\n\n", GREEN, NC);
	}
	else
	{
		printf("%s⚠️  Some bonus tests failed. Review the output above. ⚠️%s\n\n", YELLOW, NC);
	}
}

int	main(void)
{
	printf("\n%s╔═══════════════════════════════════════════════════════════════════════════════╗%s\n", BLUE, NC);
	printf("%s║                         FT_PRINTF BONUS TESTER                                ║%s\n", BLUE, NC);
	printf("%s╚═══════════════════════════════════════════════════════════════════════════════╝%s\n", BLUE, NC);
	
	// Tests flag -
	print_header("TESTS: Flag - (left justify)");
	run_test_fork("test_flag_minus_string", test_flag_minus_string);
	run_test_fork("test_flag_minus_int", test_flag_minus_int);
	run_test_fork("test_flag_minus_negative", test_flag_minus_negative);
	run_test_fork("test_flag_minus_char", test_flag_minus_char);
	run_test_fork("test_flag_minus_hex", test_flag_minus_hex);
	
	// Tests flag 0
	print_header("TESTS: Flag 0 (zero padding)");
	run_test_fork("test_flag_zero_int", test_flag_zero_int);
	run_test_fork("test_flag_zero_negative", test_flag_zero_negative);
	run_test_fork("test_flag_zero_hex", test_flag_zero_hex);
	run_test_fork("test_flag_zero_unsigned", test_flag_zero_unsigned);
	
	// Tests largeur
	print_header("TESTS: Width (field width)");
	run_test_fork("test_width_string", test_width_string);
	run_test_fork("test_width_int", test_width_int);
	run_test_fork("test_width_char", test_width_char);
	run_test_fork("test_width_hex", test_width_hex);
	run_test_fork("test_width_pointer", test_width_pointer);
	
	// Tests précision
	print_header("TESTS: Precision");
	run_test_fork("test_precision_string", test_precision_string);
	run_test_fork("test_precision_int", test_precision_int);
	run_test_fork("test_precision_zero", test_precision_zero);
	run_test_fork("test_precision_hex", test_precision_hex);
	run_test_fork("test_precision_negative", test_precision_negative);
	
	// Tests flag #
	print_header("TESTS: Flag # (alternate form)");
	run_test_fork("test_flag_hash_hex_lower", test_flag_hash_hex_lower);
	run_test_fork("test_flag_hash_hex_upper", test_flag_hash_hex_upper);
	run_test_fork("test_flag_hash_zero", test_flag_hash_zero);
	run_test_fork("test_flag_hash_with_width", test_flag_hash_with_width);
	
	// Tests flag +
	print_header("TESTS: Flag + (sign)");
	run_test_fork("test_flag_plus_positive", test_flag_plus_positive);
	run_test_fork("test_flag_plus_negative", test_flag_plus_negative);
	run_test_fork("test_flag_plus_zero", test_flag_plus_zero);
	run_test_fork("test_flag_plus_with_width", test_flag_plus_with_width);
	
	// Tests flag espace
	print_header("TESTS: Flag ' ' (space)");
	run_test_fork("test_flag_space_positive", test_flag_space_positive);
	run_test_fork("test_flag_space_negative", test_flag_space_negative);
	run_test_fork("test_flag_space_zero", test_flag_space_zero);
	run_test_fork("test_flag_space_with_width", test_flag_space_with_width);
	
	// Tests combinés
	print_header("TESTS: Combined flags");
	run_test_fork("test_combo_width_precision", test_combo_width_precision);
	run_test_fork("test_combo_minus_width", test_combo_minus_width);
	run_test_fork("test_combo_zero_width", test_combo_zero_width);
	run_test_fork("test_combo_plus_width_precision", test_combo_plus_width_precision);
	run_test_fork("test_combo_hash_width_hex", test_combo_hash_width_hex);
	run_test_fork("test_combo_hash_precision_hex", test_combo_hash_precision_hex);
	run_test_fork("test_combo_minus_plus", test_combo_minus_plus);
	run_test_fork("test_combo_space_width", test_combo_space_width);
	
	// Tests avancés
	print_header("TESTS: Advanced combinations");
	run_test_fork("test_advanced_string_width_precision", test_advanced_string_width_precision);
	run_test_fork("test_advanced_negative_width_precision", test_advanced_negative_width_precision);
	run_test_fork("test_advanced_multiple_flags", test_advanced_multiple_flags);
	run_test_fork("test_advanced_hex_upper_combo", test_advanced_hex_upper_combo);
	run_test_fork("test_advanced_int_max_min", test_advanced_int_max_min);
	
	// Résumé
	print_summary();
	
	return (failed_tests > 0 ? 1 : 0);
}
