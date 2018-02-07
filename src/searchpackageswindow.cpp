#include "searchpackageswindow.h"

#include "ck.h"

SearchPackagesWindow::SearchPackagesWindow(QWidget *parent) : SearchWindowBase(parent)
{
    setTitleAndIcon("Packages", ":/icon/packages");

    findAll();
}

void SearchPackagesWindow::populate(const QString& tags)
{
    cleanResults();
    for (const QString& name : CK::instance().queryPackagesByTags(tags))
    {
        QStringList parts = name.split(':');
        addResult(name, parts.last());
    }
}

void SearchPackagesWindow::findAll()
{
    populate("");
}

void SearchPackagesWindow::findByTags()
{
    populate(searchText());
}

void SearchPackagesWindow::resultSelected(const QString& uid)
{
    showInfo(uid);
}

void SearchPackagesWindow::showInfo(const QString& uid)
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
