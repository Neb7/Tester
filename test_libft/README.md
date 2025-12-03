# Libft Tester

Testeur complet pour le projet libft de 42 avec protection contre les crashes.

## Fonctionnalités

✅ **Protection par Fork**: Chaque test s'exécute dans un processus séparé
- Protection contre les segfaults
- Protection contre les bus errors  
- Protection contre les infinite loops (timeout 2s)

✅ **Messages d'erreur détaillés**: Indique exactement ce qui n'a pas fonctionné dans chaque test

✅ **Résumé complet**: Affiche le nombre total de tests, réussis et échoués

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

Chaque test affiche :
- `[OK]` en vert si le test passe
- `[KO]` en rouge si le test échoue (avec détails de l'erreur)
- `[CRASH: Signal X]` en rouge si segfault/bus error
- `[TIMEOUT]` en jaune si le test prend trop de temps

À la fin, un résumé indique :
- Nombre total de tests
- Nombre de tests réussis (en vert)
- Nombre de tests échoués (en rouge si > 0)
- Message final de succès ✅ ou d'échec ❌

## Structure des tests

### test_libft.c
- **Part 1**: Fonctions de la libc (is*, str*, mem*, atoi, to*)
- **Part 2**: Fonctions additionnelles (substr, strjoin, split, itoa, put*, etc.)

### test_libft_bonus.c
- Tests des fonctions de listes chaînées (lst*)

## Avantages du système de fork

1. **Sécurité**: Un crash dans un test n'arrête pas les autres tests
2. **Isolation**: Chaque test démarre dans un environnement propre
3. **Détection**: Identifie le type exact d'erreur (segfault, timeout, etc.)
4. **Fiabilité**: Continue à tester même si certaines fonctions sont cassées

## Notes

- Les tests timeout après 2 secondes (configurable dans `run_test_fork`)
- Les messages d'erreur détaillés apparaissent sous le `[KO]`
- Compatible Linux (utilise `fork`, `waitpid`, `signal`)
