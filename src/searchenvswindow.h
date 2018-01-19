#ifndef ENVLISTWIDGET_H
#define ENVLISTWIDGET_H

#include "searchwindowbase.h"

#include "ckjson.h"

class SearchEnvsWindow : public SearchWindowBase
{
    Q_OBJECT

public:
    explicit SearchEnvsWindow(QWidget *parent = nullptr);

protected:
    void findByTags() override;
    void findByUid() override;
    void editMeta() override;
    void resultSelected(QListWidgetItem *current, QListWidgetItem *previous) override;

private:
    void populateEnvs(const QString& tags, const QString &uid);
    void showEnvInfo(const CkEnvInfo &info, const CkEnvMeta& meta);
    void editEnvScript();
    void refreshEnv();
};

#endif // ENVLISTWIDGET_H
