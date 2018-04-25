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
class QPushButton;
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
    enum Feature {
        CanSearch = 0x01,
        CanSearchByTags = 0x02,
        CanSearchByUid = 0x04,
        CanSearchByName = 0x08,
    };
    Q_DECLARE_FLAGS(Features, Feature)

    explicit SearchWindowBase(QWidget *parent = nullptr);

    void setTitleAndIcon(const QString& title, const char* iconPath);
    void applyFeatures();

protected:
    QMenu *_resultsContextMenu;
    QListWidget* _resultsList;
    InfoView* _infoPanel;

    virtual void findByTags();
    virtual void findByUid();
    virtual void findByName();
    virtual void editMeta() {}
    virtual void resultSelected(const QString& uid) { Q_UNUSED(uid); }
    virtual Features features() const { return Features(); }

    QString searchText() const;
    QString selectedUid() const;
    QString uidOf(QListWidgetItem* item) const;
    QListWidgetItem* selectedItem() const;

    void cleanResults();
    void addResult(const QString& uid, const QString& text);
    void showHtmlInfo(const QString& html);

private:
    QLineEdit* _searchBox;
    QWidget* _searchPanel;
    QAction *_resultsContextMenuHeader;
    QLabel* _titleResults;
    QPushButton *_findByTagsButton, *_findByUidButton, *_findByNameButton;

    void copyEnvUid() const;

    static QAction* makeHeaderItem(QMenu* menu);
    static QLabel* makeTitleLabel(const QString& title);

    void resultsContextMenuRequested(const QPoint &pos);
    void resultsItemSelected(QListWidgetItem *current, QListWidgetItem *previous);
};

Q_DECLARE_OPERATORS_FOR_FLAGS(SearchWindowBase::Features)

#endif // SEARCHWINDOWBASE_H
