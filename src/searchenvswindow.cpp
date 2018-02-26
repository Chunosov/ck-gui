#include "searchenvswindow.h"

#include "appevents.h"
#include "ck.h"
#include "utils.h"
#include "orion/helpers/OriDialogs.h"

#include <QMenu>
#include <QListWidgetItem>

SearchEnvsWindow::SearchEnvsWindow(QWidget *parent) : SearchWindowBase(parent)
{
    setTitleAndIcon("Envs", ":/icon/envs");

    _resultsContextMenu->addAction("Edit script", this, &SearchEnvsWindow::editEnvScript);
    _resultsContextMenu->addSeparator();
    _resultsContextMenu->addAction("Refresh env", this, &SearchEnvsWindow::refreshEnv);
    _resultsContextMenu->addAction("Delete env", this, &SearchEnvsWindow::deleteEnv);

    findAll();
}

void SearchEnvsWindow::populateEnvs(const QString& tags, const QString& uid)
{
    cleanResults();
    for (const CkEntry& env : CK::instance().queryEnvsByTags(tags))
    {
        if (!uid.isEmpty())
        {
            if (!env.uid.contains(uid)) continue;
        }
        addResult(env.uid, env.name);
    }
}

void SearchEnvsWindow::findAll()
{
    _lastSearch = SEARCH_ALL;
    populateEnvs("", "");
}

void SearchEnvsWindow::findByTags()
{
    _lastSearch = SEARCH_TAG;
    populateEnvs(searchText(), "");
}

void SearchEnvsWindow::findByUid()
{
    _lastSearch = SEARCH_UID;
    populateEnvs("", searchText());
}

void SearchEnvsWindow::repeatSearch()
{
    switch (_lastSearch)
    {
    case SEARCH_ALL: findAll(); break;
    case SEARCH_TAG: findByTags(); break;
    case SEARCH_UID: findByUid(); break;
    }
}

void SearchEnvsWindow::resultSelected(const QString& uid)
{
    CkEnvMeta meta(uid);
    if (!meta.ok()) return;

    CkEnvInfo info(uid);
    if (!info.ok()) return;

    showEnvInfo(info, meta);
}

void SearchEnvsWindow::showEnvInfo(const CkEnvInfo& info, const CkEnvMeta& meta)
{
    static QString indent("&nbsp;&nbsp;&nbsp;&nbsp;");

    auto env_uid = info.backup_data_uid();
    auto full_path = meta.full_path();
    auto env_script_path = Utils::makePath({ CK::envPath(env_uid), meta.env_script() });

    QString s =
        FormatValue("", QString("<b>%1</b> %2").arg(info.data_name(), meta.version()))
                    .asHeader()
                    .format()

        + "<p>" + FormatValue("uid", env_uid)
                    .format()

        + "<p>" + FormatValue("env_script", env_script_path)
                    .withState(CK::isFileExists(env_script_path) ? FormatValue::Normal : FormatValue::Error)
                    .format()

        + "<p>" + FormatValue("full_path", full_path)
                    .withState(CK::isFileExists(full_path) ? FormatValue::Normal : FormatValue::Error)
                    .format()

        + "<p>" + FormatValue("tags", meta.tags().join(", "))
                    .format()
            ;

    // Format deps
    auto deps = meta.deps();
    if (!deps.isEmpty())
    {
        s += "<p>" + FormatValue("deps", "").format();
        for (auto dep : deps)
        {
            s += QString("<br>%1%2 <b>%3</b>").arg(indent, dep.name(), dep.version());
            auto uoa = dep.uoa();
            if (!uoa.isEmpty())
                s += QString(" <span style='color: gray'>(%1)</span>").arg(uoa);
        }
    }

    showHtmlInfo(s);
}

void SearchEnvsWindow::editEnvScript()
{
    auto item = selectedItem();
    if (item)
        AppEvents::requestTextEditor(CK::envScriptPath(uidOf(item)), item->text() + " [Script]");
}

void SearchEnvsWindow::editMeta()
{
    auto item = selectedItem();
    if (item)
        AppEvents::requestTextEditor(CK::envMetaPath(uidOf(item)), item->text() + " [Meta]");
}

void SearchEnvsWindow::refreshEnv()
{
    CK::instance().refreshEnv(selectedUid());
}

void SearchEnvsWindow::deleteEnv()
{
    auto uid = selectedUid();
    if (uid.isEmpty()) return;
    if (Ori::Dlg::yes(tr("Are you sure to delete env %1").arg(uid)))
    {
        CK::instance().removeEnv(uid);
        repeatSearch();
    }
}
