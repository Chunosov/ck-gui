#ifndef SEARCHPROGRAMSWINDOW_H
#define SEARCHPROGRAMSWINDOW_H

#include "searchwindowbase.h"

class SearchProgramsWindow : public SearchWindowBase
{
    Q_OBJECT

public:
    explicit SearchProgramsWindow(QWidget *parent = nullptr);

protected:
    void findAll();
    void findByTags() override;
    void resultSelected(const QString& uid) override;

private:
    void populate(const QString& tags);
    void showInfo(const QString& uid);
};

#endif // SEARCHPROGRAMSWINDOW_H
