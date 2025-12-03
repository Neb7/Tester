# Guide de démarrage rapide

## 🚀 Démarrage en 3 étapes

### 1. Préparation
```bash
# Placez ce dossier dans votre projet libft
cd votre_libft/test_libft
```

### 2. Compilation
```bash
# Option 1 : Tout en une fois
./run_tests.sh

# Option 2 : Étape par étape
make test        # Tests de base
make test_bonus  # Tests bonus (si disponibles)
```

### 3. Interprétation des résultats

#### ✅ Tous les tests passent
```
===================
Total: 33 tests
Passed: 33

✅ Tous les tests sont passés avec succès !
```
**→ Votre libft est correcte !**

#### ❌ Certains tests échouent
```
ft_strlen: [KO]
  Error: s="abc" ft_strlen=5 expected=3

===================
Total: 33 tests
Passed: 32
Failed: 1

❌ Certains tests ont échoué.
```
**→ Corrigez les fonctions indiquées**

#### 💥 Un test crash
```
ft_strchr: [CRASH: Signal 11 (SEGFAULT)]
```
**→ Votre fonction a un problème de pointeur/mémoire**

#### ⏱️ Un test timeout
```
ft_split: [TIMEOUT]
```
**→ Votre fonction a une boucle infinie ou est trop lente**

## 📋 Checklist de débogage

Quand un test échoue :

1. **Lisez le message d'erreur**
   ```
   Error: s="abc" ft_strlen=5 expected=3
   ```
   - `s="abc"` = l'entrée testée
   - `ft_strlen=5` = votre résultat
   - `expected=3` = résultat attendu

2. **Testez manuellement**
   ```c
   printf("%zu\n", ft_strlen("abc"));  // Devrait afficher 3
   ```

3. **Comparez avec la vraie fonction**
   ```c
   printf("ft: %zu, std: %zu\n", 
          ft_strlen("abc"), strlen("abc"));
   ```

4. **Vérifiez les cas limites**
   - Chaîne vide : `""`
   - NULL (si applicable)
   - Valeurs limites (INT_MAX, INT_MIN)

## 🔍 Types d'erreurs courantes

### SEGFAULT
**Causes possibles :**
- Accès à un pointeur NULL
- Dépassement de buffer
- Double free
- Utilisation après free

**Solution :** Utilisez `valgrind` pour plus de détails
```bash
valgrind --leak-check=full ./test_libft
```

### TIMEOUT
**Causes possibles :**
- Boucle infinie (`while(1)` sans break)
- Condition de sortie incorrecte
- Récursion infinie

**Solution :** Ajoutez des printf pour tracer l'exécution

### KO (logique incorrecte)
**Causes possibles :**
- Mauvaise compréhension du sujet
- Erreur de calcul
- Gestion incorrecte des cas limites

**Solution :** Relisez le sujet et comparez avec la fonction standard

## 💡 Astuces

### Tester une seule fonction
Modifiez le `main()` pour ne garder qu'un test :
```c
int main() {
    run_test_fork("ft_strlen", test_strlen);
    return 0;
}
```

### Augmenter le timeout
Pour les fonctions gourmandes :
```c
int timeout = 5; // Au lieu de 2
```

### Ajouter des tests personnalisés
```c
int test_edge_case() {
    char *result = ft_substr("", 0, 100);
    if (!result || strcmp(result, "") != 0) {
        printf("  Error: edge case failed\n");
        free(result);
        return 1;
    }
    free(result);
    return 0;
}

// Dans main
run_test_fork("ft_substr edge case", test_edge_case);
```

## 🎯 Objectif final

```
=== LIBFT TESTER ===

--- Part 1: Libc functions ---
ft_isalpha: [OK]
ft_isdigit: [OK]
ft_isalnum: [OK]
...
ft_calloc: [OK]

--- Part 2: Additional functions ---
ft_itoa: [OK]
ft_substr: [OK]
...
ft_strmapi: [OK]

===================
Total: 33 tests
Passed: 33

✅ Tous les tests sont passés avec succès !

=== LIBFT BONUS TESTER ===

--- Bonus: Linked list functions ---
ft_lstnew + ft_lstadd_front/back: [OK]
ft_lstsize + ft_lstlast: [OK]
ft_lstdelone: [OK]
ft_lstiter: [OK]
ft_lstmap: [OK]

===================
Total: 5 tests
Passed: 5

✅ Tous les tests bonus sont passés avec succès !
```

**🎉 Félicitations ! Votre libft est complète et correcte !**

## 📞 Support

Si vous trouvez un bug dans le tester ou avez des suggestions :
- Vérifiez que votre libft compile avec `-Wall -Wextra -Werror`
- Vérifiez que libft.h est accessible depuis test_libft/
- Vérifiez que libft.a existe et contient toutes les fonctions

Bonne chance ! 🍀
