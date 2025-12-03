/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_libft_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: benpicar <benpicar@student.42mulhouse.fr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 03:47:39 by benpicar          #+#    #+#             */
/*   Updated: 2025/12/03 12:54:21 by benpicar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

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
#include <stdbool.h>
#include <sys/wait.h>
#include <signal.h>

// Variable globale pour suivre l'état des tests
int failed_tests = 0;
int total_tests = 0;
int null_tests = 0;
int null_protected = 0;

// Fonction utilitaire pour exécuter un test dans un fork
typedef int (*test_func)(void);

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
    int timeout = 20;
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
                printf("%s[CRASH: %s]%s ", RED,
                       sig == SIGSEGV ? "SEGFAULT" : 
                       sig == SIGBUS ? "BUS ERROR" : 
                       sig == SIGABRT ? "ABORT" : "UNKNOWN", NC);
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
    int timeout = 20;
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

void run_multiple_null_tests_inline(test_func tests[], int count) {
    for (int i = 0; i < count; i++) {
        null_tests++;
        fflush(stdout);
        
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
        int timeout = 20;
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
        
        if (!result_printed) {
            kill(pid, SIGKILL);
            waitpid(pid, NULL, 0);
            printf("%s[⚠]%s ", YELLOW, NC);
        }
        
        next_null_test:
        continue;
    }
}

// Bonus tests

void del_content(void *content) {
    free(content);
}

void print_content(void *content) {
    if (content)
        printf("%s ", (char *)content);
}

void *dup_content(void *content) {
    if (!content) return NULL;
    return strdup((char *)content);
}

int test_lstnew_and_add_front_back() {
    t_list *lst = NULL;
    t_list *n1 = ft_lstnew(strdup("one"));
    t_list *n2 = ft_lstnew(strdup("two"));
    t_list *n3 = ft_lstnew(strdup("three"));
    ft_lstadd_front(&lst, n2);
    ft_lstadd_back(&lst, n3);
    ft_lstadd_front(&lst, n1);
    const char *ref[] = {"one", "two", "three", NULL};
    t_list *tmp = lst;
    int i = 0;
    while (tmp && ref[i]) {
        if (strcmp((char*)tmp->content, ref[i]) != 0) {
            printf("  Error: i=%d got=\"%s\" expected=\"%s\"\n", i, (char*)tmp->content, ref[i]);
            ft_lstclear(&lst, del_content);
            return 1;
        }
        tmp = tmp->next; i++;
    }
    if (tmp != NULL || ref[i] != NULL) {
        printf("  Error: list size mismatch\n");
        ft_lstclear(&lst, del_content);
        return 1;
    }
    ft_lstclear(&lst, del_content);
    return 0;
}

int test_lstsize_and_last() {
    t_list *lst = NULL;
    ft_lstadd_back(&lst, ft_lstnew(strdup("a")));
    ft_lstadd_back(&lst, ft_lstnew(strdup("b")));
    ft_lstadd_back(&lst, ft_lstnew(strdup("c")));
    int size = ft_lstsize(lst);
    t_list *last = ft_lstlast(lst);
    if (size != 3) {
        printf("  Error: size=%d expected=3\n", size);
        ft_lstclear(&lst, del_content);
        return 1;
    }
    if (strcmp((char*)last->content, "c") != 0) {
        printf("  Error: last=\"%s\" expected=\"c\"\n", (char*)last->content);
        ft_lstclear(&lst, del_content);
        return 1;
    }
    ft_lstclear(&lst, del_content);
    return 0;
}

int test_lstdelone() {
    t_list *n = ft_lstnew(strdup("to_del"));
    ft_lstdelone(n, del_content);
    return 0;
}

int test_lstiter() {
    t_list *lst = NULL;
    ft_lstadd_back(&lst, ft_lstnew(strdup("x")));
    ft_lstadd_back(&lst, ft_lstnew(strdup("y")));
    ft_lstadd_back(&lst, ft_lstnew(strdup("z")));
    // On vérifie juste que ça ne crash pas et que les contenus sont accessibles
    ft_lstiter(lst, print_content);
    printf("\n");
    ft_lstclear(&lst, del_content);
    return 0;
}

void *to_upper(void *content) {
    char *s = strdup((char *)content);
    for (int i = 0; s && s[i]; i++)
        if (s[i] >= 'a' && s[i] <= 'z') s[i] -= 32;
    return s;
}

int test_lstmap() {
    t_list *lst = NULL;
    ft_lstadd_back(&lst, ft_lstnew(strdup("abc")));
    ft_lstadd_back(&lst, ft_lstnew(strdup("def")));
    t_list *mapped = ft_lstmap(lst, to_upper, del_content);
    const char *ref[] = {"ABC", "DEF", NULL};
    t_list *tmp = mapped;
    int i = 0;
    while (tmp && ref[i]) {
        if (strcmp((char*)tmp->content, ref[i]) != 0) {
            printf("  Error: i=%d got=\"%s\" expected=\"%s\"\n", i, (char*)tmp->content, ref[i]);
            ft_lstclear(&lst, del_content);
            ft_lstclear(&mapped, del_content);
            return 1;
        }
        tmp = tmp->next; i++;
    }
    if (tmp != NULL || ref[i] != NULL) {
        printf("  Error: list size mismatch\n");
        ft_lstclear(&lst, del_content);
        ft_lstclear(&mapped, del_content);
        return 1;
    }
    ft_lstclear(&lst, del_content);
    ft_lstclear(&mapped, del_content);
    return 0;
}

int test_lstclear() {
    t_list *lst = NULL;
    ft_lstadd_back(&lst, ft_lstnew(strdup("a")));
    ft_lstadd_back(&lst, ft_lstnew(strdup("b")));
    ft_lstadd_back(&lst, ft_lstnew(strdup("c")));
    ft_lstclear(&lst, del_content);
    if (lst != NULL) {
        printf("  Error: lst should be NULL after ft_lstclear\n");
        return 1;
    }
    return 0;
}

// ========================================
// Tests de protection NULL (Bonus)
// ========================================

int test_lstnew_null() {
    t_list *n = ft_lstnew(NULL);
    if (n == NULL) {
        printf("  Error: ft_lstnew(NULL) should create a node with NULL content\n");
        return 1;
    }
    if (n->content != NULL) {
        printf("  Error: node content should be NULL\n");
        free(n);
        return 1;
    }
    free(n);
    return 0;
}

int test_lstadd_front_null_lst() {
    t_list *n = ft_lstnew(strdup("test"));
    ft_lstadd_front(NULL, n);
    // Si pas de crash, c'est OK
    if (n) ft_lstdelone(n, del_content);
    return 0;
}

int test_lstadd_front_null_node() {
    t_list *lst = NULL;
    ft_lstadd_front(&lst, NULL);
    // Si pas de crash, c'est OK
    return 0;
}

int test_lstadd_back_null_lst() {
    t_list *n = ft_lstnew(strdup("test"));
    ft_lstadd_back(NULL, n);
    // Si pas de crash, c'est OK
    if (n) ft_lstdelone(n, del_content);
    return 0;
}

int test_lstadd_back_null_node() {
    t_list *lst = NULL;
    ft_lstadd_back(&lst, NULL);
    // Si pas de crash, c'est OK
    return 0;
}

int test_lstsize_null() {
    int size = ft_lstsize(NULL);
    if (size != 0) {
        printf("  Error: ft_lstsize(NULL) should return 0\n");
        return 1;
    }
    return 0;
}

int test_lstlast_null() {
    t_list *last = ft_lstlast(NULL);
    if (last != NULL) {
        printf("  Error: ft_lstlast(NULL) should return NULL\n");
        return 1;
    }
    return 0;
}

int test_lstdelone_null_node() {
    ft_lstdelone(NULL, del_content);
    // Si pas de crash, c'est OK
    return 0;
}

int test_lstdelone_null_del() {
    t_list *n = ft_lstnew(strdup("test"));
    ft_lstdelone(n, NULL);
    // Si pas de crash, c'est OK (mais attention aux leaks)
    return 0;
}

int test_lstclear_null_lst() {
    ft_lstclear(NULL, del_content);
    // Si pas de crash, c'est OK
    return 0;
}

int test_lstclear_null_del() {
    t_list *lst = NULL;
    ft_lstadd_back(&lst, ft_lstnew(strdup("test")));
    ft_lstclear(&lst, NULL);
    // Si pas de crash, c'est OK (mais attention aux leaks)
    return 0;
}

int test_lstiter_null_lst() {
    ft_lstiter(NULL, print_content);
    // Si pas de crash, c'est OK
    return 0;
}

int test_lstiter_null_f() {
    t_list *lst = NULL;
    ft_lstadd_back(&lst, ft_lstnew(strdup("test")));
    ft_lstiter(lst, NULL);
    // Si pas de crash, c'est OK
    ft_lstclear(&lst, del_content);
    return 0;
}

int test_lstmap_null_lst() {
    t_list *mapped = ft_lstmap(NULL, to_upper, del_content);
    if (mapped != NULL) {
        printf("  Error: ft_lstmap(NULL) should return NULL\n");
        ft_lstclear(&mapped, del_content);
        return 1;
    }
    return 0;
}

int test_lstmap_null_f() {
    t_list *lst = NULL;
    ft_lstadd_back(&lst, ft_lstnew(strdup("test")));
    t_list *mapped = ft_lstmap(lst, NULL, del_content);
    if (mapped != NULL) {
        printf("  Error: ft_lstmap(lst, NULL, del) should return NULL\n");
        ft_lstclear(&mapped, del_content);
    }
    ft_lstclear(&lst, del_content);
    return 0;
}

int test_lstmap_null_del() {
    t_list *lst = NULL;
    ft_lstadd_back(&lst, ft_lstnew(strdup("test")));
    t_list *mapped = ft_lstmap(lst, to_upper, NULL);
    // Si pas de crash, c'est OK (mais attention aux leaks)
    if (mapped) ft_lstclear(&mapped, del_content);
    ft_lstclear(&lst, del_content);
    return 0;
}

int main(void)
{
    printf("=== LIBFT BONUS TESTER ===\n\n");
    printf("--- Bonus: Linked list functions ---\n");
    
    run_test_fork_inline("ft_lstnew", test_lstnew_null);
    test_func lstnew_null_tests[] = {test_lstnew_null};
    run_multiple_null_tests_inline(lstnew_null_tests, 1);
    printf("\n");
    
    run_test_fork_inline("ft_lstadd_front/back", test_lstnew_and_add_front_back);
    test_func lstadd_null_tests[] = {test_lstadd_front_null_lst, test_lstadd_front_null_node, 
                                      test_lstadd_back_null_lst, test_lstadd_back_null_node};
    run_multiple_null_tests_inline(lstadd_null_tests, 4);
    printf("\n");
    
    run_test_fork_inline("ft_lstsize + ft_lstlast", test_lstsize_and_last);
    test_func lstsize_last_null_tests[] = {test_lstsize_null, test_lstlast_null};
    run_multiple_null_tests_inline(lstsize_last_null_tests, 2);
    printf("\n");
    
    run_test_fork_inline("ft_lstdelone", test_lstdelone);
    test_func lstdelone_null_tests[] = {test_lstdelone_null_node, test_lstdelone_null_del};
    run_multiple_null_tests_inline(lstdelone_null_tests, 2);
    printf("\n");
    
    run_test_fork_inline("ft_lstclear", test_lstclear);
    test_func lstclear_null_tests[] = {test_lstclear_null_lst, test_lstclear_null_del};
    run_multiple_null_tests_inline(lstclear_null_tests, 2);
    printf("\n");
    
    run_test_output_inline("ft_lstiter", test_lstiter, "x y z \n");
    test_func lstiter_null_tests[] = {test_lstiter_null_lst, test_lstiter_null_f};
    run_multiple_null_tests_inline(lstiter_null_tests, 2);
    printf("\n");
    
    run_test_fork_inline("ft_lstmap", test_lstmap);
    test_func lstmap_null_tests[] = {test_lstmap_null_lst, test_lstmap_null_f, test_lstmap_null_del};
    run_multiple_null_tests_inline(lstmap_null_tests, 3);
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
        printf("\n%s❌ Certains tests bonus ont échoué.%s\n", RED, NC);
        return 1;
    } else {
        printf("\n%s✅ Tous les tests bonus sont passés avec succès !%s\n", GREEN, NC);
        return 0;
    }
}
