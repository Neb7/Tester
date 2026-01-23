# Get Next Line Tester

Testeur complet pour le projet **get_next_line** de 42.

## Description

Ce tester vérifie automatiquement votre implémentation de `get_next_line` avec différents scénarios :
- ✅ Lecture de fichiers avec des lignes de différentes tailles
- ✅ Fichiers vides
- ✅ Fichiers sans retour à la ligne final
- ✅ Fichiers avec des lignes très longues
- ✅ Fichiers avec des lignes vides
- ✅ Protection contre les crashs (segfault, timeout)

## Fonctionnalités

🔒 **Protection par fork** : Chaque test s'exécute dans un processus séparé pour éviter qu'un crash n'arrête la suite des tests.

⏱️ **Timeout** : Les tests bloqués (boucle infinie) sont automatiquement interrompus après 2 secondes.

📊 **Résultats détaillés** : Affichage clair des tests réussis et échoués avec des couleurs.

🎯 **Tests complets** : Couvre les cas simples, les cas limites et les situations d'erreur.

## Structure

```
GNL/
├── tester_gnl.c        # Le tester principal
├── run_all.c           # Programme pour exécuter tous les tests
├── Makefile            # Makefile pour compiler et exécuter
├── asset/              # Fichiers de test
│   ├── empty.txt
│   ├── empty_lines.txt
│   ├── long_line.txt
│   ├── no_nl_final.txt
│   ├── single_no_nl.txt
│   ├── single_with_nl.txt
│   ├── stress.txt
│   └── test1.txt
└── README.md           # Ce fichier
```

## Utilisation

### Prérequis

Votre implémentation de `get_next_line` doit être accessible depuis ce dossier.

### Compilation

```bash
make
```

### Exécution

```bash
# Lancer tous les tests
./tester_gnl

# Ou utiliser le programme de test complet
./run_all

# Nettoyer les fichiers compilés
make clean
make fclean

# Recompiler
make re
```

## Tests inclus

Le tester vérifie :
- Lecture ligne par ligne correcte
- Gestion des lignes vides
- Gestion des fichiers vides
- Gestion des fichiers sans '\n' final
- Gestion des très longues lignes
- Gestion des multiples lectures
- Comportement avec différents BUFFER_SIZE

## Format de sortie

Les tests affichent :
- `[OK]` en vert si le test passe
- `[KO]` en rouge si le test échoue
- `[CRASH]` en rouge si un segfault/bus error se produit
- `[TIMEOUT]` en jaune si le test dépasse 2 secondes

## Notes

⚠️ Assurez-vous que votre `get_next_line.h` est bien inclus et que tous les fichiers nécessaires sont présents.

📝 Certains tests peuvent nécessiter un BUFFER_SIZE spécifique. Testez avec différentes valeurs (1, 10, 42, 1024, etc.).

---

*Ce tester a été créé dans le cadre du cursus 42.*
