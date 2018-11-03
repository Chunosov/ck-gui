#! /bin/bash

cd ${INSTALL_DIR}/QScintilla_gpl-2.10.4/Qt4Qt5
${CK_ENV_LIB_QT_QMAKE_EXE}
make CC=${CK_CC} CXX=${CK_CXX}
