#!/bin/bash

# Script de démonstration du tester libft

echo "╔═══════════════════════════════════════╗"
echo "║     LIBFT TESTER - Démonstration     ║"
echo "╔═══════════════════════════════════════╗"
echo ""

# Vérifier si libft existe
if [ ! -d "../" ]; then
    echo "❌ Erreur: Le dossier parent (libft) n'existe pas"
    exit 1
fi

# Vérifier si libft.h existe
if [ ! -f "../libft.h" ]; then
    echo "❌ Erreur: libft.h n'existe pas dans le dossier parent"
    exit 1
fi

echo "📚 Compilation de libft..."
make -C .. > /dev/null 2>&1

if [ $? -ne 0 ]; then
    echo "❌ Erreur lors de la compilation de libft"
    exit 1
fi

echo "✅ Libft compilée avec succès"
echo ""

echo "🔨 Compilation du tester..."
make > /dev/null 2>&1

if [ $? -ne 0 ]; then
    echo "❌ Erreur lors de la compilation du tester"
    exit 1
fi

echo "✅ Tester compilé avec succès"
echo ""

echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
echo "🧪 LANCEMENT DES TESTS DE BASE"
echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
echo ""

./test_libft
TEST_RESULT=$?

echo ""
echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"

# Vérifier si les bonus existent
if grep -q "t_list" ../libft.h 2>/dev/null; then
    echo "🧪 LANCEMENT DES TESTS BONUS"
    echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
    echo ""
    
    make bonus > /dev/null 2>&1
    if [ $? -eq 0 ]; then
        ./test_libft_bonus
        BONUS_RESULT=$?
    else
        echo "⚠️  Tests bonus non disponibles"
        BONUS_RESULT=0
    fi
else
    echo "ℹ️  Pas de partie bonus détectée"
    BONUS_RESULT=0
fi

echo ""
echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
echo ""

if [ $TEST_RESULT -eq 0 ] && [ $BONUS_RESULT -eq 0 ]; then
    echo "🎉 TOUS LES TESTS ONT RÉUSSI ! 🎉"
    exit 0
else
    echo "⚠️  CERTAINS TESTS ONT ÉCHOUÉ"
    exit 1
fi
