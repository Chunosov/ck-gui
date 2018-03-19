#ifndef SEARCHWINDOWBASE_H
#define SEARCHWINDOWBASE_H

#include <QWidget>
#include <QTextBrowser>

QT_BEGIN_NAMESPACE
class QAction;
class QLabel;
class QLineEdit;
class QListWidget;
class QListWidgetItem;
class QMenu;
QT_END_NAMESPACE

class FormatValue
{
public:
    enum ValueState { Normal, Error, Modest };
    FormatValue(const QString& name, const QString& value): _name(name), _value(value){}
    FormatValue& withIndent();
    FormatValue& withState(ValueState state) { _state = state; return *this; }
    FormatValue& asHeader() { _isHeader = true; return *this; }
    FormatValue& asHyperlink(const QString& linkTarget = QString()) {
        _isHyperlink = true, _hyperlinkTarget = linkTarget; return *this;
    }
    QString format();
private:
    ValueState _state = Normal;
    QString _name;
    QString _value;
    QString stateStyle();
    bool _isHeader = false;
    bool _isHyperlink = false;
    QString _hyperlinkTarget;
    QString _indent;
};

//-----------------------------------------------------------------------------

class InfoView : public QTextBrowser
{
public:
    explicit InfoView(QWidget* parent = nullptr) : QTextBrowser(parent) {}
protected:
    void mousePressEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;
private:
    bool shouldProcess(QMouseEvent *e);
    QString _clickedAnchor;
};

//-----------------------------------------------------------------------------

class SearchWindowBase : public QWidget
{
    Q_OBJECT

public:
    explicit SearchWindowBase(QWidget *parent = nullptr);

    void setTitleAndIcon(const QString& title, const char* iconPath);

protected:
    QMenu *_resultsContextMenu;
    QListWidget* _resultsList;
    InfoView* _infoPanel;

    virtual void findByTags();
    virtual void findByUid();
    virtual void editMeta() {}
    virtual void resultSelected(const QString& uid) { Q_UNUSED(uid); }

    QString searchText() const;
    QString selectedUid() const;
    QString uidOf(QListWidgetItem* item) const;
    QListWidgetItem* selectedItem() const;

    void cleanResults();
    void addResult(const QString& uid, const QString& text);
    void showHtmlInfo(const QString& html);

private:
    QLineEdit* _searchBox;
    QAction *_resultsContextMenuHeader;
    QLabel* _titleResults;

    void copyEnvUid() const;

    QAction* makeHeaderItem(QMenu* menu);

    void resultsContextMenuRequested(const QPoint &pos);
    void resultsItemSelected(QListWidgetItem *current, QListWidgetItem *previous);
};

#endif // SEARCHWINDOWBASE_H
