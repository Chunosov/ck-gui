#! /bin/bash

FMT_BOLD="\e[1m"
FMT_RESET="\e[0m"

echo ""
echo -e "${FMT_BOLD}Build ${CK_PROG_TARGET_EXE} ...${FMT_RESET}"
echo "Use qmake: $QMAKE"
echo ""

export ORION_QT_PATH=${CK_ENV_LIB_ORION_QT}
export QSCINTILLA_PATH=${CK_ENV_LIB_QSCINTILLA}

${CK_ENV_LIB_QT_QMAKE_EXE} ..

make CC=${CK_CC} CXX=${CK_CXX}

cp ../bin/${CK_PROG_TARGET_EXE} ${CK_PROG_TARGET_EXE}
