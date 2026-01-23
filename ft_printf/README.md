# ft_printf Tester

Testeur complet pour le projet **ft_printf** de 42.

## Description

Ce tester vérifie automatiquement votre implémentation de `ft_printf` avec toutes les conversions :

### Conversions obligatoires
- `%c` - Caractère
- `%s` - Chaîne de caractères
- `%p` - Pointeur (format hexadécimal)
- `%d` - Nombre décimal
- `%i` - Entier (base 10)
- `%u` - Nombre décimal non signé
- `%x` - Hexadécimal (minuscules)
- `%X` - Hexadécimal (majuscules)
- `%%` - Symbole pourcentage

### Conversions bonus
- `-` - Justification à gauche
- `0` - Padding avec zéros
- `.` - Précision
- `#` - Forme alternative
- `+` - Afficher le signe
- ` ` (espace) - Espace avant les nombres positifs
- Largeur de champ

Le tester compare les résultats de votre `ft_printf()` avec le `printf()` original pour vérifier :
- ✅ La sortie correcte
- ✅ La valeur de retour (nombre de caractères imprimés)
- ✅ La gestion des cas limites (NULL, INT_MAX, INT_MIN, etc.)
- ✅ La protection contre les crashs (segfault, bus error)
- ✅ Les timeouts

## Fonctionnalités

🔒 **Protection par fork** : Chaque test s'exécute dans un processus séparé pour éviter qu'un crash n'arrête toute la suite de tests.

⏱️ **Timeout** : Les tests qui prennent trop de temps (boucle infinie) sont automatiquement interrompés après 2 secondes.

📊 **Statistiques détaillées** : Affichage du nombre de tests réussis/échoués et du taux de réussite.

🎨 **Interface colorée** : Résultats faciles à lire avec des couleurs.

🚫 **Sortie masquée** : Les printf des tests ne polluent pas l'affichage (redirection vers /dev/null).

## Structure

```
ft_printf/
├── tester_printf.c         # Tester pour les conversions obligatoires
├── tester_printf_bonus.c   # Tester pour les flags bonus
├── Makefile                # Makefile pour compiler et exécuter
├── run_tests.sh            # Script pour exécuter facilement
└── README.md               # Ce fichier
```

## Utilisation

### Prérequis

Votre implémentation de `ft_printf` doit être dans le dossier parent de ce tester.
- Votre Makefile doit générer `libftprintf.a`
- Votre header `ft_printf.h` doit contenir le prototype : `int ft_printf(const char *, ...);`

### Installation

1. Clonez ou copiez ce tester dans votre dossier ft_printf :
```bash
cd ft_printf
# Copiez les fichiers du tester ici
```

2. Modifiez le Makefile si nécessaire :
   - Si votre ft_printf est dans un sous-dossier, changez la variable `PRINTF_DIR`
   - Par défaut, `PRINTF_DIR = .` (dossier courant)

### Compilation et Exécution

#### Méthode 1 : Utiliser le Makefile

```bash
# Compiler et exécuter
make run

# Compiler seulement
make

# Exécuter avec valgrind
make valgrind

# Nettoyer
make clean      # Supprime les .o
make fclean     # Supprime tout
make re         # Recompile tout
```

#### Méthode 2 : Utiliser le script

```bash
# Rendre le script exécutable (une seule fois)
chmod +x run_tests.sh

# Lancer les tests
./run_tests.sh

# Avec valgrind
./run_tests.sh valgrind
```

#### Méthode 3 : Manuel

```bash
# Compiler votre ft_printf
make

# Compiler le tester
cc -Wall -Wextra -Werror tester_printf.c libftprintf.a -o tester_printf

# Exécuter
./tester_printf
```

## Interprétation des résultats

### Codes de résultat

- 🟢 `[OK]` : Test réussi
- 🔴 `[KO]` : Test échoué (sortie ou valeur de retour incorrecte)
- 🔴 `[CRASH: SEGFAULT]` : Segmentation fault
- 🔴 `[CRASH: BUS ERROR]` : Bus error
- 🔴 `[CRASH: ABORT]` : Abort signal
- 🟡 `[TIMEOUT]` : Le test a pris trop de temps (probable boucle infinie)
- 🔴 `[ERROR]` : Erreur système

### Exemple de sortie

```
╔════════════════════════════════════════════════════════════════════════╗
║                          FT_PRINTF TESTER                              ║
╚════════════════════════════════════════════════════════════════════════╝

╔════════════════════════════════════════════════════════════════════════╗
║ TESTS: %c (character)                                                  ║
╚════════════════════════════════════════════════════════════════════════╝

test_char_simple                                            [OK]
test_char_multiple                                          [OK]
test_char_null                                              [OK]
test_char_special                                           [OK]

...

╔════════════════════════════════════════════════════════════════════════╗
║ SUMMARY                                                                ║
╠════════════════════════════════════════════════════════════════════════╣
║ Total tests: 42                                                        ║
║ Passed: 42                                                             ║
║ Failed: 0                                                              ║
║ Success rate: 100.00%                                                  ║
╚════════════════════════════════════════════════════════════════════════╝

🎉 All tests passed! Congratulations! 🎉
```

## Tests inclus

### Tests %c (4 tests)
- Caractère simple
- Caractères multiples
- Caractère NULL
- Caractères spéciaux (tab, newline, space)

### Tests %s (5 tests)
- Chaîne simple
- Chaîne vide
- Pointeur NULL
- Chaînes multiples
- Chaîne longue

### Tests %p (3 tests)
- Pointeur simple
- Pointeur NULL
- Pointeurs multiples

### Tests %d et %i (7 tests)
- Entier simple
- Zéro
- Nombre négatif
- INT_MAX
- INT_MIN
- Conversion %i
- Entiers multiples

### Tests %u (4 tests)
- Unsigned simple
- Zéro
- UINT_MAX
- Nombre négatif (comportement unsigned)

### Tests %x (4 tests)
- Hexadécimal simple
- Zéro
- UINT_MAX
- Lettres hexadécimales

### Tests %X (4 tests)
- Hexadécimal majuscule simple
- Zéro
- UINT_MAX
- Lettres hexadécimales

### Tests %% (2 tests)
- Pourcentage simple
- Pourcentages multiples

### Tests mixtes (4 tests)
- Toutes les conversions ensemble
- Cas complexes
- Sans format
- Chaîne vide

**Total : 37+ tests**

## Débogage

Si vous avez des erreurs :

1. **Vérifiez la compilation** :
```bash
make fclean && make
```

2. **Testez avec valgrind** :
```bash
make valgrind
```

3. **Ajoutez des printf de débogage** dans votre ft_printf pour voir où ça bloque

4. **Testez manuellement** un cas qui échoue :
```bash
./tester_printf 2>&1 | grep "test_name"
```

## Personnalisation

Vous pouvez facilement ajouter vos propres tests en modifiant `tester_printf.c` :

```c
int test_my_custom() {
    int ret1 = ft_printf("Your format: %d\n", 42);
    int ret2 = printf("Your format: %d\n", 42);
    if (ret1 != ret2)
        return 1;
    return 0;
}

// Puis dans main() :
run_test_fork("test_my_custom", test_my_custom);
```

## Limitations

⚠️ Ce tester ne teste **pas** les bonus (flags `-0.`, `# +`, field width, etc.)

⚠️ Le tester compare uniquement les valeurs de retour, pas exactement la sortie caractère par caractère (à cause de la complexité du fork et des pipes)

⚠️ Pour les pointeurs, le format peut varier selon les systèmes (0x0, (nil), etc.)

## Ressources

### Documentation
- [man printf](https://man7.org/linux/man-pages/man3/printf.3.html)
- [Variadic functions in C](https://en.cppreference.com/w/c/variadic)
- [stdarg.h documentation](https://en.cppreference.com/w/c/variadic)

### Articles et tutoriels
- [How to implement printf](https://www.cypress.com/file/54761/download)
- [Understanding variadic arguments](https://www.geeksforgeeks.org/variadic-functions-in-c/)

### Utilisation de l'IA
L'IA (ChatGPT, GitHub Copilot) a été utilisée pour :
- 🚫 **PAS** pour générer le code de ft_printf
- ✅ Comprendre le fonctionnement de va_arg, va_start, va_end
- ✅ Déboguer des erreurs de compilation spécifiques
- ✅ Optimiser la structure de ce tester
- ✅ Améliorer la lisibilité du code de test

**Important** : Le code de ft_printf doit être écrit par vous-même pour l'apprentissage !

## Contribution

Si vous trouvez des bugs ou voulez ajouter des tests, n'hésitez pas à améliorer ce tester !

## Licence

Libre d'utilisation pour les étudiants de 42. Pensez à créditer si vous partagez !

---

**Bon courage pour votre ft_printf ! 🚀**
