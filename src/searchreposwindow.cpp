#include "searchreposwindow.h"

#include "ck.h"
#include "utils.h"

SearchReposWindow::SearchReposWindow(QWidget *parent) : SearchWindowBase(parent)
{
    setTitleAndIcon("Envs", ":/icon/repos");

    findAll();
}

void SearchReposWindow::findAll()
{
    cleanResults();
    for (const QString& name : CK::instance().queryRepos())
        addResult(name, name);
}

void SearchReposWindow::resultSelected(const QString& name)
{
    CkRepoMeta meta(name);
    if (!meta.ok()) return;

    showRepoInfo(meta);
}

void SearchReposWindow::showRepoInfo(const CkRepoMeta& meta)
{
    QString s =
        FormatValue("", meta.name())
                    .asHeader()
                    .format()

        + "<p>" + FormatValue("uid", meta.uid())
                    .format();

    QString url = meta.url();
    if (!url.isEmpty())
        s += "<p>" + FormatValue("url", url)
                        .asHyperlink(Utils::urlTarget(url))
                        .format();

    // Format deps
    auto deps = meta.deps();
    if (!deps.isEmpty())
    {
        s += "<p>" + FormatValue("deps", "").format();
        for (auto dep : deps)
        {
            s += "<br>" + FormatValue("", dep.uoa() + " ")
                             .withIndent()
                             .format();
            QString url = dep.url();
            if (!url.isEmpty())
                s += FormatValue("", url)
                        .asHyperlink(Utils::urlTarget(url))
                        .format();
            else
                s += FormatValue("", "(no url is provided)")
                        .withState(FormatValue::Modest)
                        .format();
        }
    }

    showHtmlInfo(s);
}
