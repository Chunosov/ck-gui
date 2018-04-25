#ifndef SEARCH_PACKAGES_WINDOW_H
#define SEARCH_PACKAGES_WINDOW_H

#include "searchwindowbase.h"

class SearchPackagesWindow : public SearchWindowBase
{
    Q_OBJECT

public:
    explicit SearchPackagesWindow(QWidget *parent = nullptr);

protected:
    void findByTags() override;
    void findByName() override;
    void editMeta() override;
    void resultSelected(const QString& uid) override;
    Features features() const override { return {CanSearch, CanSearchByTags, CanSearchByName}; }

private:
    void populate(const QStringList &searchResults);
    void showInfo(const QString& uid);
};

#endif // SEARCH_PACKAGES_WINDOW_H
