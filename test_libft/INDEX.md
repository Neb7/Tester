# 📑 Index - Libft Tester v2.0

## 🚀 Démarrage rapide

**Vous êtes pressé ?** → Lisez `QUICKSTART.md`

**Première utilisation ?** → Exécutez `./run_tests.sh`

## 📚 Documentation par objectif

### Je veux...

#### ...commencer immédiatement
1. Lisez [`QUICKSTART.md`](QUICKSTART.md)
2. Exécutez `./run_tests.sh`
3. Suivez les instructions à l'écran

#### ...comprendre les fonctionnalités
- Lisez [`README.md`](README.md) - Vue d'ensemble
- Lisez [`IMPROVEMENTS.md`](IMPROVEMENTS.md) - Détails des améliorations

#### ...comprendre ce qui a changé
- Lisez [`CHANGELOG.md`](CHANGELOG.md) - Historique des versions
- Section "Version 2.0" pour les changements récents

#### ...savoir ce que contient ce dossier
- Lisez [`CONTENTS.md`](CONTENTS.md) - Inventaire complet

#### ...déboguer mes tests
- Consultez "Checklist de débogage" dans [`QUICKSTART.md`](QUICKSTART.md)
- Consultez "Types d'erreurs courantes" dans [`QUICKSTART.md`](QUICKSTART.md)

#### ...personnaliser les tests
- Modifiez `test_libft.c` ou `test_libft_bonus.c`
- Consultez "Ajouter vos propres tests" dans [`QUICKSTART.md`](QUICKSTART.md)

#### ...compiler manuellement
```bash
make          # Compile le tester
make test     # Compile et lance les tests de base
make bonus    # Compile le tester bonus
make test_bonus  # Compile et lance les tests bonus
make clean    # Nettoie les binaires
```

## 📖 Guide de lecture par niveau

### 🟢 Débutant
1. `QUICKSTART.md` - Démarrage rapide
2. `README.md` - Fonctionnalités principales
3. Exécuter `./run_tests.sh`

### 🟡 Intermédiaire  
1. `IMPROVEMENTS.md` - Comprendre les améliorations
2. `CONTENTS.md` - Structure du projet
3. Modifier quelques tests dans `test_libft.c`

### 🔴 Avancé
1. `CHANGELOG.md` - Historique complet
2. Étudier `test_libft.c` et `test_libft_bonus.c`
3. Comprendre le système de fork
4. Ajouter vos propres tests personnalisés

## 🗂️ Carte du contenu

```
test_libft/
│
├── 📄 Fichiers principaux
│   ├── test_libft.c          → Tests Part 1 & 2 (569 lignes)
│   ├── test_libft_bonus.c    → Tests bonus (239 lignes)
│   ├── Makefile              → Compilation automatique
│   └── run_tests.sh          → Script tout-en-un
│
├── 📚 Documentation utilisateur
│   ├── README.md             → Vue d'ensemble (73 lignes)
│   ├── QUICKSTART.md         → Démarrage rapide (237 lignes)
│   └── INDEX.md              → Ce fichier (vous êtes ici)
│
├── 📊 Documentation technique
│   ├── IMPROVEMENTS.md       → Détails techniques (259 lignes)
│   ├── CHANGELOG.md          → Historique (186 lignes)
│   └── CONTENTS.md           → Inventaire (144 lignes)
│
└── 📈 Statistiques
    ├── 9 fichiers au total
    ├── 808 lignes de code C
    ├── 756 lignes de documentation
    └── 60K taille totale
```

## 🎯 Objectifs par fichier

| Fichier | Objectif | Temps de lecture |
|---------|----------|------------------|
| `QUICKSTART.md` | Démarrer en 5 min | ⏱️ 5 min |
| `README.md` | Comprendre les fonctionnalités | ⏱️ 3 min |
| `IMPROVEMENTS.md` | Détails techniques | ⏱️ 10 min |
| `CHANGELOG.md` | Voir l'évolution | ⏱️ 5 min |
| `CONTENTS.md` | Explorer le contenu | ⏱️ 3 min |
| `INDEX.md` | Navigation | ⏱️ 2 min |

## 🔍 Recherche rapide

### Par mot-clé

#### Fork
- `README.md` - Section "Fonctionnalités"
- `IMPROVEMENTS.md` - Section "Système de Fork"
- `test_libft.c` - Fonction `run_test_fork()`

#### Segfault
- `QUICKSTART.md` - Section "SEGFAULT"
- `IMPROVEMENTS.md` - Section "Types de résultats"
- `README.md` - "Protection par Fork"

#### Timeout
- `QUICKSTART.md` - Section "TIMEOUT"
- `test_libft.c` - Variable `timeout` ligne ~57
- `IMPROVEMENTS.md` - Section "Timeout automatique"

#### Erreurs
- `QUICKSTART.md` - "Checklist de débogage"
- `IMPROVEMENTS.md` - "Messages d'erreur détaillés"

#### Compilation
- `Makefile` - Toutes les règles
- `QUICKSTART.md` - Section "Compilation"
- `run_tests.sh` - Script automatique

## 🎨 Légende des icônes

- 📄 Fichier de code
- 📚 Documentation utilisateur
- 📊 Documentation technique
- 🚀 Démarrage rapide
- 🔧 Configuration
- 🐛 Débogage
- ✅ Réussite
- ❌ Échec
- ⚠️ Avertissement
- 💡 Astuce
- 🎯 Objectif
- 📈 Statistique

## ⚡ Commandes rapides

```bash
# Lancer tous les tests
./run_tests.sh

# Tests de base uniquement
make test

# Tests bonus uniquement
make test_bonus

# Recompiler tout
make re

# Nettoyer
make clean

# Voir l'aide
cat README.md

# Démarrage rapide
cat QUICKSTART.md
```

## 🆘 En cas de problème

1. **Le tester ne compile pas**
   → Vérifiez que libft.h existe dans le dossier parent
   → Vérifiez que votre Makefile libft fonctionne

2. **Tous les tests échouent**
   → Vérifiez que libft.a contient toutes les fonctions
   → Compilez avec `nm ../libft.a` pour voir les symboles

3. **Un test spécifique échoue**
   → Consultez "Checklist de débogage" dans QUICKSTART.md
   → Regardez le message d'erreur détaillé

4. **Le tester crash**
   → Vérifiez que vous êtes sur Linux
   → Vérifiez les permissions (`chmod +x run_tests.sh`)

## 📞 Support

- 💬 Posez des questions à vos camarades
- 📖 Relisez la documentation
- 🔍 Cherchez dans l'index (ce fichier)
- 🐛 Signalez les bugs si vous en trouvez

## 🎓 Ressources additionnelles

- `man fork` - Documentation du système fork
- `man waitpid` - Documentation de waitpid
- `man signal` - Documentation des signaux
- Documentation officielle de libft (sujet)

---

**Version du tester** : 2.0  
**Dernière mise à jour** : 2025-12-02  
**Mainteneur** : benpicar (@Neb7)

Bonne chance pour votre libft ! 🍀
