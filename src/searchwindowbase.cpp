#include "searchwindowbase.h"
#include "utils.h"
#include "orion/helpers/OriLayouts.h"
#include "orion/helpers/OriDialogs.h"

#include <QApplication>
#include <QClipboard>
#include <QDesktopServices>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QMenu>
#include <QMouseEvent>
#include <QPushButton>
#include <QSplitter>

QString FormatValue::stateStyle()
{
    static QString normalStyle("color: black");
    static QString errorStyle("color: red");
    static QString modestStyle("color: gray");
    switch (_state)
    {
    case Normal: return normalStyle;
    case Error: return errorStyle;
    case Modest: return modestStyle;
    }
    return QString();
}

FormatValue& FormatValue::withIndent()
{
    static QString indent("&nbsp;&nbsp;&nbsp;&nbsp;");
    _indent += indent;
    return *this;
}

QString FormatValue::format()
{
    QString nameStr, valueStr;
    if (_isHeader)
    {
        nameStr = QString("<span style='font: 14pt'>%1</span>").arg(_value);
    }
    else
    {
        if (!_name.isEmpty())
            nameStr = QString("<b>%1</b>: ").arg(_name);

        if (!_value.isEmpty())
        {
            if (_isHyperlink)
            {
                QString href = _hyperlinkTarget.isEmpty() ? _value: _hyperlinkTarget;
                valueStr = QString("<a href='%2'>%1</a>").arg(_value, href);
            }
            else
                valueStr = QString("<span style='%2'>%1</span>").arg(_value, stateStyle());
        }
    }
    return QString("%1%2%3").arg(_indent, nameStr, valueStr);
}

//-----------------------------------------------------------------------------

bool InfoView::shouldProcess(QMouseEvent *e)
{
    return /*e->modifiers() & Qt::ControlModifier &&*/ e->button() & Qt::LeftButton;
}

void InfoView::mousePressEvent(QMouseEvent *e)
{
    QString href = anchorAt(e->pos());
    if (!href.isEmpty() && shouldProcess(e))
        _clickedAnchor = href;
    else QTextEdit::mousePressEvent(e);
}

void InfoView::mouseReleaseEvent(QMouseEvent *e)
{
    if (!_clickedAnchor.isEmpty() && shouldProcess(e))
    {
        QDesktopServices::openUrl(_clickedAnchor);
        _clickedAnchor.clear();
    }
    else QTextEdit::mouseReleaseEvent(e);
}

//-----------------------------------------------------------------------------

SearchWindowBase::SearchWindowBase(QWidget *parent) : QWidget(parent)
{
    _searchBox = new QLineEdit;
    _titleResults = Utils::makeTitle("Results:");
    _infoPanel = new InfoView;

    auto findByTagsButton = new QPushButton("by tags");
    connect(findByTagsButton, &QPushButton::clicked, this, &SearchWindowBase::findByTags);
    auto findByUidButton = new QPushButton("by UID");
    connect(findByUidButton, &QPushButton::clicked, this, &SearchWindowBase::findByUid);

    _resultsList = new QListWidget;
    _resultsList->setAlternatingRowColors(true);
    _resultsList->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(_resultsList, &QListWidget::customContextMenuRequested, this, &SearchWindowBase::resultsContextMenuRequested);
    connect(_resultsList, &QListWidget::currentItemChanged, this, &SearchWindowBase::resultsItemSelected);

    auto searchPanel = new QWidget;
    Ori::Layouts::LayoutV({
                              Utils::makeTitle("Search:"),
                              Ori::Layouts::LayoutH({
                                  _searchBox,
                                  findByTagsButton,
                                  findByUidButton,
                              }).setSpacing(3),
                              Ori::Layouts::Space(6),
                              _titleResults,
                              _resultsList,
                          })
            .setMargin(0)
            .setSpacing(0)
            .useFor(searchPanel);

    auto splitter = new QSplitter;
    splitter->addWidget(searchPanel);
    splitter->addWidget(_infoPanel);
    splitter->setStretchFactor(0, 1);
    splitter->setStretchFactor(1, 2);

    Ori::Layouts::LayoutV({splitter})
            .setMargin(0)
            .setSpacing(0)
            .useFor(this);

    _resultsContextMenu = new QMenu(this);
    _resultsContextMenuHeader = makeHeaderItem(_resultsContextMenu);
    _resultsContextMenu->addSeparator();
    _resultsContextMenu->addAction("Copy UID", this, &SearchWindowBase::copyEnvUid);
    _resultsContextMenu->addSeparator();
    _resultsContextMenu->addAction("Edit meta", this, &SearchWindowBase::editMeta);
}

void SearchWindowBase::setTitleAndIcon(const QString& title, const char* iconPath)
{
    setWindowTitle(title);
    setWindowIcon(QIcon(iconPath));
}

QAction* SearchWindowBase::makeHeaderItem(QMenu* menu)
{
    QAction* item = menu->addAction("");
    auto font = item->font();
    font.setBold(true);
    font.setPointSize(font.pointSize()+2);
    item->setFont(font);
    item->setEnabled(false);
    return item;
}

void SearchWindowBase::resultsContextMenuRequested(const QPoint &pos)
{
    auto popupItem = _resultsList->itemAt(pos);
    if (!popupItem) return;

    _resultsContextMenuHeader->setText(uidOf(popupItem));
    _resultsContextMenu->popup(_resultsList->mapToGlobal(pos));
}

void SearchWindowBase::resultsItemSelected(QListWidgetItem *current, QListWidgetItem *previous)
{
    Q_UNUSED(previous)
    if (!current) return;

    QString uid = uidOf(current);
    if (uid.isEmpty()) return;

    resultSelected(uid);
}

QListWidgetItem* SearchWindowBase::selectedItem() const
{
    auto items = _resultsList->selectedItems();
    return items.isEmpty() ? nullptr : items.first();
}

QString SearchWindowBase::selectedUid() const
{
    return uidOf(selectedItem());
}

QString SearchWindowBase::uidOf(QListWidgetItem* item) const
{
    return item ? item->data(Qt::UserRole).toString() : QString();
}

void SearchWindowBase::copyEnvUid() const
{
    QApplication::clipboard()->setText(selectedUid());
}

QString SearchWindowBase::searchText() const
{
    return _searchBox->text().trimmed();
}

void SearchWindowBase::cleanResults()
{
    _resultsList->clear();
}

void SearchWindowBase::addResult(const QString& uid, const QString& text)
{
    auto item = new QListWidgetItem(_resultsList);
    item->setText(text);
    item->setToolTip(uid);
    item->setData(Qt::UserRole, uid);
    _resultsList->addItem(item);
}

void SearchWindowBase::showHtmlInfo(const QString& html)
{
    _infoPanel->setHtml(html);
}

void SearchWindowBase::findByTags()
{
    Ori::Dlg::info("Not implemented");
}

void SearchWindowBase::findByUid()
{
    Ori::Dlg::info("Not implemented");
}
