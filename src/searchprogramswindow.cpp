#include "searchprogramswindow.h"

#include "ck.h"

SearchProgramsWindow::SearchProgramsWindow(QWidget *parent) : SearchWindowBase(parent)
{
    setTitleAndIcon("Programs", ":/icon/programs");

    findAll();
}

void SearchProgramsWindow::populate(const QString& tags)
{
    cleanResults();
    for (const QString& name : CK::instance().queryProgramsByTags(tags))
    {
        QStringList parts = name.split(':');
        addResult(name, parts.last());
    }
}

void SearchProgramsWindow::findAll()
{
    populate("");
}

void SearchProgramsWindow::findByTags()
{
    populate(searchText());
}

void SearchProgramsWindow::resultSelected(const QString& uid)
{
    showInfo(uid);
}

void SearchProgramsWindow::showInfo(const QString& uid)
{
    auto parts = uid.split(':');
    auto name = parts.last();
    auto repo = parts.first();

    QString s =
        FormatValue("", QString("<b>%1</b>").arg(name))
                    .asHeader()
                    .format()
            + "<p>" + FormatValue("repo", repo)
                        .format();

    showHtmlInfo(s);
}

