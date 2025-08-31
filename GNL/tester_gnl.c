/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tester_gnl.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: benpicar <benpicar@student.42mulhouse.fr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 10:05:34 by benpicar          #+#    #+#             */
/*   Updated: 2025/08/29 10:30:01 by benpicar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>

#define RED   "\033[0;31m"
#define GREEN "\033[0;32m"
#define NC    "\033[0m"

// Affiche uniquement le statut du test ([OK] ou [FAIL]) avec couleur
void print_test_status(int success) {
    if (success)
        printf(GREEN "[OK]" NC "\n");
    else
        printf(RED "[FAIL]" NC "\n");
}

// Test basique : lecture d'un fichier ligne par ligne
int test_basic(const char *filename, const char **expected_lines, int expected_count) {
    int fd = open(filename, O_RDONLY);
    if (fd < 0) {
        perror("open");
        return 0;
    }
    int success = 1;
    for (int i = 0; i < expected_count; ++i) {
        char *line = get_next_line(fd);
        if (!line || strcmp(line, expected_lines[i]) != 0) {
            printf("Attendu: '%s', Obtenu: '%s'\n", expected_lines[i], line ? line : "(null)");
            success = 0;
        }
        free(line);
    }
    char *last = get_next_line(fd);
    if (last) {
        printf("Attendu: (null), Obtenu: '%s'\n", last);
        success = 0;
        free(last);
    }
    close(fd);
    return success;
}

int test_long_line(const char *filename, const char *expected_line) {
    int fd = open(filename, O_RDONLY);
    if (fd < 0) return 0;
    char *line = get_next_line(fd);
    int success = (line && strcmp(line, expected_line) == 0);
    if (!success) printf("Attendu: '%s', Obtenu: '%s'\n", expected_line, line ? line : "(null)");
    free(line);
    char *last = get_next_line(fd);
    if (last) { success = 0; free(last); }
    close(fd);
    return success;
}

int test_empty_lines(const char *filename, int nb_lines) {
    int fd = open(filename, O_RDONLY);
    if (fd < 0) return 0;
    int success = 1;
    for (int i = 0; i < nb_lines; ++i) {
        char *line = get_next_line(fd);
        if (!line || strcmp(line, "\n") != 0) {
            printf("Attendu: '\\n', Obtenu: '%s'\n", line ? line : "(null)");
            success = 0;
        }
        free(line);
    }
    char *last = get_next_line(fd);
    if (last) { success = 0; free(last); }
    close(fd);
    return success;
}

int test_no_newline_final(const char *filename, const char *expected_line) {
    int fd = open(filename, O_RDONLY);
    if (fd < 0) return 0;
    char *line = get_next_line(fd);
    int success = (line && strcmp(line, expected_line) == 0);
    if (!success) printf("Attendu: '%s', Obtenu: '%s'\n", expected_line, line ? line : "(null)");
    free(line);
    char *last = get_next_line(fd);
    if (last) { success = 0; free(last); }
    close(fd);
    return success;
}

int test_multi_fd(const char *file1, const char *file2, const char *exp1, const char *exp2) {
    int fd1 = open(file1, O_RDONLY);
    int fd2 = open(file2, O_RDONLY);
    if (fd1 < 0 || fd2 < 0) return 0;
    char *l1 = get_next_line(fd1);
    char *l2 = get_next_line(fd2);
    int success = (l1 && l2 && strcmp(l1, exp1) == 0 && strcmp(l2, exp2) == 0);
    if (!success) printf("Multi-fd: Obtenu: '%s' et '%s'\n", l1, l2);
    free(l1); free(l2);
    close(fd1); close(fd2);
    return success;
}

int test_invalid_fd() {
    char *line = get_next_line(-1);
    int success = (line == NULL);
    if (!success) { printf("fd -1 devrait retourner NULL\n"); free(line); }
    return success;
}

int test_stress(const char *filename, int nb) {
    int fd = open(filename, O_RDONLY);
    if (fd < 0) return 0;
    int count = 0;
    char *line;
    while ((line = get_next_line(fd))) { free(line); count++; }
    close(fd);
	// printf("Lignes lues: %d, Attendu: %d\n", count, nb);
    return (count == nb);
}

int main(void) {
    int success;
    const char *lines1[] = {"Hello world\n", "42\n", "Test\n"};
    printf("Test basique (3 lignes)\n");
    success = test_basic("test1.txt", lines1, 3);
    print_test_status(success);

    printf("Test fichier vide\n");
    success = test_basic("empty.txt", NULL, 0);
    print_test_status(success);

    const char *lines2[] = {"Unique line"};
    printf("Test une ligne sans newline\n");
    success = test_basic("single_no_nl.txt", lines2, 1);
    print_test_status(success);

    const char *lines3[] = {"Unique line\n"};
    printf("Test une ligne avec newline\n");
    success = test_basic("single_with_nl.txt", lines3, 1);
    print_test_status(success);

    char long_line[4097];
    memset(long_line, 'A', 4096); long_line[4096] = '\0';
    printf("Test longue ligne (4096 chars)\n");
    success = test_long_line("long_line.txt", long_line);
    print_test_status(success);

    printf("Test 10 lignes vides\n");
    success = test_empty_lines("empty_lines.txt", 10);
    print_test_status(success);

    printf("Test pas de newline final\n");
    success = test_no_newline_final("no_nl_final.txt", "No newline at end");
    print_test_status(success);

    printf("Test multi-fd\n");
    success = test_multi_fd("test1.txt", "single_with_nl.txt", "Hello world\n", "Unique line\n");
    print_test_status(success);

    printf("Test fd invalide\n");
    success = test_invalid_fd();
    print_test_status(success);

    printf("Stress test 1000 lignes\n");
    success = test_stress("stress.txt", 1000);
    print_test_status(success);

    return 0;
}
