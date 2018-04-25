# ck-gui

Simple draft of UI for [ck](https://github.com/ctuning/ck).

## Build Requirements

### Qt
Install [Qt](qt.io) framework.

### QScintilla
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

