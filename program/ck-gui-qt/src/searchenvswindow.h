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
    void findAll();
    void findByTags() override;
    void findByUid() override;
    void editMeta() override;
    void resultSelected(const QString& uid) override;
    Features features() const override { return {CanSearch, CanSearchByTags, CanSearchByUid}; }

private slots:
    void editEnvScript();
    void refreshEnv();
    void deleteEnv();

private:
    enum SearchMode { SEARCH_ALL, SEARCH_TAG, SEARCH_UID };
    void populateEnvs(const QString& tags, const QString &uid);
    void showEnvInfo(const CkEnvInfo &info, const CkEnvMeta& meta);
    void repeatSearch();

    SearchMode _lastSearch;
};

#endif // ENVLISTWIDGET_H
