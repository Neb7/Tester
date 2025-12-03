# Améliorations du Tester Libft

## 🔄 Changements principaux

### 1. Système de Fork
Chaque test s'exécute maintenant dans un processus séparé (fork), ce qui apporte :

#### Avantages
- **Protection contre les crashes** : Un segfault dans un test n'arrête pas les autres
- **Détection précise des erreurs** : Identifie SEGFAULT, BUS ERROR, ABORT
- **Timeout automatique** : Les boucles infinies sont détectées (2s max par test)
- **Isolation** : Chaque test démarre dans un environnement propre

#### Comment ça marche
```c
int run_test_fork(const char *test_name, test_func func) {
    pid_t pid = fork();
    
    if (pid == 0) {
        // Processus enfant : exécute le test
        int result = func();
        exit(result);
    }
    
    // Processus parent : surveille le test
    waitpid(pid, &status, ...);
    // Détecte : crash, timeout, succès, échec
}
```

### 2. Messages d'erreur détaillés

#### Avant
```
ft_strlen vs strlen:
[KO]: s="abc" ft_strlen=5 strlen=3
```

#### Après
```
ft_strlen: [KO]
  Error: s="abc" ft_strlen=5 expected=3
```

Chaque erreur indique maintenant :
- **Ce qui a été testé** : la valeur d'entrée
- **Ce qui a été obtenu** : le résultat de votre fonction
- **Ce qui était attendu** : le résultat correct

### 3. Types de résultats

| Résultat | Couleur | Signification |
|----------|---------|---------------|
| `[OK]` | 🟢 Vert | Test réussi |
| `[KO]` | 🔴 Rouge | Test échoué (logique incorrecte) |
| `[CRASH: SEGFAULT]` | 🔴 Rouge | Segmentation fault |
| `[CRASH: BUS ERROR]` | 🔴 Rouge | Bus error |
| `[CRASH: ABORT]` | 🔴 Rouge | Abort (assertion, malloc fail, etc.) |
| `[TIMEOUT]` | 🟡 Jaune | Boucle infinie ou trop lent |
| `[ERROR: fork failed]` | 🔴 Rouge | Erreur système |

### 4. Résumé final

#### Avant
```
Tous les tests sont passés avec succès !
```
ou
```
Certains tests ont échoué.
```

#### Après
```
===================
Total: 33 tests
Passed: 31
Failed: 2

❌ Certains tests ont échoué.
```

Le résumé indique maintenant :
- **Nombre total de tests**
- **Nombre de tests réussis** (en vert)
- **Nombre de tests échoués** (en rouge si > 0)
- **Message clair** avec emoji

### 5. Structure améliorée

#### Tests retournent maintenant un code d'erreur
```c
// Avant
void test_strlen() {
    // ... tests ...
    if (ok) printf("[OK]\n");
}

// Après
int test_strlen() {
    // ... tests ...
    if (error) {
        printf("  Error: ...\n");
        return 1;  // Échec
    }
    return 0;  // Succès
}
```

### 6. Facilité d'utilisation

#### Makefile
```bash
make          # Compile le tester
make test     # Compile et lance les tests
make bonus    # Compile le tester bonus
make test_bonus  # Compile et lance les tests bonus
```

#### Script automatique
```bash
./run_tests.sh  # Lance automatiquement tous les tests disponibles
```

## 📊 Comparaison

| Fonctionnalité | Avant | Après |
|----------------|-------|-------|
| Protection segfault | ❌ | ✅ |
| Détection timeout | ❌ | ✅ |
| Messages d'erreur détaillés | ⚠️ Basique | ✅ Complets |
| Résumé des tests | ⚠️ Minimal | ✅ Détaillé |
| Isolation des tests | ❌ | ✅ |
| Code retour significatif | ❌ | ✅ |
| Facilité d'utilisation | ⚠️ | ✅ |

## 🚀 Utilisation

### Installation
Copiez le dossier `test_libft` dans votre projet libft :
```
libft/
├── ft_*.c
├── libft.h
├── Makefile
└── test_libft/
    ├── test_libft.c
    ├── test_libft_bonus.c
    ├── Makefile
    ├── run_tests.sh
    └── README.md
```

### Exécution rapide
```bash
cd test_libft
make test        # Tests de base
make test_bonus  # Tests bonus
```

### Avec le script
```bash
cd test_libft
./run_tests.sh   # Lance tout automatiquement
```

## 🔧 Configuration

### Modifier le timeout
Dans `test_libft.c` et `test_libft_bonus.c` :
```c
int timeout = 2; // Changer cette valeur (en secondes)
```

### Ajouter vos propres tests
```c
int test_ma_fonction() {
    // Votre test ici
    if (erreur) {
        printf("  Error: description\n");
        return 1;
    }
    return 0;
}

// Dans main()
run_test_fork("ma_fonction", test_ma_fonction);
```

## 📝 Notes

- Compatible Linux uniquement (utilise fork, waitpid, signal)
- Nécessite gcc avec -Wall -Wextra -Werror
- Les tests peuvent être lancés individuellement en modifiant le main()
- Le code retour du programme indique le succès (0) ou l'échec (1)
