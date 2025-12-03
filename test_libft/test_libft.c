/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_libft.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: benpicar <benpicar@student.42mulhouse.fr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 03:04:01 by benpicar          #+#    #+#             */
/*   Updated: 2025/12/03 12:40:03 by benpicar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdbool.h>

#define GREEN "\033[32m"
#define RED   "\033[31m"
#define YELLOW "\033[33m"
#define GRAY "\033[90m"
#define NC    "\033[0m"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/wait.h>
#include <signal.h>

// Variable globale pour suivre l'état des tests
int failed_tests = 0;
int total_tests = 0;
int null_tests = 0;
int null_protected = 0;

// Fonction utilitaire pour exécuter un test dans un fork
typedef int (*test_func)(void);

int run_test_fork(const char *test_name, test_func func) {
    total_tests++;
    printf("%s: ", test_name);
    fflush(stdout);
    
    pid_t pid = fork();
    if (pid == -1) {
        printf("%s[ERROR: fork failed]%s\n", RED, NC);
        failed_tests++;
        return 0;
    }
    
    if (pid == 0) {
        // Processus enfant
        int result = func();
        exit(result);
    }
    
    // Processus parent
    int status;
    int timeout = 2; // 2 secondes timeout
    int elapsed = 0;
    
    while (elapsed < timeout) {
        int ret = waitpid(pid, &status, WNOHANG);
        if (ret == pid) {
            if (WIFEXITED(status)) {
                int exit_code = WEXITSTATUS(status);
                if (exit_code == 0) {
                    printf("%s[OK]%s\n", GREEN, NC);
                    return 1;
                } else {
                    printf("%s[KO]%s\n", RED, NC);
                    failed_tests++;
                    return 0;
                }
            } else if (WIFSIGNALED(status)) {
                int sig = WTERMSIG(status);
                printf("%s[CRASH: Signal %d (%s)]%s\n", RED, sig, 
                       sig == SIGSEGV ? "SEGFAULT" : 
                       sig == SIGBUS ? "BUS ERROR" : 
                       sig == SIGABRT ? "ABORT" : "UNKNOWN", NC);
                failed_tests++;
                return 0;
            }
        } else if (ret == -1) {
            printf("%s[ERROR: waitpid failed]%s\n", RED, NC);
            failed_tests++;
            return 0;
        }
        usleep(100000); // 100ms
        elapsed++;
    }
    
    // Timeout
    kill(pid, SIGKILL);
    waitpid(pid, NULL, 0);
    printf("%s[TIMEOUT]%s\n", YELLOW, NC);
    failed_tests++;
    return 0;
}

// Fonction pour tester un test fork sans newline (pour grouper avec tests NULL)
int run_test_fork_inline(const char *test_name, test_func func) {
    total_tests++;
    printf("%s: ", test_name);
    fflush(stdout);
    
    pid_t pid = fork();
    if (pid == -1) {
        printf("%s[ERROR]%s ", RED, NC);
        failed_tests++;
        return 0;
    }
    
    if (pid == 0) {
        int result = func();
        exit(result);
    }
    
    int status;
    int timeout = 2;
    int elapsed = 0;
    
    while (elapsed < timeout) {
        int ret = waitpid(pid, &status, WNOHANG);
        if (ret == pid) {
            if (WIFEXITED(status)) {
                int exit_code = WEXITSTATUS(status);
                if (exit_code == 0) {
                    printf("%s[OK]%s ", GREEN, NC);
                    return 1;
                } else {
                    printf("%s[KO]%s ", RED, NC);
                    failed_tests++;
                    return 0;
                }
            } else if (WIFSIGNALED(status)) {
                int sig = WTERMSIG(status);
                printf("%s[CRASH:%d]%s ", RED, sig, NC);
                failed_tests++;
                return 0;
            }
        } else if (ret == -1) {
            printf("%s[ERROR]%s ", RED, NC);
            failed_tests++;
            return 0;
        }
        usleep(100000);
        elapsed++;
    }
    
    kill(pid, SIGKILL);
    waitpid(pid, NULL, 0);
    printf("%s[TIMEOUT]%s ", YELLOW, NC);
    failed_tests++;
    return 0;
}

// Fonction pour tester avec capture de sortie
int run_test_output(const char *test_name, test_func func, const char *expected_output) {
    total_tests++;
    printf("%s: ", test_name);
    fflush(stdout);
    
    int pipefd[2];
    if (pipe(pipefd) == -1) {
        printf("%s[ERROR: pipe failed]%s\n", RED, NC);
        failed_tests++;
        return 0;
    }
    
    pid_t pid = fork();
    if (pid == -1) {
        printf("%s[ERROR: fork failed]%s\n", RED, NC);
        close(pipefd[0]);
        close(pipefd[1]);
        failed_tests++;
        return 0;
    }
    
    if (pid == 0) {
        // Processus enfant: redirige stdout vers le pipe
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);
        
        int result = func();
        exit(result);
    }
    
    // Processus parent
    close(pipefd[1]);
    
    char buffer[1024] = {0};
    int total_read = 0;
    int timeout = 2;
    int elapsed = 0;
    int status;
    
    while (elapsed < timeout) {
        int ret = waitpid(pid, &status, WNOHANG);
        if (ret == pid) {
            // Processus terminé, lire la sortie
            ssize_t n;
            while ((n = read(pipefd[0], buffer + total_read, sizeof(buffer) - total_read - 1)) > 0) {
                total_read += n;
            }
            close(pipefd[0]);
            
            if (WIFEXITED(status)) {
                int exit_code = WEXITSTATUS(status);
                if (exit_code != 0) {
                    printf("%s[KO]%s\n", RED, NC);
                    failed_tests++;
                    return 0;
                }
                
                // Comparer la sortie
                if (strcmp(buffer, expected_output) == 0) {
                    printf("%s[OK]%s\n", GREEN, NC);
                    return 1;
                } else {
                    printf("%s[KO]%s\n", RED, NC);
                    printf("  Expected: \"%s\"\n", expected_output);
                    printf("  Got:      \"%s\"\n", buffer);
                    failed_tests++;
                    return 0;
                }
            } else if (WIFSIGNALED(status)) {
                int sig = WTERMSIG(status);
                printf("%s[CRASH: Signal %d (%s)]%s\n", RED, sig, 
                       sig == SIGSEGV ? "SEGFAULT" : 
                       sig == SIGBUS ? "BUS ERROR" : 
                       sig == SIGABRT ? "ABORT" : "UNKNOWN", NC);
                close(pipefd[0]);
                failed_tests++;
                return 0;
            }
        } else if (ret == -1) {
            printf("%s[ERROR: waitpid failed]%s\n", RED, NC);
            close(pipefd[0]);
            failed_tests++;
            return 0;
        }
        usleep(100000);
        elapsed++;
    }
    
    // Timeout
    kill(pid, SIGKILL);
    waitpid(pid, NULL, 0);
    close(pipefd[0]);
    printf("%s[TIMEOUT]%s\n", YELLOW, NC);
    failed_tests++;
    return 0;
}

// Fonction pour tester avec capture de sortie sans newline (pour grouper avec tests NULL)
int run_test_output_inline(const char *test_name, test_func func, const char *expected_output) {
    total_tests++;
    printf("%s: ", test_name);
    fflush(stdout);
    
    int pipefd[2];
    if (pipe(pipefd) == -1) {
        printf("%s[ERROR]%s ", RED, NC);
        failed_tests++;
        return 0;
    }
    
    pid_t pid = fork();
    if (pid == -1) {
        printf("%s[ERROR]%s ", RED, NC);
        close(pipefd[0]);
        close(pipefd[1]);
        failed_tests++;
        return 0;
    }
    
    if (pid == 0) {
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);
        
        int result = func();
        exit(result);
    }
    
    close(pipefd[1]);
    
    char buffer[1024] = {0};
    int total_read = 0;
    int timeout = 2;
    int elapsed = 0;
    int status;
    
    while (elapsed < timeout) {
        int ret = waitpid(pid, &status, WNOHANG);
        if (ret == pid) {
            ssize_t n;
            while ((n = read(pipefd[0], buffer + total_read, sizeof(buffer) - total_read - 1)) > 0) {
                total_read += n;
            }
            close(pipefd[0]);
            
            if (WIFEXITED(status)) {
                int exit_code = WEXITSTATUS(status);
                if (exit_code != 0) {
                    printf("%s[KO]%s ", RED, NC);
                    failed_tests++;
                    return 0;
                }
                
                if (strcmp(buffer, expected_output) == 0) {
                    printf("%s[OK]%s ", GREEN, NC);
                    return 1;
                } else {
                    printf("%s[KO]%s ", RED, NC);
                    failed_tests++;
                    return 0;
                }
            } else if (WIFSIGNALED(status)) {
                int sig = WTERMSIG(status);
                printf("%s[CRASH:%d]%s ", RED, sig, NC);
                close(pipefd[0]);
                failed_tests++;
                return 0;
            }
        } else if (ret == -1) {
            printf("%s[ERROR]%s ", RED, NC);
            close(pipefd[0]);
            failed_tests++;
            return 0;
        }
        usleep(100000);
        elapsed++;
    }
    
    kill(pid, SIGKILL);
    waitpid(pid, NULL, 0);
    close(pipefd[0]);
    printf("%s[TIMEOUT]%s ", YELLOW, NC);
    failed_tests++;
    return 0;
}

// Structure pour stocker les tests multiples
typedef struct {
    test_func func;
    const char *expected_output;
    const char *description;
} test_case_t;

// Structure pour stocker les résultats des tests
typedef struct {
    int passed;
    char buffer[1024];
} test_result_t;

// Fonction pour tester plusieurs tests fork sur une même ligne (sans capture de sortie)
void run_multiple_fork_tests(const char *func_name, test_func *tests, int count) {
    printf("%s: ", func_name);
    fflush(stdout);
    
    for (int i = 0; i < count; i++) {
        total_tests++;
        
        fflush(stdout);  // Flush avant CHAQUE fork
        pid_t pid = fork();
        if (pid == -1) {
            printf("%s[ERROR]%s ", RED, NC);
            failed_tests++;
            continue;
        }
        
        if (pid == 0) {
            int result = tests[i]();
            exit(result);
        }
        
        int status;
        int timeout = 2;
        int elapsed = 0;
        int result_printed = 0;
        
        while (elapsed < timeout) {
            int ret = waitpid(pid, &status, WNOHANG);
            if (ret == pid) {
                if (!result_printed) {
                    result_printed = 1;
                    if (WIFEXITED(status)) {
                        int exit_code = WEXITSTATUS(status);
                        if (exit_code == 0) {
                            printf("%s[OK]%s ", GREEN, NC);
                        } else {
                            printf("%s[KO]%s ", RED, NC);
                            failed_tests++;
                        }
                    } else if (WIFSIGNALED(status)) {
                        int sig = WTERMSIG(status);
                        printf("%s[CRASH:%d]%s ", RED, sig, NC);
                        failed_tests++;
                    }
                }
                goto next_test;
            } else if (ret == -1) {
                printf("%s[ERROR]%s ", RED, NC);
                failed_tests++;
                goto next_test;
            }
            usleep(100000);
            elapsed++;
        }
        
        kill(pid, SIGKILL);
        waitpid(pid, NULL, 0);
        printf("%s[TIMEOUT]%s ", YELLOW, NC);
        failed_tests++;
        
next_test:
        continue;
    }
    
    printf("\n");
}

// Fonction pour tester plusieurs tests fork NULL protection sur une même ligne
void run_multiple_null_tests_inline(test_func *tests, int count) {
    for (int i = 0; i < count; i++) {
        total_tests++;
        null_tests++;
        
        fflush(stdout);  // Flush avant CHAQUE fork
        pid_t pid = fork();
        if (pid == -1) {
            printf("%s[ERROR]%s ", RED, NC);
            continue;
        }
        
        if (pid == 0) {
            int result = tests[i]();
            exit(result);
        }
        
        int status;
        int timeout = 2;
        int elapsed = 0;
        int result_printed = 0;
        
        while (elapsed < timeout) {
            int ret = waitpid(pid, &status, WNOHANG);
            if (ret == pid) {
                if (!result_printed) {
                    result_printed = 1;
                    if (WIFEXITED(status)) {
                        int exit_code = WEXITSTATUS(status);
                        if (exit_code == 0) {
                            printf("%s[%s🛡%s]%s ", GREEN, GRAY, GREEN, NC);
                            null_protected++;
                        } else {
                            printf("%s[⚠]%s ", YELLOW, NC);
                        }
                    } else if (WIFSIGNALED(status)) {
                        printf("%s[⚠]%s ", YELLOW, NC);
                    }
                }
                goto next_null_test;
            } else if (ret == -1) {
                printf("%s[ERROR]%s ", RED, NC);
                goto next_null_test;
            }
            usleep(100000);
            elapsed++;
        }
        
        kill(pid, SIGKILL);
        waitpid(pid, NULL, 0);
        printf("%s[TIMEOUT]%s ", YELLOW, NC);
        
next_null_test:
        continue;
    }
}

// Fonction pour tester plusieurs cas d'une même fonction sur une ligne
void run_multiple_output_tests_internal(const char *func_name, test_case_t *tests, int count, int is_null_test) {
    printf("%s: ", func_name);
    fflush(stdout);
    
    test_result_t *results = calloc(count, sizeof(test_result_t));
    int has_error = 0;
    
    for (int i = 0; i < count; i++) {
        total_tests++;
        
        int pipefd[2];
        if (pipe(pipefd) == -1) {
            printf("%s[ERROR]%s ", RED, NC);
            failed_tests++;
            has_error = 1;
            results[i].passed = 0;
            continue;
        }
        
        fflush(stdout); // IMPORTANT: vider stdout du parent avant fork
        pid_t pid = fork();
        if (pid == -1) {
            close(pipefd[0]);
            close(pipefd[1]);
            printf("%s[ERROR]%s ", RED, NC);
            failed_tests++;
            has_error = 1;
            results[i].passed = 0;
            continue;
        }
        
        if (pid == 0) {
            close(pipefd[0]);
            // Fermer complètement stdout puis rediriger vers le pipe
            close(STDOUT_FILENO);
            dup2(pipefd[1], STDOUT_FILENO);
            close(pipefd[1]);
            int result = tests[i].func();
            exit(result);
        }
        
        close(pipefd[1]);
        
        int total_read = 0;
        int timeout = 2;
        int elapsed = 0;
        int status;
        
        while (elapsed < timeout) {
            int ret = waitpid(pid, &status, WNOHANG);
            if (ret == pid) {
                ssize_t n;
                while ((n = read(pipefd[0], results[i].buffer + total_read, 
                        sizeof(results[i].buffer) - total_read - 1)) > 0) {
                    total_read += n;
                }
                close(pipefd[0]);
                
                if (WIFEXITED(status)) {
                    int exit_code = WEXITSTATUS(status);
                    if (exit_code == 0 && strcmp(results[i].buffer, tests[i].expected_output) == 0) {
                        if (is_null_test) {
                            printf("%s[%s🛡%s]%s ", GREEN, GRAY, GREEN, NC);
                        } else {
                            printf("%s[OK]%s ", GREEN, NC);
                        }
                        results[i].passed = 1;
                    } else {
                        if (is_null_test) {
                            printf("%s[⚠]%s ", RED, NC);
                        } else {
                            printf("%s[KO]%s ", RED, NC);
                        }
                        failed_tests++;
                        has_error = 1;
                        results[i].passed = 0;
                    }
                } else if (WIFSIGNALED(status)) {
                    if (is_null_test) {
                        printf("%s[⚠]%s ", RED, NC);
                    } else {
                        printf("%s[KO]%s ", RED, NC);
                    }
                    failed_tests++;
                    has_error = 1;
                    results[i].passed = 0;
                }
                break;
            } else if (ret == -1) {
                printf("%s[ERROR]%s ", RED, NC);
                failed_tests++;
                has_error = 1;
                results[i].passed = 0;
                close(pipefd[0]);
                break;
            }
            usleep(100000);
            elapsed++;
        }
        
        if (elapsed >= timeout) {
            kill(pid, SIGKILL);
            waitpid(pid, NULL, 0);
            close(pipefd[0]);
            printf("%s[TIMEOUT]%s ", YELLOW, NC);
            failed_tests++;
            has_error = 1;
            results[i].passed = 0;
        }
    }
    
    printf("\n");
    
    // Si des erreurs, afficher les détails
    if (has_error) {
        for (int i = 0; i < count; i++) {
            if (!results[i].passed) {
                printf("  Test '%s' failed:\n", tests[i].description);
                printf("    Expected: \"%s\"\n", tests[i].expected_output);
                printf("    Got:      \"%s\"\n", results[i].buffer);
            }
        }
    }
    
    free(results);
}

void run_multiple_output_tests(const char *func_name, test_case_t *tests, int count) {
    run_multiple_output_tests_internal(func_name, tests, count, 0);
}

// Fonction pour tester plusieurs tests output NULL protection (inline, sans newline)
void run_multiple_output_null_tests_inline(test_case_t *tests, int count) {
    test_result_t *results = calloc(count, sizeof(test_result_t));
    int has_error = 0;
    
    for (int i = 0; i < count; i++) {
        total_tests++;
        null_tests++;
        
        int pipefd[2];
        if (pipe(pipefd) == -1) {
            printf("%s[ERROR]%s ", RED, NC);
            has_error = 1;
            results[i].passed = 0;
            continue;
        }
        
        fflush(stdout);
        pid_t pid = fork();
        if (pid == -1) {
            close(pipefd[0]);
            close(pipefd[1]);
            printf("%s[ERROR]%s ", RED, NC);
            has_error = 1;
            results[i].passed = 0;
            continue;
        }
        
        if (pid == 0) {
            close(pipefd[0]);
            close(STDOUT_FILENO);
            dup2(pipefd[1], STDOUT_FILENO);
            close(pipefd[1]);
            int result = tests[i].func();
            exit(result);
        }
        
        close(pipefd[1]);
        
        int total_read = 0;
        int timeout = 2;
        int elapsed = 0;
        int status;
        
        while (elapsed < timeout) {
            int ret = waitpid(pid, &status, WNOHANG);
            if (ret == pid) {
                ssize_t n;
                while ((n = read(pipefd[0], results[i].buffer + total_read, 
                        sizeof(results[i].buffer) - total_read - 1)) > 0) {
                    total_read += n;
                }
                close(pipefd[0]);
                
                if (WIFEXITED(status)) {
                    int exit_code = WEXITSTATUS(status);
                    if (exit_code == 0 && strcmp(results[i].buffer, tests[i].expected_output) == 0) {
                        printf("%s[%s🛡%s]%s ", GREEN, GRAY, GREEN, NC);
                        results[i].passed = 1;
                        null_protected++;
                    } else {
                        printf("%s[⚠]%s ", YELLOW, NC);
                        has_error = 1;
                        results[i].passed = 0;
                    }
                } else if (WIFSIGNALED(status)) {
                    printf("%s[⚠]%s ", YELLOW, NC);
                    has_error = 1;
                    results[i].passed = 0;
                }
                break;
            } else if (ret == -1) {
                printf("%s[ERROR]%s ", RED, NC);
                has_error = 1;
                results[i].passed = 0;
                close(pipefd[0]);
                break;
            }
            usleep(100000);
            elapsed++;
        }
        
        if (elapsed >= timeout) {
            kill(pid, SIGKILL);
            waitpid(pid, NULL, 0);
            close(pipefd[0]);
            printf("%s[TIMEOUT]%s ", YELLOW, NC);
            has_error = 1;
            results[i].passed = 0;
        }
    }
    
    // Afficher les détails des erreurs si nécessaire
    if (has_error) {
        printf("\n");
        for (int i = 0; i < count; i++) {
            if (!results[i].passed) {
                printf("  Test '%s' failed:\n", tests[i].description);
                printf("    Expected: \"%s\"\n", tests[i].expected_output);
                printf("    Got:      \"%s\"\n", results[i].buffer);
            }
        }
    }
    
    free(results);
}

int test_isalpha() {
    for (int c = 0; c <= 255; c++) {
        int ft = ft_isalpha(c) ? 1 : 0;
        int std = isalpha(c) ? 1 : 0;
        if (ft != std) {
            printf("  Error: c=%d ft_isalpha=%d isalpha=%d\n", c, ft, std);
            return 1;
        }
    }
    return 0;
}

int test_isdigit() {
    for (int c = 0; c <= 255; c++) {
        int ft = ft_isdigit(c) ? 1 : 0;
        int std = isdigit(c) ? 1 : 0;
        if (ft != std) {
            printf("  Error: c=%d ft_isdigit=%d isdigit=%d\n", c, ft, std);
            return 1;
        }
    }
    return 0;
}

int test_isalnum() {
    for (int c = 0; c <= 255; c++) {
        int ft = ft_isalnum(c) ? 1 : 0;
        int std = isalnum(c) ? 1 : 0;
        if (ft != std) {
            printf("  Error: c=%d ft_isalnum=%d isalnum=%d\n", c, ft, std);
            return 1;
        }
    }
    return 0;
}

int test_isascii() {
    for (int c = 0; c <= 255; c++) {
        int ft = ft_isascii(c) ? 1 : 0;
        int std = isascii(c) ? 1 : 0;
        if (ft != std) {
            printf("  Error: c=%d ft_isascii=%d isascii=%d\n", c, ft, std);
            return 1;
        }
    }
    return 0;
}

int test_isprint() {
    for (int c = 0; c <= 255; c++) {
        int ft = ft_isprint(c) ? 1 : 0;
        int std = isprint(c) ? 1 : 0;
        if (ft != std) {
            printf("  Error: c=%d ft_isprint=%d isprint=%d\n", c, ft, std);
            return 1;
        }
    }
    return 0;
}

int test_strlen() {
    const char *tests[] = {"", "a", "abc", "Hello, world!", NULL};
    for (int i = 0; tests[i]; i++) {
        size_t ft = ft_strlen(tests[i]);
        size_t std = strlen(tests[i]);
        if (ft != std) {
            printf("  Error: s=\"%s\" ft_strlen=%zu strlen=%zu\n", tests[i], ft, std);
            return 1;
        }
    }
    return 0;
}

int test_strlcpy() {
    const char *srcs[] = {"Hello", "", "abc", NULL};
    for (int i = 0; srcs[i]; i++) {
        char d1[20] = {0};
        size_t r1 = ft_strlcpy(d1, srcs[i], 20);
        size_t r2 = strlen(srcs[i]);
        if (r1 != r2 || strcmp(d1, srcs[i]) != 0) {
            printf("  Error: src=\"%s\" ft_strlcpy=%zu expected=%zu d1=\"%s\"\n", srcs[i], r1, r2, d1);
            return 1;
        }
    }
    return 0;
}

int test_strlcat() {
    char d1[20] = "Hi ";
    const char *src = "there";
    size_t r1 = ft_strlcat(d1, src, 20);
    if (strcmp(d1, "Hi there") != 0 || r1 != strlen("Hi ") + strlen(src)) {
        printf("  Error: dest=\"%s\" r1=%zu expected=%zu\n", d1, r1, strlen("Hi ") + strlen(src));
        return 1;
    }
    return 0;
}

int test_strchr() {
    const char *s = "Hello";
    for (int c = 0; c < 128; c++) {
        char *ft = ft_strchr(s, c);
        char *std = strchr(s, c);
        if ((ft == NULL) != (std == NULL) || (ft && std && ft - s != std - s)) {
            printf("  Error: c=%d(%c) ft_strchr=%p strchr=%p\n", c, c, (void*)ft, (void*)std);
            return 1;
        }
    }
    return 0;
}

int test_strrchr() {
    const char *s = "Hello";
    for (int c = 0; c < 128; c++) {
        char *ft = ft_strrchr(s, c);
        char *std = strrchr(s, c);
        if ((ft == NULL) != (std == NULL) || (ft && std && ft - s != std - s)) {
            printf("  Error: c=%d(%c) ft_strrchr=%p strrchr=%p\n", c, c, (void*)ft, (void*)std);
            return 1;
        }
    }
    return 0;
}

int test_strncmp() {
    const char *a[] = {"abc", "abc", "abc", "abc", "", "abc", NULL};
    const char *b[] = {"abc", "abd", "", "abcde", "", "", NULL};
    size_t n[] = {3, 2, 1, 5, 0, 1};
    for (int i = 0; a[i]; i++) {
        int ft = ft_strncmp(a[i], b[i], n[i]);
        int std = strncmp(a[i], b[i], n[i]);
        if ((ft == 0) != (std == 0)) {
            printf("  Error: \"%s\" vs \"%s\" n=%zu ft_strncmp=%d strncmp=%d\n", a[i], b[i], n[i], ft, std);
            return 1;
        }
    }
    return 0;
}

int test_strnstr() {
    const char *big = "Hello world";
    const char *little = "world";
    char *ft = ft_strnstr(big, little, 11);
    char *std = strstr(big, little);
    if ((ft == NULL && std != NULL) || (ft && std && ft - big != std - big)) {
        printf("  Error: ft_strnstr=%p strstr=%p\n", (void*)ft, (void*)std);
        return 1;
    }
    return 0;
}

int test_memset() {
    char a[10], b[10];
    memset(b, 'A', 5);
    ft_memset(a, 'A', 5);
    for (int i = 0; i < 5; i++) {
        if (a[i] != b[i]) {
            printf("  Error: i=%d a=%d b=%d\n", i, a[i], b[i]);
            return 1;
        }
    }
    return 0;
}

int test_bzero() {
    char a[10];
    ft_bzero(a, 10);
    for (int i = 0; i < 10; i++) {
        if (a[i] != 0) {
            printf("  Error: i=%d a=%d (expected 0)\n", i, a[i]);
            return 1;
        }
    }
    return 0;
}

int test_memcpy() {
    char a[10], b[10];
    ft_memcpy(a, "12345", 6);
    memcpy(b, "12345", 6);
    if (memcmp(a, b, 6) != 0) {
        printf("  Error: a=\"%s\" b=\"%s\"\n", a, b);
        return 1;
    }
    return 0;
}

int test_memmove() {
    char a[10] = "abcdefg";
    char b[10] = "abcdefg";
    ft_memmove(a+2, a, 5);
    memmove(b+2, b, 5);
    if (memcmp(a, b, 10) != 0) {
        printf("  Error: a=\"%s\" b=\"%s\"\n", a, b);
        return 1;
    }
    return 0;
}

int test_memchr() {
    char s[] = "abcde";
    for (int c = 0; c < 128; c++) {
        void *ft = ft_memchr(s, c, 5);
        void *std = memchr(s, c, 5);
        if (ft != std) {
            printf("  Error: c=%d(%c) ft_memchr=%p memchr=%p\n", c, c, ft, std);
            return 1;
        }
    }
    return 0;
}

int test_memcmp() {
    const char *a[] = {"abc", "abc", "abc", "", NULL};
    const char *b[] = {"abc", "abd", "", "", NULL};
    size_t n[] = {3, 3, 1, 0};
    for (int i = 0; a[i]; i++) {
        int ft = ft_memcmp(a[i], b[i], n[i]);
        int std = memcmp(a[i], b[i], n[i]);
        if ((ft == 0) != (std == 0)) {
            printf("  Error: \"%s\" vs \"%s\" n=%zu ft_memcmp=%d memcmp=%d\n", a[i], b[i], n[i], ft, std);
            return 1;
        }
    }
    return 0;
}

int test_atoi() {
    const char *tests[] = {"42", "-42", "0", "  123", "abc", "2147483647", "-2147483648", NULL};
    for (int i = 0; tests[i]; i++) {
        int ft = ft_atoi(tests[i]);
        int std = atoi(tests[i]);
        if (ft != std) {
            printf("  Error: s=\"%s\" ft_atoi=%d atoi=%d\n", tests[i], ft, std);
            return 1;
        }
    }
    return 0;
}

int test_toupper() {
    for (int c = 0; c <= 255; c++) {
        int ft = ft_toupper(c);
        int std = toupper(c);
        if (ft != std) {
            printf("  Error: c=%d(%c) ft_toupper=%d toupper=%d\n", c, c, ft, std);
            return 1;
        }
    }
    return 0;
}

int test_tolower() {
    for (int c = 0; c <= 255; c++) {
        int ft = ft_tolower(c);
        int std = tolower(c);
        if (ft != std) {
            printf("  Error: c=%d(%c) ft_tolower=%d tolower=%d\n", c, c, ft, std);
            return 1;
        }
    }
    return 0;
}

int test_strdup() {
    const char *tests[] = {"hello", "", "abc", NULL};
    for (int i = 0; tests[i]; i++) {
        char *a = ft_strdup(tests[i]);
        char *b = strdup(tests[i]);
        if (strcmp(a, b) != 0) {
            printf("  Error: s=\"%s\" ft_strdup=\"%s\" strdup=\"%s\"\n", tests[i], a, b);
            free(a); free(b);
            return 1;
        }
        free(a); free(b);
    }
    return 0;
}

int test_calloc() {
    for (size_t n = 1; n <= 10; n++) {
        char *a = ft_calloc(n, 1);
        char *b = calloc(n, 1);
        if (memcmp(a, b, n) != 0) {
            printf("  Error: n=%zu - memory not zeroed\n", n);
            free(a); free(b);
            return 1;
        }
        free(a); free(b);
    }
    return 0;
}

// Part 2

int test_itoa() {
    int tests[] = {42, -42, 0, 2147483647, -2147483648};
    char buf[32];
    for (int i = 0; i < 5; i++) {
        char *ft = ft_itoa(tests[i]);
        snprintf(buf, sizeof(buf), "%d", tests[i]);
        if (strcmp(ft, buf) != 0) {
            printf("  Error: n=%d ft_itoa=\"%s\" expected=\"%s\"\n", tests[i], ft, buf);
            free(ft);
            return 1;
        }
        free(ft);
    }
    return 0;
}

int test_putchar_fd() {
    ft_putchar_fd('A', STDOUT_FILENO);
    return 0;
}

int test_putendl_fd() {
    ft_putendl_fd("Hello", STDOUT_FILENO);
    return 0;
}

int test_putnbr_fd() {
    ft_putnbr_fd(12345, STDOUT_FILENO);
    return 0;
}

int test_putstr_fd() {
    ft_putstr_fd("World", STDOUT_FILENO);
    return 0;
}

// Tests additionnels pour les fonctions put avec valeurs négatives et edge cases
int test_putnbr_negative() {
    ft_putnbr_fd(-42, STDOUT_FILENO);
    return 0;
}

int test_putnbr_zero() {
    ft_putnbr_fd(0, STDOUT_FILENO);
    return 0;
}

int test_putnbr_min() {
    ft_putnbr_fd(-2147483648, STDOUT_FILENO);
    return 0;
}

int test_putnbr_max() {
    ft_putnbr_fd(2147483647, STDOUT_FILENO);
    return 0;
}

int test_putstr_empty() {
    ft_putstr_fd("", STDOUT_FILENO);
    return 0;
}

int test_split() {
    char **tab = ft_split("a,b,c", ',');
    const char *ref[] = {"a", "b", "c", NULL};
    for (int i = 0; ref[i]; i++) {
        if (!tab[i] || strcmp(tab[i], ref[i]) != 0) {
            printf("  Error: i=%d ft_split=\"%s\" expected=\"%s\"\n", i, tab[i] ? tab[i] : "NULL", ref[i]);
            for (int j = 0; tab && tab[j]; j++) free(tab[j]);
            free(tab);
            return 1;
        }
    }
    if (tab[3] != NULL) {
        printf("  Error: tab[3] should be NULL\n");
        for (int i = 0; tab && tab[i]; i++) free(tab[i]);
        free(tab);
        return 1;
    }
    for (int i = 0; tab && tab[i]; i++) free(tab[i]);
    free(tab);
    return 0;
}

int test_strjoin() {
    char *s = ft_strjoin("Hello", "World");
    if (strcmp(s, "HelloWorld") != 0) {
        printf("  Error: ft_strjoin=\"%s\" expected=\"HelloWorld\"\n", s);
        free(s);
        return 1;
    }
    free(s);
    return 0;
}

int test_strtrim() {
    char *s = ft_strtrim("  Hello  ", " ");
    if (strcmp(s, "Hello") != 0) {
        printf("  Error: ft_strtrim=\"%s\" expected=\"Hello\"\n", s);
        free(s);
        return 1;
    }
    free(s);
    return 0;
}

int test_substr() {
    char *s = ft_substr("HelloWorld", 5, 5);
    if (strcmp(s, "World") != 0) {
        printf("  Error: ft_substr=\"%s\" expected=\"World\"\n", s);
        free(s);
        return 1;
    }
    free(s);
    return 0;
}

void f_iteri(unsigned int i, char *c) {
    if (c)
        *c = *c + i;
}

int test_striteri() {
    char s[] = "abcd";
    ft_striteri(s, f_iteri);
    if (strcmp(s, "aceg") != 0) {
        printf("  Error: ft_striteri=\"%s\" expected=\"aceg\"\n", s);
        return 1;
    }
    return 0;
}

char f_mapi(unsigned int i, char c) {
    return c + i;
}

int test_strmapi() {
    char *s = ft_strmapi("abcd", f_mapi);
    if (strcmp(s, "aceg") != 0) {
        printf("  Error: ft_strmapi=\"%s\" expected=\"aceg\"\n", s);
        free(s);
        return 1;
    }
    free(s);
    return 0;
}

// ========================================
// Tests de protection NULL (Part 2)
// ========================================

int test_substr_null() {
    char *s = ft_substr(NULL, 0, 5);
    if (s != NULL) {
        printf("  Error: ft_substr(NULL) should return NULL\n");
        free(s);
        return 1;
    }
    return 0;
}

int test_strjoin_null_s1() {
    char *s = ft_strjoin(NULL, "World");
    if (s != NULL) {
        printf("  Error: ft_strjoin(NULL, s2) should return NULL\n");
        free(s);
        return 1;
    }
    return 0;
}

int test_strjoin_null_s2() {
    char *s = ft_strjoin("Hello", NULL);
    if (s != NULL) {
        printf("  Error: ft_strjoin(s1, NULL) should return NULL\n");
        free(s);
        return 1;
    }
    return 0;
}

int test_strjoin_null_both() {
    char *s = ft_strjoin(NULL, NULL);
    if (s != NULL) {
        printf("  Error: ft_strjoin(NULL, NULL) should return NULL\n");
        free(s);
        return 1;
    }
    return 0;
}

int test_strtrim_null_s1() {
    char *s = ft_strtrim(NULL, " ");
    if (s != NULL) {
        printf("  Error: ft_strtrim(NULL, set) should return NULL\n");
        free(s);
        return 1;
    }
    return 0;
}

int test_strtrim_null_set() {
    char *s = ft_strtrim("Hello", NULL);
    if (s != NULL && strcmp(s, "") != 0 && strcmp(s, "Hello") != 0) {
        printf("  Error: ft_strtrim(s1, NULL) should return NULL, empty string, or copy of s1\n");
        free(s);
        return 1;
    }
    if (s) free(s);
    return 0;
}

int test_split_null() {
    char **tab = ft_split(NULL, ',');
    if (tab != NULL && tab[0] != NULL) {
        printf("  Error: ft_split(NULL) should return NULL or {NULL}\n");
        for (int i = 0; tab && tab[i]; i++) free(tab[i]);
        free(tab);
        return 1;
    }
    if (tab) free(tab);
    return 0;
}

int test_strmapi_null_s() {
    char *s = ft_strmapi(NULL, f_mapi);
    if (s != NULL) {
        printf("  Error: ft_strmapi(NULL, f) should return NULL\n");
        free(s);
        return 1;
    }
    return 0;
}

int test_strmapi_null_f() {
    char *s = ft_strmapi("abcd", NULL);
    if (s != NULL) {
        printf("  Error: ft_strmapi(s, NULL) should return NULL\n");
        free(s);
        return 1;
    }
    return 0;
}

int test_striteri_null_s() {
    ft_striteri(NULL, f_iteri);
    // Si pas de crash, c'est OK
    return 0;
}

int test_striteri_null_f() {
    char s[] = "abcd";
    ft_striteri(s, NULL);
    // Si pas de crash, c'est OK
    return 0;
}

int test_putstr_null() {
    ft_putstr_fd(NULL, STDOUT_FILENO);
    // Si pas de crash, c'est OK
    return 0;
}

int test_putendl_null() {
    ft_putendl_fd(NULL, STDOUT_FILENO);
    // Si pas de crash, c'est OK
    return 0;
}

int main() {
    printf("=== LIBFT TESTER ===\n\n");
    printf("--- Part 1: Libc functions ---\n");
    
    run_test_fork("ft_isalpha", test_isalpha);
    run_test_fork("ft_isdigit", test_isdigit);
    run_test_fork("ft_isalnum", test_isalnum);
    run_test_fork("ft_isascii", test_isascii);
    run_test_fork("ft_isprint", test_isprint);
    run_test_fork("ft_strlen", test_strlen);
    run_test_fork("ft_strlcpy", test_strlcpy);
    run_test_fork("ft_strlcat", test_strlcat);
    run_test_fork("ft_strchr", test_strchr);
    run_test_fork("ft_strrchr", test_strrchr);
    run_test_fork("ft_strncmp", test_strncmp);
    run_test_fork("ft_strnstr", test_strnstr);
    run_test_fork("ft_memset", test_memset);
    run_test_fork("ft_bzero", test_bzero);
    run_test_fork("ft_memcpy", test_memcpy);
    run_test_fork("ft_memmove", test_memmove);
    run_test_fork("ft_memchr", test_memchr);
    run_test_fork("ft_memcmp", test_memcmp);
    run_test_fork("ft_atoi", test_atoi);
    run_test_fork("ft_toupper", test_toupper);
    run_test_fork("ft_tolower", test_tolower);
    run_test_fork("ft_strdup", test_strdup);
    run_test_fork("ft_calloc", test_calloc);

    printf("\n--- Part 2: Additional functions ---\n");
    
    run_test_fork("ft_itoa", test_itoa);
    run_test_output("ft_putchar_fd", test_putchar_fd, "A");
    
    // Tests ft_putendl_fd
    run_test_output_inline("ft_putendl_fd", test_putendl_fd, "Hello\n");
    test_case_t putendl_null_tests[] = {{test_putendl_null, "", "NULL"}};
    run_multiple_output_null_tests_inline(putendl_null_tests, 1);
    printf("\n");
    
    // Tests multiples pour ft_putnbr_fd sur une seule ligne
    test_case_t putnbr_tests[] = {
        {test_putnbr_fd, "12345", "positive"},
        {test_putnbr_negative, "-42", "negative"},
        {test_putnbr_zero, "0", "zero"},
        {test_putnbr_min, "-2147483648", "INT_MIN"},
        {test_putnbr_max, "2147483647", "INT_MAX"}
    };
    run_multiple_output_tests("ft_putnbr_fd", putnbr_tests, 5);
    
    // Tests ft_putstr_fd
    run_test_output_inline("ft_putstr_fd", test_putstr_fd, "World");
    test_case_t putstr_other_tests[] = {{test_putstr_empty, "", "empty"}};
    run_multiple_output_null_tests_inline(putstr_other_tests, 1);
    test_case_t putstr_null_tests[] = {{test_putstr_null, "", "NULL"}};
    run_multiple_output_null_tests_inline(putstr_null_tests, 1);
    printf("\n");
    
    // Tests ft_split
    run_test_fork_inline("ft_split", test_split);
    test_func split_null_tests[] = {test_split_null};
    run_multiple_null_tests_inline(split_null_tests, 1);
    printf("\n");
    
    // Tests ft_strjoin
    run_test_fork_inline("ft_strjoin", test_strjoin);
    test_func strjoin_null_tests[] = {test_strjoin_null_s1, test_strjoin_null_s2, test_strjoin_null_both};
    run_multiple_null_tests_inline(strjoin_null_tests, 3);
    printf("\n");
    
    // Tests ft_strtrim
    run_test_fork_inline("ft_strtrim", test_strtrim);
    test_func strtrim_null_tests[] = {test_strtrim_null_s1, test_strtrim_null_set};
    run_multiple_null_tests_inline(strtrim_null_tests, 2);
    printf("\n");
    
    // Tests ft_substr
    run_test_fork_inline("ft_substr", test_substr);
    test_func substr_null_tests[] = {test_substr_null};
    run_multiple_null_tests_inline(substr_null_tests, 1);
    printf("\n");
    
    // Tests ft_striteri
    run_test_fork_inline("ft_striteri", test_striteri);
    test_func striteri_null_tests[] = {test_striteri_null_s, test_striteri_null_f};
    run_multiple_null_tests_inline(striteri_null_tests, 2);
    printf("\n");
    
    // Tests ft_strmapi
    run_test_fork_inline("ft_strmapi", test_strmapi);
    test_func strmapi_null_tests[] = {test_strmapi_null_s, test_strmapi_null_f};
    run_multiple_null_tests_inline(strmapi_null_tests, 2);
    printf("\n");

    printf("\n===================\n");
    printf("Total: %d tests\n", total_tests);
    printf("%sPassed: %d%s\n", GREEN, total_tests - failed_tests, NC);
    
    if (failed_tests > 0) {
        printf("%sFailed: %d%s\n", RED, failed_tests, NC);
    }
    
    if (null_tests > 0) {
        printf("%sNULL Protection: %d/%d protected%s\n", GRAY, null_protected, null_tests, NC);
    }
    
    if (failed_tests > 0) {
        printf("\n%s❌ Certains tests ont échoué.%s\n", RED, NC);
        return 1;
    } else {
        printf("\n%s✅ Tous les tests sont passés avec succès !%s\n", GREEN, NC);
        return 0;
    }
}
