#ifndef SEARCHPACKAGESWINDOW_H
#define SEARCHPACKAGESWINDOW_H

#include "searchwindowbase.h"

class SearchPackagesWindow : public SearchWindowBase
{
    Q_OBJECT

public:
    explicit SearchPackagesWindow(QWidget *parent = nullptr);

protected:
    void findAll();
    void findByTags() override;
    void resultSelected(const QString& uid) override;
    Features features() const override { return {CanSearch, CanSearchByTags}; }

private:
    void populate(const QString& tags);
    void showInfo(const QString& uid);
};

#endif // SEARCHPACKAGESWINDOW_H
