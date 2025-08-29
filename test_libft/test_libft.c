/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_libft.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: benpicar <benpicar@student.42mulhouse.fr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 03:04:01 by benpicar          #+#    #+#             */
/*   Updated: 2025/08/29 03:48:24 by benpicar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

#define GREEN "\033[32m"
#define RED   "\033[31m"
#define NC    "\033[0m"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>

void test_isalpha() {
    printf("ft_isalpha vs isalpha:\n");
    int ok = 1;
    for (int c = 0; c <= 255; c++) {
        int ft = ft_isalpha(c) ? 1 : 0;
        int std = isalpha(c) ? 1 : 0;
        if (ft != std) {
            printf("\033[31m[KO]\033[0m: c=%d ft_isalpha=%d isalpha=%d\n", c, ft, std);
            ok = 0;
            break;
        }
    }
    if (ok)
        printf("%s[OK]\n%s", GREEN, NC);
}

void test_isdigit() {
    printf("ft_isdigit vs isdigit:\n");
    int ok = 1;
    for (int c = 0; c <= 255; c++) {
        int ft = ft_isdigit(c) ? 1 : 0;
        int std = isdigit(c) ? 1 : 0;
        if (ft != std) {
            printf("\033[31m[KO]\033[0m: c=%d ft_isdigit=%d isdigit=%d\n", c, ft, std);
            ok = 0;
            break;
        }
    }
    if (ok)
        printf("%s[OK]\n%s", GREEN, NC);
}

void test_isalnum() {
    printf("ft_isalnum vs isalnum:\n");
    int ok = 1;
    for (int c = 0; c <= 255; c++) {
        int ft = ft_isalnum(c) ? 1 : 0;
        int std = isalnum(c) ? 1 : 0;
        if (ft != std) {
            printf("\033[31m[KO]\033[0m: c=%d ft_isalnum=%d isalnum=%d\n", c, ft, std);
            ok = 0;
            break;
        }
    }
    if (ok)
        printf("%s[OK]\n%s", GREEN, NC);
}

void test_isascii() {
    printf("ft_isascii vs isascii:\n");
    int ok = 1;
    for (int c = 0; c <= 255; c++) {
        int ft = ft_isascii(c) ? 1 : 0;
        int std = isascii(c) ? 1 : 0;
        if (ft != std) {
            printf("\033[31m[KO]\033[0m: c=%d ft_isascii=%d isascii=%d\n", c, ft, std);
            ok = 0;
            break;
        }
    }
    if (ok)
        printf("%s[OK]\n%s", GREEN, NC);
}

void test_isprint() {
    printf("ft_isprint vs isprint:\n");
    int ok = 1;
    for (int c = 0; c <= 255; c++) {
        int ft = ft_isprint(c) ? 1 : 0;
        int std = isprint(c) ? 1 : 0;
        if (ft != std) {
            printf("\033[31m[KO]\033[0m: c=%d ft_isprint=%d isprint=%d\n", c, ft, std);
            ok = 0;
            break;
        }
    }
    if (ok)
        printf("%s[OK]\n%s", GREEN, NC);
}

void test_strlen() {
    printf("ft_strlen vs strlen:\n");
    const char *tests[] = {"", "a", "abc", "Hello, world!", NULL};
    int ok = 1;
    for (int i = 0; tests[i]; i++) {
        size_t ft = ft_strlen(tests[i]);
        size_t std = strlen(tests[i]);
        if (ft != std) {
            printf("\033[31m[KO]\033[0m: s=\"%s\" ft_strlen=%zu strlen=%zu\n", tests[i], ft, std);
            ok = 0;
            break;
        }
    }
    if (ok) printf("%s[OK]\n%s", GREEN, NC);
}

void test_strlcpy() {
    printf("ft_strlcpy vs strlcpy:\n");
    const char *srcs[] = {"Hello", "", "abc", NULL};
    int ok = 1;
    for (int i = 0; srcs[i]; i++) {
        char d1[20] = {0}/*, d2[20] = {0}*/;
        size_t r1 = ft_strlcpy(d1, srcs[i], 20);
        // size_t r2 = strlcpy(d2, srcs[i], 20); // Non portable sur Linux
        size_t r2 = strlen(srcs[i]);
        if (r1 != r2 || strcmp(d1, srcs[i]) != 0) {
            printf("\033[31m[KO]\033[0m: src=\"%s\" ft_strlcpy=%zu d1=\"%s\"\n", srcs[i], r1, d1);
            ok = 0;
            break;
        }
    }
    if (ok) printf("%s[OK]\n%s", GREEN, NC);
}

void test_strlcat() {
    printf("ft_strlcat vs strlcat:\n");
    char d1[20] = "Hi ";
    const char *src = "there";
    size_t r1 = ft_strlcat(d1, src, 20);
    // Pas de strlcat standard sur Linux, on vérifie juste le résultat attendu
    if (strcmp(d1, "Hi there") == 0 && r1 == strlen("Hi ") + strlen(src))
        printf("%s[OK]\n%s", GREEN, NC);
    else
        printf("\033[31m[KO]\033[0m: dest=\"%s\" r1=%zu\n", d1, r1);
}

void test_strchr() {
    printf("ft_strchr vs strchr:\n");
    const char *s = "Hello";
    int ok = 1;
    for (int c = 0; c < 128; c++) {
        char *ft = ft_strchr(s, c);
        char *std = strchr(s, c);
        if ((ft == NULL) != (std == NULL) || (ft && std && ft - s != std - s)) {
            printf("\033[31m[KO]\033[0m: c=%d ft_strchr=%p strchr=%p\n", c, (void*)ft, (void*)std);
            ok = 0;
            break;
        }
    }
    if (ok) printf("%s[OK]\n%s", GREEN, NC);
}

void test_strrchr() {
    printf("ft_strrchr vs strrchr:\n");
    const char *s = "Hello";
    int ok = 1;
    for (int c = 0; c < 128; c++) {
        char *ft = ft_strrchr(s, c);
        char *std = strrchr(s, c);
        if ((ft == NULL) != (std == NULL) || (ft && std && ft - s != std - s)) {
            printf("\033[31m[KO]\033[0m: c=%d ft_strrchr=%p strrchr=%p\n", c, (void*)ft, (void*)std);
            ok = 0;
            break;
        }
    }
    if (ok) printf("%s[OK]\n%s", GREEN, NC);
}

void test_strncmp() {
    printf("ft_strncmp vs strncmp:\n");
    const char *a[] = {"abc", "abc", "abc", "abc", "", "abc", NULL};
    const char *b[] = {"abc", "abd", "", "abcde", "", "", NULL};
    size_t n[] = {3, 2, 1, 5, 0, 1};
    int ok = 1;
    for (int i = 0; a[i]; i++) {
        int ft = ft_strncmp(a[i], b[i], n[i]);
        int std = strncmp(a[i], b[i], n[i]);
        if ((ft == 0) != (std == 0)) {
            printf("\033[31m[KO]\033[0m: \"%s\" \"%s\" n=%zu ft_strncmp=%d strncmp=%d\n", a[i], b[i], n[i], ft, std);
            ok = 0;
            break;
        }
    }
    if (ok) printf("%s[OK]\n%s", GREEN, NC);
}

void test_strnstr() {
    printf("ft_strnstr vs strnstr:\n");
    const char *big = "Hello world";
    const char *little = "world";
    char *ft = ft_strnstr(big, little, 11);
    char *std = strstr(big, little); // strnstr n'existe pas sur Linux, on compare à strstr si len assez grand
    if ((ft == NULL && std == NULL) || (ft && std && ft - big == std - big))
        printf("%s[OK]\n%s", GREEN, NC);
    else
        printf("\033[31m[KO]\033[0m: ft_strnstr=%p strstr=%p\n", (void*)ft, (void*)std);
}

void test_memset() {
    printf("ft_memset vs memset:\n");
    char a[10], b[10];
    memset(b, 'A', 5);
    ft_memset(a, 'A', 5);
    int ok = 1;
    for (int i = 0; i < 5; i++) {
        if (a[i] != b[i]) {
            printf("\033[31m[KO]\033[0m: i=%d a=%d b=%d\n", i, a[i], b[i]);
            ok = 0;
            break;
        }
    }
    if (ok) printf("%s[OK]\n%s", GREEN, NC);
}

void test_bzero() {
    printf("ft_bzero vs bzero:\n");
    char a[10];
    ft_bzero(a, 10);
    int ok = 1;
    for (int i = 0; i < 10; i++) {
        if (a[i] != 0) {
            printf("\033[31m[KO]\033[0m: i=%d a=%d\n", i, a[i]);
            ok = 0;
            break;
        }
    }
    if (ok) printf("%s[OK]\n%s", GREEN, NC);
}

void test_memcpy() {
    printf("ft_memcpy vs memcpy:\n");
    char a[10], b[10];
    ft_memcpy(a, "12345", 6);
    memcpy(b, "12345", 6);
    if (memcmp(a, b, 6) == 0)
        printf("%s[OK]\n%s", GREEN, NC);
    else
        printf("\033[31m[KO]\033[0m: a=\"%s\" b=\"%s\"\n", a, b);
}

void test_memmove() {
    printf("ft_memmove vs memmove:\n");
    char a[10] = "abcdefg";
    char b[10] = "abcdefg";
    ft_memmove(a+2, a, 5);
    memmove(b+2, b, 5);
    if (memcmp(a, b, 10) == 0)
        printf("%s[OK]\n%s", GREEN, NC);
    else
        printf("\033[31m[KO]\033[0m: a=\"%s\" b=\"%s\"\n", a, b);
}

void test_memchr() {
    printf("ft_memchr vs memchr:\n");
    char s[] = "abcde";
    int ok = 1;
    for (int c = 0; c < 128; c++) {
        void *ft = ft_memchr(s, c, 5);
        void *std = memchr(s, c, 5);
        if (ft != std) {
            printf("\033[31m[KO]\033[0m: c=%d ft_memchr=%p memchr=%p\n", c, ft, std);
            ok = 0;
            break;
        }
    }
    if (ok) printf("%s[OK]\n%s", GREEN, NC);
}

void test_memcmp() {
    printf("ft_memcmp vs memcmp:\n");
    const char *a[] = {"abc", "abc", "abc", "", NULL};
    const char *b[] = {"abc", "abd", "", "", NULL};
    size_t n[] = {3, 3, 1, 0};
    int ok = 1;
    for (int i = 0; a[i]; i++) {
        int ft = ft_memcmp(a[i], b[i], n[i]);
        int std = memcmp(a[i], b[i], n[i]);
        if ((ft == 0) != (std == 0)) {
            printf("\033[31m[KO]\033[0m: \"%s\" \"%s\" n=%zu ft_memcmp=%d memcmp=%d\n", a[i], b[i], n[i], ft, std);
            ok = 0;
            break;
        }
    }
    if (ok) printf("%s[OK]\n%s", GREEN, NC);
}

void test_atoi() {
    printf("ft_atoi vs atoi:\n");
    const char *tests[] = {"42", "-42", "0", "  123", "abc", "2147483647", "-2147483648", NULL};
    int ok = 1;
    for (int i = 0; tests[i]; i++) {
        int ft = ft_atoi(tests[i]);
        int std = atoi(tests[i]);
        if (ft != std) {
            printf("\033[31m[KO]\033[0m: s=\"%s\" ft_atoi=%d atoi=%d\n", tests[i], ft, std);
            ok = 0;
            break;
        }
    }
    if (ok) printf("%s[OK]\n%s", GREEN, NC);
}

void test_toupper() {
    printf("ft_toupper vs toupper:\n");
    int ok = 1;
    for (int c = 0; c <= 255; c++) {
        int ft = ft_toupper(c);
        int std = toupper(c);
        if (ft != std) {
            printf("\033[31m[KO]\033[0m: c=%d ft_toupper=%d toupper=%d\n", c, ft, std);
            ok = 0;
            break;
        }
    }
    if (ok) printf("%s[OK]\n%s", GREEN, NC);
}

void test_tolower() {
    printf("ft_tolower vs tolower:\n");
    int ok = 1;
    for (int c = 0; c <= 255; c++) {
        int ft = ft_tolower(c);
        int std = tolower(c);
        if (ft != std) {
            printf("\033[31m[KO]\033[0m: c=%d ft_tolower=%d tolower=%d\n", c, ft, std);
            ok = 0;
            break;
        }
    }
    if (ok) printf("%s[OK]\n%s", GREEN, NC);
}

void test_strdup() {
    printf("ft_strdup vs strdup:\n");
    const char *tests[] = {"hello", "", "abc", NULL};
    int ok = 1;
    for (int i = 0; tests[i]; i++) {
        char *a = ft_strdup(tests[i]);
        char *b = strdup(tests[i]);
        if (strcmp(a, b) != 0) {
            printf("\033[31m[KO]\033[0m: s=\"%s\" ft_strdup=\"%s\" strdup=\"%s\"\n", tests[i], a, b);
            ok = 0;
            free(a); free(b);
            break;
        }
        free(a); free(b);
    }
    if (ok) printf("%s[OK]\n%s", GREEN, NC);
}

void test_calloc() {
    printf("ft_calloc vs calloc:\n");
    int ok = 1;
    for (size_t n = 1; n <= 10; n++) {
        char *a = ft_calloc(n, 1);
        char *b = calloc(n, 1);
        if (memcmp(a, b, n) != 0) {
            printf("\033[31m[KO]\033[0m: n=%zu\n", n);
            ok = 0;
            free(a); free(b);
            break;
        }
        free(a); free(b);
    }
    if (ok) printf("%s[OK]\n%s", GREEN, NC);
}

// Part 2

void test_itoa() {
    printf("ft_itoa:\n");
    int tests[] = {42, -42, 0, 2147483647, -2147483648};
    char buf[32];
    int ok = 1;
    for (int i = 0; i < 5; i++) {
        char *ft = ft_itoa(tests[i]);
        snprintf(buf, sizeof(buf), "%d", tests[i]);
        if (strcmp(ft, buf) != 0) {
            printf("\033[31m[KO]\033[0m: n=%d ft_itoa=\"%s\" ref=\"%s\"\n", tests[i], ft, buf);
            ok = 0;
            free(ft);
            break;
        }
        free(ft);
    }
    if (ok) printf("%s[OK]\n%s", GREEN, NC);
}

void test_putchar_fd() {
    printf("ft_putchar_fd (should print 'A' then newline): ");
    ft_putchar_fd('A', 1);
    ft_putchar_fd('\n', 1);
    printf("%s[OK]\n%s", GREEN, NC);
}

void test_putendl_fd() {
    printf("ft_putendl_fd (should print 'Hello' then newline): ");
    ft_putendl_fd("Hello", 1);
    printf("%s[OK]\n%s", GREEN, NC);
}

void test_putnbr_fd() {
    printf("ft_putnbr_fd (should print 12345 then newline): ");
    ft_putnbr_fd(12345, 1);
    ft_putchar_fd('\n', 1);
    printf("%s[OK]\n%s", GREEN, NC);
}

void test_putstr_fd() {
    printf("ft_putstr_fd (should print 'World' then newline): ");
    ft_putstr_fd("World", 1);
    ft_putchar_fd('\n', 1);
    printf("%s[OK]\n%s", GREEN, NC);
}

void test_split() {
    printf("ft_split:\n");
    char **tab = ft_split("a,b,c", ',');
    int ok = 1;
    const char *ref[] = {"a", "b", "c", NULL};
    for (int i = 0; ref[i]; i++) {
        if (!tab[i] || strcmp(tab[i], ref[i]) != 0) {
            printf("\033[31m[KO]\033[0m: i=%d ft_split=\"%s\" ref=\"%s\"\n", i, tab[i], ref[i]);
            ok = 0;
            break;
        }
    }
    if (ok && tab[3] != NULL) {
        printf("\033[31m[KO]\033[0m: tab[3] should be NULL\n");
        ok = 0;
    }
    for (int i = 0; tab && tab[i]; i++) free(tab[i]);
    free(tab);
    if (ok) printf("%s[OK]\n%s", GREEN, NC);
}

void test_strjoin() {
    printf("ft_strjoin:\n");
    char *s = ft_strjoin("Hello", "World");
    if (strcmp(s, "HelloWorld") == 0)
        printf("%s[OK]\n%s", GREEN, NC);
    else
        printf("\033[31m[KO]\033[0m: ft_strjoin=\"%s\" ref=\"HelloWorld\"\n", s);
    free(s);
}

void test_strtrim() {
    printf("ft_strtrim:\n");
    char *s = ft_strtrim("  Hello  ", " ");
    if (strcmp(s, "Hello") == 0)
        printf("%s[OK]\n%s", GREEN, NC);
    else
        printf("\033[31m[KO]\033[0m: ft_strtrim=\"%s\" ref=\"Hello\"\n", s);
    free(s);
}

void test_substr() {
    printf("ft_substr:\n");
    char *s = ft_substr("HelloWorld", 5, 5);
    if (strcmp(s, "World") == 0)
        printf("%s[OK]\n%s", GREEN, NC);
    else
        printf("\033[31m[KO]\033[0m: ft_substr=\"%s\" ref=\"World\"\n", s);
    free(s);
}

void f_iteri(unsigned int i, char *c) {
    if (c)
        *c = *c + i;
}

void test_striteri() {
    printf("ft_striteri:\n");
    char s[] = "abcd";
    ft_striteri(s, f_iteri);
    if (strcmp(s, "aceg") == 0)
        printf("%s[OK]\n%s", GREEN, NC);
    else
        printf("\033[31m[KO]\033[0m: ft_striteri=\"%s\" ref=\"aceg\"\n", s);
}

char f_mapi(unsigned int i, char c) {
    return c + i;
}

void test_strmapi() {
    printf("ft_strmapi:\n");
    char *s = ft_strmapi("abcd", f_mapi);
    if (strcmp(s, "aceg") == 0)
        printf("%s[OK]\n%s", GREEN, NC);
    else
        printf("\033[31m[KO]\033[0m: ft_strmapi=\"%s\" ref=\"aceg\"\n", s);
    free(s);
}

int main() {
    test_isalpha();
    test_isdigit();
    test_isalnum();
    test_isascii();
    test_isprint();
    test_strlen();
    test_strlcpy();
    test_strlcat();
    test_strchr();
    test_strrchr();
    test_strncmp();
    test_strnstr();
    test_memset();
    test_bzero();
    test_memcpy();
    test_memmove();
    test_memchr();
    test_memcmp();
    test_atoi();
    test_toupper();
    test_tolower();
    test_strdup();
    test_calloc();

	printf("\n------Part 2------\n\n");

	test_itoa();
    test_putchar_fd();
    test_putendl_fd();
    test_putnbr_fd();
    test_putstr_fd();
    test_split();
    test_strjoin();
    test_strtrim();
    test_substr();
    test_striteri();
    test_strmapi();
	
    return (0);
}
