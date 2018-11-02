#! /bin/bash

function exit_if_error() {
 if [ "${?}" != "0" ]; then exit 1; fi
}

function remove_dir_if_exists() {
  if [ -d $1 ]; then rm -rdf $1; fi
}

#-----------------------------------------------------------------------

function usage() {
  echo
  echo "Use options:"
  echo "-b Rebuild project in release mode"
  echo "-a Make standalone AppImage package"
  exit 1
}

while getopts "ba" opt_name; do
  case "$opt_name" in
    b) DO="true" DO_BUILD="true";;
    a) DO="true" DO_APPIMAGE="true";;
    *) usage;;
  esac
done

if [[ -z "${DO}" ]]; then
  echo "Nothing to be done."
  usage
fi

#-----------------------------------------------------------------------

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
cd ${SCRIPT_DIR}/..

# Create out dir if none
if [ ! -d out ]; then
  mkdir out
fi
cd out

#-----------------------------------------------------------------------

if [[ "${DO_BUILD}" == "true" ]]; then
  echo
  echo "Building release..."

  # Check if qmake in path
  QMAKE_VER="$(qmake -v)"
  if [[ -z "${QMAKE_VER}" ]]; then
    echo
    echo "ERROR: qmake is not found in PATH."
    echo "Find Qt installation and update your PATH like:"
    echo 'PATH=/home/user/Qt/5.10.0/gcc_64/bin:$PATH'
    echo 'and run this script again.'
    exit
  else
    echo
    echo "${QMAKE_VER}"
  fi

  # Build release
  echo
  echo "Building..."
  remove_dir_if_exists BuildDir
  mkdir BuildDir
  cd BuildDir
  qmake -config release ../../ck-gui.pro
  make
  exit_if_error

  echo
  echo "Building done"

  cd ..
fi

#-----------------------------------------------------------------------

if [[ "${DO_APPIMAGE}" == "true" ]]; then
  echo
  echo "Making AppImage..."

  # Download linuxdeplyqt if none
  # https://github.com/probonopd/linuxdeployqt
  LINUXDEPLOYQT=linuxdeployqt-continuous-x86_64.AppImage
  if [ ! -f ${LINUXDEPLOYQT} ]; then
    echo
    echo "Downloading ${LINUXDEPLOYQT}..."
    wget -c https://github.com/probonopd/linuxdeployqt/releases/download/continuous/${LINUXDEPLOYQT}
    chmod a+x ${LINUXDEPLOYQT}
    exit_if_error
  fi

  # Create AppDir structure
  echo
  echo "Creating AppDir structure..."
  remove_dir_if_exists AppDir
  mkdir -p AppDir/usr/bin
  mkdir -p AppDir/usr/lib
  mkdir -p AppDir/usr/share/applications
  mkdir -p AppDir/usr/share/icons/hicolor/256x256/apps
  cp ../bin/ck-gui AppDir/usr/bin
  cp ../scripts/ck-gui.desktop AppDir/usr/share/applications
  cp ../img/main-icon.svg AppDir/usr/share/icons/hicolor/256x256/apps/ck-gui.svg
  exit_if_error

  # Run linuxdeplyqt on the AppDir
  echo
  echo "Creating AppImage..."
  # For some reason the tool forgets about these plugins:
  SVG_PLUGINS=iconengines/libqsvgicon.so,imageformats/libqsvg.so
  ./${LINUXDEPLOYQT} AppDir/usr/share/applications/ck-gui.desktop -appimage -no-translations -extra-plugins=${SVG_PLUGINS}
  exit_if_error

  echo
  echo "AppImage created"
fi
