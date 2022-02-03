#!/bin/bash

YELLOW="\033[1;33m"
COLOR_OFF="\033[0m"

function tRet {
    if [ "$?" -ne "0" ]; then
        echo -ne "${YELLOW}ERROR${COLOR_OFF}\n" 
        exit 1;
    fi
}

function try_install() {
    dpkg -l "$1" | grep -q ^ii && return 1
    apt -y install "$@"
    return 0
}

find -name *.o -exec rm -rf {} \;
APPLI=test
REPORT=covreport
echo -e "${YELLOW}*** ${APPLI} ***${COLOR_OFF}\n"
find -name ${APPLI} -exec rm -rf {} \;
gcc -v -o ${APPLI} main.c useful.c -Werror
./${APPLI}
################################################
# coverage
###############################################
try_install firefox
try_install lcov
find -name *.gcno -exec rm -rf {} \;
find -name *.gcda -exec rm -rf {} \;
gcc -v -o ${APPLI} main.c useful.c -O0 --coverage -lgcov  -fprofile-arcs -ftest-coverage
./${APPLI}
gcov -o *.gcno *.c
mkdir -p ${REPORT}
lcov --directory . -c -o rapport.info ; tRet
genhtml -o ${REPORT} -t "coverage ${APPLI}" rapport.info ; tRet
/usr/bin/firefox ./${REPORT}/index.html
################################################
# profiling
###############################################
gcc -pg -ggdb3 -O0 -std=c99 -Wall -Wextra -pedantic -o ${APPLI}.out main.c useful.c
./${APPLI}.out 10000
try_install graphviz
python3 -m pip install --user gprof2dot
gprof ${APPLI}.out > ${APPLI}.gprof
gprof2dot < ${APPLI}.gprof | dot -Tsvg -o output.svg
gprof -b ${APPLI}.out
/usr/bin/firefox output.svg
################################################
# valgrind
###############################################
#try_install valgrind
#try_install kcachegrind
#valgrind --tool=callgrind --dump-instr=yes --simulate-cache=yes --collect-jumps=yes --collect-atstart=no --instr-atstart=no ${APPLI}  
#kcachegrind callgrind.out.*
################################################
# Address Sanitizer
###############################################
#try_install libasan

