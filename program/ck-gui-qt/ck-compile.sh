#! /bin/bash

QMAKE="${CK_ENV_LIB_QT_QMAKE_EXE}"

FMT_BOLD="\e[1m"
FMT_RESET="\e[0m"

echo ""
echo -e "${FMT_BOLD}Build ${CK_PROG_TARGET_EXE} ...${FMT_RESET}"
echo "Use qmake: $QMAKE"
echo ""

echo "Check for QScintilla ..."
if [ "$(ls ${CK_ENV_LIB_QT_LIB}/libqscintilla2_* 2> /dev/null | wc -l)" != "0" ]; then
  echo "OK. Found in ${CK_ENV_LIB_QT_LIB}"
else
  echo "Not found in ${CK_ENV_LIB_QT_LIB}"
  echo "Installing ..."
  wget https://netix.dl.sourceforge.net/project/pyqt/QScintilla2/QScintilla-2.10.4/QScintilla_gpl-2.10.4.tar.gz
  tar -zxvf QScintilla_gpl-2.10.4.tar.gz
  cd QScintilla_gpl-2.10.4/Qt4Qt5
  $QMAKE
  make CC=${CK_CC} CXX=${CK_CXX}
  make install
  cd ../..
fi 

export ORION_QT_PATH=${CK_ENV_LIB_ORION_QT}

$QMAKE ..

make CC=${CK_CC} CXX=${CK_CXX}

cp ../bin/${CK_PROG_TARGET_EXE} ${CK_PROG_TARGET_EXE}
