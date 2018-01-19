#ifndef SEARCHWINDOWBASE_H
#define SEARCHWINDOWBASE_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class QAction;
class QLabel;
class QLineEdit;
class QListWidget;
class QListWidgetItem;
class QMenu;
class QTextBrowser;
QT_END_NAMESPACE

class SearchWindowBase : public QWidget
{
    Q_OBJECT

public:
    explicit SearchWindowBase(QWidget *parent = nullptr);

    void setTitleAndIcon(const QString& title, const char* iconPath);

protected:
    QMenu *_resultsContextMenu;
    QListWidget* _resultsList;
    QTextBrowser* _infoPanel;

    virtual void findByTags() {}
    virtual void findByUid() {}
    virtual void editMeta() {}
    virtual void resultSelected(QListWidgetItem *current, QListWidgetItem *previous) { Q_UNUSED(current); Q_UNUSED(previous); }

    QString searchText() const;
    QString selectedUid() const;
    QString uidOf(QListWidgetItem* item) const;

    QString formatValueHtml(const QString& name, const QString& value);

    void cleanResults();
    void addResult(const QString& uid, const QString& text);

private:
    QLineEdit* _searchBox;
    QAction *_resultsContextMenuHeader;
    QLabel* _titleResults;

    void copyEnvUid() const;

    QAction* makeHeaderItem(QMenu* menu);

    void resultsContextMenuRequested(const QPoint &pos);
};

#endif // SEARCHWINDOWBASE_H
