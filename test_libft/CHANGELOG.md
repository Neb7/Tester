# Changelog - Libft Tester

## Version 2.1 - 2025-12-03

### 🎉 Améliorations majeures

#### ✨ Nouvelles fonctionnalités
- **Capture de sortie** : Vérification des fonctions ft_put* via pipes
- **Tests de protection NULL** : 29 tests pour vérifier la gestion des NULL
  - Affichage avec `[🛡]` (protégé) et `[⚠]` (non protégé)
  - Tests informatifs uniquement, n'affectent pas le résultat global
- **Tests groupés** : Tous les tests d'une fonction sur une seule ligne
- **Statistiques enrichies** : Ajout du compteur de protections NULL

#### 📊 Tests
- **52 tests fonctionnels** (Part 1 & 2)
- **7 tests bonus**
- **29 tests NULL** (13 pour Part 2, 16 pour bonus)
- **88 tests** au total

---

## Version 2.0 - 2025-12-02

### 🎉 Nouvelle version majeure avec système de fork

#### ✨ Nouvelles fonctionnalités
- **Système de fork** : Chaque test s'exécute dans un processus isolé
- **Protection contre les crashes** : SEGFAULT, BUS ERROR, ABORT détectés
- **Détection de timeout** : Boucles infinies détectées (2s par test)
- **Messages d'erreur détaillés** : Indication précise de ce qui a échoué
- **Résumé complet** : Statistiques finales (total, réussis, échoués)
- **Code retour significatif** : 0 = succès, 1 = échec

#### 🔧 Améliorations
- Refactorisation complète des fonctions de test
- Les tests retournent maintenant un code d'erreur (0/1)
- Meilleure lisibilité des résultats
- Codes couleur améliorés (vert, rouge, jaune)
- Makefile avec cibles pratiques (test, test_bonus)
- Script bash automatique (run_tests.sh)

#### 📚 Documentation
- README.md : Documentation principale
- QUICKSTART.md : Guide de démarrage rapide
- IMPROVEMENTS.md : Liste détaillée des améliorations
- CONTENTS.md : Description du contenu du dossier
- CHANGELOG.md : Historique des versions

#### 🐛 Corrections
- Élimination des doubles affichages
- Meilleure gestion de la mémoire dans les tests
- Correction des fuites mémoires potentielles dans les tests

#### 📊 Tests
- **23 tests** pour Part 1
- **29 tests** pour Part 2
- **7 tests** pour les bonus
- **59 tests** au total

### Détails techniques

#### Architecture
```
run_test_fork() ← Wrapper principal
    ├── fork() ← Création processus enfant
    ├── test_xxx() ← Exécution du test
    ├── waitpid() ← Attente résultat
    └── Analyse du code retour
```

#### Types de signaux détectés
- **SIGSEGV (11)** : Segmentation fault
- **SIGBUS (7)** : Bus error
- **SIGABRT (6)** : Abort signal

#### Timeout
- Configurable (défaut : 2 secondes)
- Vérification toutes les 100ms
- Kill automatique si dépassement

---

## Version 1.0 - Date antérieure

### 📝 Version originale
- Tests basiques pour libft
- Comparaison avec fonctions standard
- Messages [OK]/[KO] simples
- Pas de protection contre les crashes
- Pas de timeout
- Pas de fork

### Limitations
- ❌ Un segfault arrêtait tous les tests
- ❌ Boucles infinies bloquaient le tester
- ❌ Messages d'erreur peu détaillés
- ❌ Pas de résumé statistique
- ❌ Pas de code retour significatif

---

## Roadmap (futures versions)

### Version 2.1 (prévue)
- [ ] Support de tests paramétrables via arguments
- [ ] Export des résultats en JSON
- [ ] Mode verbose avec plus de détails
- [ ] Comparaison avec norminette

### Version 3.0 (idées)
- [ ] Interface web pour visualiser les résultats
- [ ] Intégration avec CI/CD (GitHub Actions)
- [ ] Tests de performance
- [ ] Support MacOS (pas uniquement Linux)
- [ ] Génération automatique de rapports HTML

---

## Migration depuis v1.0

### Changements incompatibles
- Les fonctions de test retournent maintenant `int` au lieu de `void`
- Les variables globales `all_tests_passed` remplacées par `failed_tests` et `total_tests`

### Comment migrer
Si vous aviez modifié l'ancienne version :

1. **Vos fonctions de test** :
```c
// Ancien format
void mon_test() {
    if (erreur) {
        printf("[KO]\n");
        return;
    }
    printf("[OK]\n");
}

// Nouveau format
int mon_test() {
    if (erreur) {
        printf("  Error: détails\n");
        return 1;  // Échec
    }
    return 0;  // Succès
}
```

2. **Appel des tests** :
```c
// Ancien format
mon_test();

// Nouveau format
run_test_fork("mon_test", mon_test);
```

---

## Contributions

### Mainteneur
- benpicar (@Neb7)

### Remerciements
- La communauté 42 pour les retours
- Tous les testeurs de la version 1.0

### Comment contribuer
1. Fork le projet
2. Créez une branche pour votre fonctionnalité
3. Committez vos changements
4. Poussez vers la branche
5. Ouvrez une Pull Request

---

## Licence

Ce tester est fourni tel quel pour aider les étudiants de 42.
Libre d'utilisation, de modification et de partage.

---

**Note** : Ce changelog sera mis à jour avec chaque nouvelle version.
