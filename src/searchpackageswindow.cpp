#include "searchpackageswindow.h"

#include "appevents.h"
#include "ck.h"

#include <QListWidgetItem>

SearchPackagesWindow::SearchPackagesWindow(QWidget *parent) : SearchWindowBase(parent)
{
    setTitleAndIcon("Packages", ":/icon/packages");
}

void SearchPackagesWindow::populate(const QStringList& searchResults)
{
    SearchResults results;
    for (const QString& name : searchResults)
    {
        QStringList parts = name.split(':');
        results.append({name, parts.last()});
    }
    setResults(std::move(results));
}

void SearchPackagesWindow::findByTags()
{
    populate(CK::instance().queryPackagesByTags(searchText()));
}

void SearchPackagesWindow::findByName()
{
    populate(CK::instance().queryPackagesByName(searchText()));
}

void SearchPackagesWindow::editMeta()
{
    auto item = selectedItem();
    if (item)
        AppEvents::requestTextEditor(CK::packageMetaPath(uidOf(item)), item->text() + " [Meta]");
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
    auto full_path = CK::packagePath(uid);

    QString s =
        FormatValue("", QString("<b>%1</b>").arg(name))
                    .asHeader()
                    .format()
            + "<p>" + FormatValue("repo", repo)
                       .format()
            + "<p>" + FormatValue("path", full_path)
                       .withState(CK::isFileExists(full_path) ? FormatValue::Normal : FormatValue::Error)
                       .format();

    showHtmlInfo(s);
}
