# Libft Tester

Testeur complet pour le projet libft de 42 avec protection contre les crashes et tests de protection NULL.

## Fonctionnalités

✅ **Protection par Fork**: Chaque test s'exécute dans un processus séparé
- Protection contre les segfaults
- Protection contre les bus errors  
- Protection contre les infinite loops (timeout 2s)

✅ **Capture de sortie**: Vérifie la sortie des fonctions ft_put* via pipes

✅ **Tests de protection NULL**: Vérifie que vos fonctions gèrent correctement les arguments NULL
- Affichage avec `[🛡]` en vert pour les protections implémentées
- Affichage avec `[⚠]` en jaune pour les protections manquantes
- Ces tests sont **informatifs uniquement** et n'affectent pas le résultat global

✅ **Messages d'erreur détaillés**: Indique exactement ce qui n'a pas fonctionné dans chaque test

✅ **Tests groupés**: Tous les tests d'une même fonction sur une seule ligne

✅ **Résumé complet**: Affiche le nombre total de tests, réussis, échoués et protections NULL

## Utilisation

### Compilation

Pour tester votre libft, copiez ce dossier dans votre projet libft et compilez :

```bash
cd libft/test_libft
make
```

### Exécution

```bash
# Tests de base
./test_libft

# Tests bonus
./test_libft_bonus
```

## Format de sortie

Chaque fonction affiche ses tests sur une seule ligne :
- `[OK]` en vert si le test passe
- `[KO]` en rouge si le test échoue (avec détails de l'erreur)
- `[🛡]` en vert si la protection NULL est implémentée
- `[⚠]` en jaune si la protection NULL est manquante
- `[CRASH: Signal X]` en rouge si segfault/bus error
- `[TIMEOUT]` en jaune si le test prend trop de temps

Exemple :
```
ft_substr: [OK] [🛡]
ft_strjoin: [OK] [⚠] [⚠] [⚠]
ft_putnbr_fd: [OK] [OK] [OK] [OK] [OK]
```

À la fin, un résumé indique :
- Nombre total de tests fonctionnels
- Nombre de tests réussis (en vert)
- Nombre de tests échoués (en rouge si > 0)
- Statistiques de protection NULL (en gris) : X/Y protégées
- Message final de succès ✅ ou d'échec ❌

## Structure des tests

### test_libft.c
- **Part 1**: Fonctions de la libc (is*, str*, mem*, atoi, to*) - 23 tests
- **Part 2**: Fonctions additionnelles (substr, strjoin, split, itoa, put*, etc.) - 29 tests
  - Tests fonctionnels avec capture de sortie pour ft_put*
  - Tests de protection NULL pour 13 fonctions (substr, strjoin, strtrim, split, strmapi, striteri, putstr_fd, putendl_fd)

### test_libft_bonus.c
- Tests des fonctions de listes chaînées (lst*) - 7 tests fonctionnels
- Tests de protection NULL pour 16 cas (lstnew, lstadd_front/back, lstsize, lstlast, lstdelone, lstclear, lstiter, lstmap)

## Avantages du système de fork

1. **Sécurité**: Un crash dans un test n'arrête pas les autres tests
2. **Isolation**: Chaque test démarre dans un environnement propre
3. **Détection**: Identifie le type exact d'erreur (segfault, timeout, etc.)
4. **Fiabilité**: Continue à tester même si certaines fonctions sont cassées
5. **Capture de sortie**: Vérifie précisément ce qui est écrit sur stdout via pipes

## Tests de protection NULL

Les tests de protection NULL vérifient si vos fonctions gèrent correctement les pointeurs NULL en argument. Ces tests sont **informatifs uniquement** :
- ✅ Ils n'affectent **pas** le résultat global (passed/failed)
- ℹ️ Ils vous indiquent simplement quelles protections sont implémentées
- 🛡 Protection implémentée = votre fonction ne crash pas avec NULL
- ⚠ Protection manquante = votre fonction crash ou ne gère pas NULL

Exemple de statistiques :
```
Total: 52 tests
Passed: 52
NULL Protection: 12/13 protected
✅ Tous les tests sont passés avec succès !
```

## Notes

- Les tests timeout après 2 secondes (configurable)
- Les messages d'erreur détaillés apparaissent sous les tests échoués
- Compatible Linux (utilise `fork`, `waitpid`, `signal`, `pipe`)
- Nécessite un système POSIX (Linux, macOS)
