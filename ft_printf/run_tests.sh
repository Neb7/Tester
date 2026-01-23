#!/bin/bash

# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    run_tests.sh                                       :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: benpicar <benpicar@student.42mulhouse.fr>  +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/01/23 10:00:00 by benpicar          #+#    #+#              #
#    Updated: 2026/01/23 10:00:00 by benpicar         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Couleurs
GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[0;33m'
BLUE='\033[0;34m'
CYAN='\033[0;36m'
NC='\033[0m'

# Variables
TESTER="./tester_printf"
USE_VALGRIND=0

# Fonction d'aide
show_help() {
    echo -e "${BLUE}╔════════════════════════════════════════════════════════════════════════╗${NC}"
    echo -e "${BLUE}║                    FT_PRINTF TESTER - RUN SCRIPT                       ║${NC}"
    echo -e "${BLUE}╠════════════════════════════════════════════════════════════════════════╣${NC}"
    echo -e "${BLUE}║${NC} Usage: ./run_tests.sh [option]                                    ${BLUE}║${NC}"
    echo -e "${BLUE}╠════════════════════════════════════════════════════════════════════════╣${NC}"
    echo -e "${BLUE}║${NC} Options:                                                           ${BLUE}║${NC}"
    echo -e "${BLUE}║${NC}   ${GREEN}(no option)${NC}     - Run tests normally                           ${BLUE}║${NC}"
    echo -e "${BLUE}║${NC}   ${GREEN}valgrind${NC}        - Run tests with valgrind memory check        ${BLUE}║${NC}"
    echo -e "${BLUE}║${NC}   ${GREEN}clean${NC}           - Clean compiled files                         ${BLUE}║${NC}"
    echo -e "${BLUE}║${NC}   ${GREEN}fclean${NC}          - Deep clean (remove all binaries)            ${BLUE}║${NC}"
    echo -e "${BLUE}║${NC}   ${GREEN}re${NC}              - Recompile everything                         ${BLUE}║${NC}"
    echo -e "${BLUE}║${NC}   ${GREEN}help${NC}            - Show this help message                       ${BLUE}║${NC}"
    echo -e "${BLUE}╚════════════════════════════════════════════════════════════════════════╝${NC}"
}

# Fonction de nettoyage
clean_files() {
    echo -e "${YELLOW}Cleaning...${NC}"
    make clean
    echo -e "${GREEN}✓ Cleaned!${NC}"
}

# Fonction de nettoyage complet
fclean_files() {
    echo -e "${YELLOW}Deep cleaning...${NC}"
    make fclean
    echo -e "${GREEN}✓ Deep cleaned!${NC}"
}

# Fonction de recompilation
recompile() {
    echo -e "${YELLOW}Recompiling...${NC}"
    make re
    if [ $? -eq 0 ]; then
        echo -e "${GREEN}✓ Recompilation successful!${NC}"
        return 0
    else
        echo -e "${RED}✗ Recompilation failed!${NC}"
        return 1
    fi
}

# Parse arguments
if [ "$1" = "help" ] || [ "$1" = "-h" ] || [ "$1" = "--help" ]; then
    show_help
    exit 0
elif [ "$1" = "clean" ]; then
    clean_files
    exit 0
elif [ "$1" = "fclean" ]; then
    fclean_files
    exit 0
elif [ "$1" = "re" ]; then
    recompile
    exit $?
elif [ "$1" = "valgrind" ]; then
    USE_VALGRIND=1
fi

# Bannière
echo -e "${CYAN}╔════════════════════════════════════════════════════════════════════════╗${NC}"
echo -e "${CYAN}║                     FT_PRINTF TESTER LAUNCHER                          ║${NC}"
echo -e "${CYAN}╚════════════════════════════════════════════════════════════════════════╝${NC}"
echo ""

# Vérification des fichiers nécessaires
echo -e "${BLUE}[1/4]${NC} Checking required files..."

if [ ! -f "tester_printf.c" ]; then
    echo -e "${RED}✗ Error: tester_printf.c not found!${NC}"
    exit 1
fi

if [ ! -f "Makefile" ]; then
    echo -e "${RED}✗ Error: Makefile not found!${NC}"
    exit 1
fi

echo -e "${GREEN}✓ All required files found!${NC}"
echo ""

# Compilation
echo -e "${BLUE}[2/4]${NC} Compiling tester..."
make > /dev/null 2>&1

if [ $? -ne 0 ]; then
    echo -e "${RED}✗ Compilation failed!${NC}"
    echo -e "${YELLOW}Running make with full output:${NC}"
    make
    exit 1
fi

if [ ! -f "$TESTER" ]; then
    echo -e "${RED}✗ Error: tester_printf executable not found after compilation!${NC}"
    exit 1
fi

echo -e "${GREEN}✓ Compilation successful!${NC}"
echo ""

# Vérification de valgrind si nécessaire
if [ $USE_VALGRIND -eq 1 ]; then
    echo -e "${BLUE}[3/4]${NC} Checking valgrind..."
    
    if ! command -v valgrind &> /dev/null; then
        echo -e "${RED}✗ valgrind not found!${NC}"
        echo -e "${YELLOW}Install it with: sudo apt-get install valgrind${NC}"
        exit 1
    fi
    
    echo -e "${GREEN}✓ valgrind found!${NC}"
    echo ""
else
    echo -e "${BLUE}[3/4]${NC} Valgrind check skipped (not requested)"
    echo ""
fi

# Exécution des tests
echo -e "${BLUE}[4/4]${NC} Running tests..."
echo ""
echo -e "${CYAN}════════════════════════════════════════════════════════════════════════${NC}"
echo ""

if [ $USE_VALGRIND -eq 1 ]; then
    echo -e "${YELLOW}Running with valgrind (this may take a while)...${NC}"
    echo ""
    valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --log-file=valgrind.log $TESTER
    RESULT=$?
    
    echo ""
    echo -e "${CYAN}════════════════════════════════════════════════════════════════════════${NC}"
    echo ""
    
    # Afficher le résumé de valgrind
    if [ -f "valgrind.log" ]; then
        echo -e "${YELLOW}Valgrind Summary:${NC}"
        grep "ERROR SUMMARY" valgrind.log
        grep "definitely lost" valgrind.log
        grep "indirectly lost" valgrind.log
        grep "possibly lost" valgrind.log
        echo ""
        echo -e "${BLUE}Full valgrind log saved in: valgrind.log${NC}"
    fi
else
    $TESTER
    RESULT=$?
    
    echo ""
    echo -e "${CYAN}════════════════════════════════════════════════════════════════════════${NC}"
    echo ""
fi

# Résultat final
if [ $RESULT -eq 0 ]; then
    echo -e "${GREEN}╔════════════════════════════════════════════════════════════════════════╗${NC}"
    echo -e "${GREEN}║                     ✓ ALL TESTS PASSED! ✓                              ║${NC}"
    echo -e "${GREEN}╚════════════════════════════════════════════════════════════════════════╝${NC}"
    exit 0
else
    echo -e "${RED}╔════════════════════════════════════════════════════════════════════════╗${NC}"
    echo -e "${RED}║                     ✗ SOME TESTS FAILED ✗                              ║${NC}"
    echo -e "${RED}╚════════════════════════════════════════════════════════════════════════╝${NC}"
    echo ""
    echo -e "${YELLOW}Tips for debugging:${NC}"
    echo -e "  • Check the output above for specific failed tests"
    echo -e "  • Run with valgrind: ${GREEN}./run_tests.sh valgrind${NC}"
    echo -e "  • Add debug printf() in your ft_printf implementation"
    echo -e "  • Compare your output with the original printf()"
    exit 1
fi
