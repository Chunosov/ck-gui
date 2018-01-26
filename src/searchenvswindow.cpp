#include "searchenvswindow.h"

#include "appevents.h"
#include "ck.h"
#include "utils.h"

#include <QMenu>
#include <QTextBrowser>

SearchEnvsWindow::SearchEnvsWindow(QWidget *parent) : SearchWindowBase(parent)
{
    setTitleAndIcon("Envs", ":/icon/envs");

    _resultsContextMenu->addAction("Edit script", this, &SearchEnvsWindow::editEnvScript);
    _resultsContextMenu->addSeparator();
    _resultsContextMenu->addAction("Refresh env", this, &SearchEnvsWindow::refreshEnv);

    populateEnvs("", "");
}

void SearchEnvsWindow::populateEnvs(const QString& tags, const QString& uid)
{
    CK ck;
    cleanResults();
    for (const CkEntry& env : ck.queryEnvsByTags(tags))
    {
        if (!uid.isEmpty())
        {
            if (!env.uid.contains(uid)) continue;
        }
        addResult(env.uid, env.name);
    }
}

void SearchEnvsWindow::findByTags()
{
    populateEnvs(searchText(), "");
}

void SearchEnvsWindow::findByUid()
{
    populateEnvs("", searchText());
}

void SearchEnvsWindow::resultSelected(QListWidgetItem *current, QListWidgetItem *previous)
{
    Q_UNUSED(previous)
    if (!current) return;

    QString uid = uidOf(current);
    if (uid.isEmpty()) return;

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
            ;

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

    _infoPanel->setHtml(s);
}

void SearchEnvsWindow::editEnvScript()
{
    AppEvents::requestTextEditor(CK::envScriptPath(selectedUid()));
}

void SearchEnvsWindow::editMeta()
{
    AppEvents::requestTextEditor(CK::envMetaPath(selectedUid()));
}

void SearchEnvsWindow::refreshEnv()
{
    CK().refreshEnv(selectedUid());
}
