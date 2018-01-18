#ifndef ENVLISTWIDGET_H
#define ENVLISTWIDGET_H

#include <QWidget>

#include "ckjson.h"

QT_BEGIN_NAMESPACE
class QAction;
class QLabel;
class QLineEdit;
class QListWidget;
class QListWidgetItem;
class QMenu;
class QTextBrowser;
QT_END_NAMESPACE

class EnvListWidget : public QWidget
{
    Q_OBJECT

public:
    explicit EnvListWidget(QWidget *parent = nullptr);

private:
    QListWidget* _envList;
    QLineEdit* _searchBox;
    QLabel* _titleResults;
    QTextBrowser* _envInfoPanel;
    QMenu *_envListContextMenu;
    QAction *_envListContextMenuHeader;

    void populateEnvs(const QString& tags, const QString &uid);
    void findByTags();
    void findByUid();
    void envSelected(QListWidgetItem *current, QListWidgetItem *previous);
    void showEnvInfo(const CkEnvInfo &info, const CkEnvMeta& meta);
    QString formatValueHtml(const QString& name, const QString& value);
    QAction* makeHeaderItem(QMenu* menu);
    void copyEnvUid();
    QString selectedUid();
    void envListContextMenuRequested(const QPoint &pos);
    QString uidOf(QListWidgetItem* item);
    void editEnvScript();
    void editEnvMeta();
    void refreshEnv();
};

#endif // ENVLISTWIDGET_H
