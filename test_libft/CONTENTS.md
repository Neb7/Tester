# 📦 Contenu du dossier test_libft

## 📄 Fichiers

### 🔧 Fichiers de test
| Fichier | Taille | Description |
|---------|--------|-------------|
| `test_libft.c` | 16K | Tester principal (Part 1 & 2) avec système de fork |
| `test_libft_bonus.c` | 7.3K | Tester pour les fonctions bonus (listes) avec fork |

### 📚 Documentation
| Fichier | Taille | Description |
|---------|--------|-------------|
| `README.md` | 2.2K | Documentation principale - Fonctionnalités et utilisation |
| `QUICKSTART.md` | 3.9K | Guide de démarrage rapide avec exemples |
| `IMPROVEMENTS.md` | 4.4K | Liste détaillée des améliorations apportées |

### 🛠️ Outils
| Fichier | Taille | Description |
|---------|--------|-------------|
| `Makefile` | 2.0K | Compilation automatique des tests |
| `run_tests.sh` | 2.4K | Script bash pour lancer tous les tests automatiquement |

## 🎯 Fichiers à lire en priorité

1. **Pour commencer rapidement** : `QUICKSTART.md`
2. **Pour comprendre les fonctionnalités** : `README.md`
3. **Pour voir ce qui a changé** : `IMPROVEMENTS.md`

## 📊 Statistiques

- **Total de lignes de code** : ~1200 lignes (test_libft.c + test_libft_bonus.c)
- **Nombre de tests** : 
  - Part 1 & 2 : 33 tests
  - Bonus : 5 tests
  - **Total : 38 tests**
- **Fonctionnalités** :
  - ✅ Protection contre segfault
  - ✅ Détection de timeout
  - ✅ Messages d'erreur détaillés
  - ✅ Isolation par fork
  - ✅ Résumé complet

## 🚀 Utilisation rapide

```bash
# Méthode 1 : Tout automatique
./run_tests.sh

# Méthode 2 : Avec Make
make test          # Tests de base
make test_bonus    # Tests bonus

# Méthode 3 : Manuel
make               # Compile
./test_libft       # Lance les tests de base
./test_libft_bonus # Lance les tests bonus
```

## 🔄 Dépendances

Le tester nécessite :
- Un dossier parent contenant votre libft
- `libft.h` dans le dossier parent
- `libft.a` compilé (créé automatiquement par le Makefile)
- gcc avec support de `-Wall -Wextra -Werror`
- Linux (pour fork, waitpid, signal)

## 📁 Structure d'installation recommandée

```
votre_libft/
├── ft_isalpha.c
├── ft_strlen.c
├── ... (vos fichiers .c)
├── libft.h
├── Makefile
└── test_libft/           ← Ce dossier
    ├── test_libft.c
    ├── test_libft_bonus.c
    ├── Makefile
    ├── run_tests.sh
    ├── README.md
    ├── QUICKSTART.md
    └── IMPROVEMENTS.md
```

## 🎨 Couleurs dans le terminal

Le tester utilise des codes ANSI pour les couleurs :
- 🟢 **Vert** : Tests réussis `[OK]`
- 🔴 **Rouge** : Tests échoués `[KO]`, crashes `[CRASH]`
- 🟡 **Jaune** : Timeout `[TIMEOUT]`

## 🔒 Sécurité

Chaque test s'exécute dans un processus isolé (fork) :
- Un crash dans un test n'affecte pas les autres
- Protection contre les boucles infinies (timeout 2s)
- Détection précise du type d'erreur (SEGFAULT, BUS ERROR, ABORT)

## 📝 Notes importantes

1. Le tester compile votre libft automatiquement
2. Les tests s'arrêtent à la première erreur dans chaque fonction
3. Le code retour du programme : 0 = succès, 1 = échec
4. Compatible uniquement Linux (utilise des appels système POSIX)

## 🎓 Niveau de difficulté

- **Débutant** : Utilisez `./run_tests.sh` et suivez les messages
- **Intermédiaire** : Utilisez `make t` et comprenez les erreurs
- **Avancé** : Modifiez les tests pour ajouter vos propres cas

## 📮 Feedback

Si vous trouvez ce tester utile :
- ⭐ Partagez-le avec vos camarades
- 🐛 Signalez les bugs si vous en trouvez
- 💡 Proposez des améliorations

Bonne chance pour votre libft ! 🚀
