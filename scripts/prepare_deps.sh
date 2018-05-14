#! /bin/bash

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
cd ${SCRIPT_DIR}/..

if [ ! -d tmp ]; then mkdir tmp; fi
cd tmp

function build_scintilla() {
  wget https://netix.dl.sourceforge.net/project/pyqt/QScintilla2/QScintilla-2.10.4/QScintilla_gpl-2.10.4.tar.gz
  tar -zxvf QScintilla_gpl-2.10.4.tar.gz
  cd QScintilla_gpl-2.10.4/Qt4Qt5
  qmake
  make
  make install
}

build_scintilla
