/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_libft_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: benpicar <benpicar@student.42mulhouse.fr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 03:47:39 by benpicar          #+#    #+#             */
/*   Updated: 2025/08/29 03:50:01 by benpicar         ###   ########.fr       */
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

void test_lstnew_and_add_front_back() {
    printf("ft_lstnew, ft_lstadd_front, ft_lstadd_back:\n");
    t_list *lst = NULL;
    t_list *n1 = ft_lstnew(strdup("one"));
    t_list *n2 = ft_lstnew(strdup("two"));
    t_list *n3 = ft_lstnew(strdup("three"));
    ft_lstadd_front(&lst, n2);
    ft_lstadd_back(&lst, n3);
    ft_lstadd_front(&lst, n1);
    const char *ref[] = {"one", "two", "three", NULL};
    t_list *tmp = lst;
    int ok = 1, i = 0;
    while (tmp && ref[i]) {
        if (strcmp((char*)tmp->content, ref[i]) != 0) {
            printf("\033[31m[KO]\033[0m: i=%d got=\"%s\" ref=\"%s\"\n", i, (char*)tmp->content, ref[i]);
            ok = 0;
            break;
        }
        tmp = tmp->next; i++;
    }
    if (ok && (tmp != NULL || ref[i] != NULL)) {
        printf("\033[31m[KO]\033[0m: list size mismatch\n");
        ok = 0;
    }
    ft_lstclear(&lst, del_content);
    if (ok) printf("%s[OK]\n%s", GREEN, NC);
}

void test_lstsize_and_last() {
    printf("ft_lstsize, ft_lstlast:\n");
    t_list *lst = NULL;
    ft_lstadd_back(&lst, ft_lstnew(strdup("a")));
    ft_lstadd_back(&lst, ft_lstnew(strdup("b")));
    ft_lstadd_back(&lst, ft_lstnew(strdup("c")));
    int size = ft_lstsize(lst);
    t_list *last = ft_lstlast(lst);
    int ok = 1;
    if (size != 3) {
        printf("\033[31m[KO]\033[0m: size=%d ref=3\n", size);
        ok = 0;
    } else if (strcmp((char*)last->content, "c") != 0) {
        printf("\033[31m[KO]\033[0m: last=\"%s\" ref=\"c\"\n", (char*)last->content);
        ok = 0;
    }
    ft_lstclear(&lst, del_content);
    if (ok) printf("%s[OK]\n%s", GREEN, NC);
}

void test_lstdelone() {
    printf("ft_lstdelone:\n");
    t_list *n = ft_lstnew(strdup("to_del"));
    ft_lstdelone(n, del_content);
    printf("%s[OK]\n%s", GREEN, NC);
}

void test_lstiter() {
    printf("ft_lstiter:\n");
    t_list *lst = NULL;
    ft_lstadd_back(&lst, ft_lstnew(strdup("x")));
    ft_lstadd_back(&lst, ft_lstnew(strdup("y")));
    ft_lstadd_back(&lst, ft_lstnew(strdup("z")));
    // On vérifie juste que ça ne crash pas et que les contenus sont accessibles
    ft_lstiter(lst, print_content);
    printf("%s\n[OK]\n%s", GREEN, NC);
    ft_lstclear(&lst, del_content);
}

void *to_upper(void *content) {
    char *s = strdup((char *)content);
    for (int i = 0; s && s[i]; i++)
        if (s[i] >= 'a' && s[i] <= 'z') s[i] -= 32;
    return s;
}

void test_lstmap() {
    printf("ft_lstmap:\n");
    t_list *lst = NULL;
    ft_lstadd_back(&lst, ft_lstnew(strdup("abc")));
    ft_lstadd_back(&lst, ft_lstnew(strdup("def")));
    t_list *mapped = ft_lstmap(lst, to_upper, del_content);
    const char *ref[] = {"ABC", "DEF", NULL};
    t_list *tmp = mapped;
    int ok = 1, i = 0;
    while (tmp && ref[i]) {
        if (strcmp((char*)tmp->content, ref[i]) != 0) {
            printf("\033[31m[KO]\033[0m: i=%d got=\"%s\" ref=\"%s\"\n", i, (char*)tmp->content, ref[i]);
            ok = 0;
            break;
        }
        tmp = tmp->next; i++;
    }
    if (ok && (tmp != NULL || ref[i] != NULL)) {
        printf("\033[31m[KO]\033[0m: list size mismatch\n");
        ok = 0;
    }
    ft_lstclear(&lst, del_content);
    ft_lstclear(&mapped, del_content);
    if (ok) printf("%s[OK]\n%s", GREEN, NC);
}

int main(void)
{
	printf("\n-----------Bonus-----------\n\n");

	test_lstnew_and_add_front_back();
    test_lstsize_and_last();
    test_lstdelone();
    test_lstiter();
    test_lstmap();
    return (0);
}
