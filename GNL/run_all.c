/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_all.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: benpicar <benpicar@student.42mulhouse.fr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 15:30:00 by benpicar          #+#    #+#             */
/*   Updated: 2025/12/03 15:28:18 by benpicar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define GREEN "\033[0;32m"
#define RED   "\033[0;31m"
#define YELLOW "\033[0;33m"
#define NC    "\033[0m"

typedef struct s_result {
    int buffer_size;
    int passed;
    int failed;
    int total;
} t_result;

int compile_and_run(int buffer_size, t_result *result) {
    char cmd[256];
    char buffer[1024];
    FILE *fp;
    
    result->buffer_size = buffer_size;
    result->passed = 0;
    result->failed = 0;
    result->total = 0;
    
    // Nettoyer et compiler
    printf("%s--- Test avec BUFFER_SIZE=%d ---%s\n", YELLOW, buffer_size, NC);
    
    snprintf(cmd, sizeof(cmd), "make fclean > /dev/null 2>&1 && make BUFFER_SIZE=%d > /dev/null 2>&1", buffer_size);
    if (system(cmd) != 0) {
        printf("%sErreur de compilation%s\n\n", RED, NC);
        return 1;
    }
    
    // Exécuter le tester et capturer la sortie
    fp = popen("./tester_gnl 2>&1", "r");
    if (!fp) {
        printf("%sErreur d'exécution%s\n\n", RED, NC);
        return 1;
    }
    
    // Afficher et analyser la sortie
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        printf("%s", buffer);
        
        // Chercher "Total: X tests"
        char *ptr;
        if ((ptr = strstr(buffer, "Total:")) != NULL) {
            sscanf(ptr, "Total: %d", &result->total);
        }
        // Chercher "Passed: X"
        if ((ptr = strstr(buffer, "Passed:")) != NULL) {
            sscanf(ptr, "Passed: %d", &result->passed);
        }
        // Chercher "Failed: X"
        if ((ptr = strstr(buffer, "Failed:")) != NULL) {
            sscanf(ptr, "Failed: %d", &result->failed);
        }
    }
    
    int status = pclose(fp);
    printf("\n");
    
    return WEXITSTATUS(status);
}

int main(void) {
    int buffer_sizes[] = {1, 10, 42, 1000, 10000};
    int nb_sizes = 5;
    t_result results[5];
    
    printf("%s=== GET_NEXT_LINE TESTER - TOUS LES BUFFER_SIZE ===%s\n\n", GREEN, NC);
    
    // Lancer tous les tests
    for (int i = 0; i < nb_sizes; i++) {
        compile_and_run(buffer_sizes[i], &results[i]);
    }
    
    // Afficher le résumé global
    printf("%s===========================================\n", GREEN);
    printf("           RÉSUMÉ DES TESTS\n");
    printf("===========================================\n");
    printf("BUFFER_SIZE  | Passed | Failed | Status\n");
    printf("-------------|--------|--------|--------%s\n", NC);
    
    int total_failed = 0;
    for (int i = 0; i < nb_sizes; i++) {
        total_failed += results[i].failed;
        
        const char *color = results[i].failed == 0 ? GREEN : RED;
        const char *status = results[i].failed == 0 ? "✅" : "❌";
        
        printf("%-12d | %s%6d%s | %s%6d%s | %s\n",
               results[i].buffer_size,
               GREEN, results[i].passed, NC,
               color, results[i].failed, NC,
               status);
    }
    
    printf("%s===========================================\n%s", GREEN, NC);
    
    if (total_failed > 0) {
        printf("\n%s❌ Certains tests ont échoué.%s\n", RED, NC);
        return 1;
    } else {
        printf("\n%s✅ Tous les tests sont passés avec succès !%s\n", GREEN, NC);
        return 0;
    }
}
