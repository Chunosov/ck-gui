#ifndef SEARCHREPOSWINDOW_H
#define SEARCHREPOSWINDOW_H

#include "searchwindowbase.h"

#include "ckjson.h"

class SearchReposWindow : public SearchWindowBase
{
    Q_OBJECT
public:
    explicit SearchReposWindow(QWidget *parent = nullptr);

protected:
    void findAll();
    void resultSelected(const QString& name) override;

private:
    void showRepoInfo(const CkRepoMeta& meta);

    static QString urlTarget(const QString& url);
};

#endif // SEARCHREPOSWINDOW_H
