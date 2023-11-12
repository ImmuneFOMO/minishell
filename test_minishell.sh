#!/bin/bash

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[0;33m'
BLUE='\033[0;34m'
MAGENTA='\033[0;35m'
CYAN='\033[0;36m'
NC='\033[0m'

COLORS=($RED $GREEN $YELLOW $BLUE $MAGENTA $CYAN)

TEST_OUTPUT="test_output_$(date +%Y%m%d_%H%M%S).txt"

function test_command() {
    command=$1
    COLOR=${COLORS[$RANDOM % ${#COLORS[@]}]}
    echo -e "${COLOR}Testing command: $command${NC}" | tee -a "$TEST_OUTPUT"
    echo "$command" | valgrind --leak-check=full --show-leak-kinds=all --suppressions=readline.supp ./minishell 2>&1 | tee -a "$TEST_OUTPUT"
    echo -e "${COLOR}------------------------------------------------------${NC}" | tee -a "$TEST_OUTPUT"
}

make re
make clean
clear

test_command "ls"
test_command "ls | cat"
test_command "wait 1"
test_command "ls < fdsfdsfdsf"
test_command "echo test > test"
test_command "rm test"
test_command "echo test"
test_command "echo -n test"
test_command "pwd"
test_command "env"
test_command "export MY_VARIABLE_TWO=\"two\" MY_VARIABLE=\"two\""
test_command "env"
test_command "unset MY_VARIABLE_TWO MY_VARIABLE"
test_command "env"
test_command "export test=\"test2\" | env"
test_command "env"
test_command "unset test"

test