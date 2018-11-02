#include "searchreposwindow.h"

#include "ck.h"

SearchReposWindow::SearchReposWindow(QWidget *parent) : SearchWindowBase(parent)
{
    setTitleAndIcon("Repos", ":/icon/repos");

    findAll();
}

void SearchReposWindow::findAll()
{
    SearchResults results;
    for (const QString& name : CK::instance().queryRepos())
        results.append({name, name});
    setResults(std::move(results));
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

       + "<p>" + FormatValue("uoa", meta.uoa())
                    .format()

       + "<p>" + FormatValue("uid", meta.uid())
                    .format();

    QString url = meta.url();
    if (!url.isEmpty())
        s += "<p>" + FormatValue("url", url)
                        .asHyperlink(urlTarget(url))
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
                        .asHyperlink(urlTarget(url))
                        .format();
            else
                s += FormatValue("", "(no url is provided)")
                        .withState(FormatValue::Modest)
                        .format();
        }
    }

    showHtmlInfo(s);
}

QString SearchReposWindow::urlTarget(const QString& url)
{
    if (url.startsWith("git@"))
    {
        int pos = url.indexOf(':');
        if (pos < 5) return url;
        QStringRef host(&url, 4, pos-4);
        int len = url.length()-pos-1;
        if (url.endsWith(".git")) len -= 4;
        QStringRef address(&url, pos+1, len);
        return "https://" % host % "/" % address;
    }
    else
        return url;
}
