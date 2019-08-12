# ck-gui

Simple draft of UI for [ck](https://github.com/ctuning/ck).

## What does it do?

This is just a thin wrapper for `ck` command line interface representing several typical scenarios as windows.

Currently there are several search scenarios:
- **Tool > Repos** - shows list of all repositoies and some of their properties.
- **Tool > Envs** - wrapper for `ck search env` command, shows list of found envs and some of their properties. Allows refreshing and deletion env via context menu.
- **Tool > Packages** - wrapper for `ck search package` command, shows list of found packages and some of their properties.
- **Tool > Programs** - wrapper for `ck search program` command, shows list of found programs and some of their properties.

For most of entities commands for displaying/editing meta (and script for env) are also available (but be careful about the [issue](https://github.com/Chunosov/ck-gui/issues/1) with scintilla editor).

![Common screen](program/ck-gui-qt/img/screenshots/screen-000.png) 

## Prebuilds

### Linux

Prebuilt version is available for Linux as self executable AppImage package [here](https://drive.google.com/file/d/1MeOvExDNWtA9KjqZtvB3TuspdLtI0XKz/view?usp=sharing).

Just download it, mark as excutable and run. It should be functional by defaul when `ck` is installed via `pip` and it is in `PATH`. The appllication will only once ask you where the root directory of ck-repositories is.

Currentlry it was only built and tested under Ubuntu 16 LTS.


## Build from sources

You can also built the application from sources by yourself.

### Build Requirements

#### Qt
Install [Qt](qt.io) framework.

#### QScintilla
Download [source code](https://www.riverbankcomputing.com/software/qscintilla/download), build library and install it according to included [instructions](http://pyqt.sourceforge.net/Docs/QScintilla2). 

Example for Ubuntu:
```bash
wget https://netix.dl.sourceforge.net/project/pyqt/QScintilla2/QScintilla-2.10.4/QScintilla_gpl-2.10.4.tar.gz
tar -zxvf QScintilla_gpl-2.10.4.tar.gz
cd QScintilla_gpl-2.10.4/Qt4Qt5
# If you have multiple versions of Qt installed 
# then make sure you use the correct version of qmake:
export PATH=/home/nikolay/Qt/5.10.0/gcc_64/bin/:$PATH
qmake
make
make install
```

### Build
```bash
git clone https://github.com/Chunosov/ck-gui
cd ck-gui
git submodule init
git submodule update
```

#### Linux
```bash
chmod +x ./scripts/make_release_linux.sh
./script/make_release_linux.sh -b
```
Note the script `make_release_linux.sh` makes full rebuild and it can take a while.
