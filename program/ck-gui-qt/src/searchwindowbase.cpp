#include "searchwindowbase.h"

#include "helpers/OriLayouts.h"
#include "helpers/OriDialogs.h"

#include <QApplication>
#include <QClipboard>
#include <QDebug>
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
    _titleResults = makeTitleLabel("Results:");
    _infoPanel = new InfoView;

    _findByTagsButton = new QPushButton("by tags");
    connect(_findByTagsButton, &QPushButton::clicked, this, &SearchWindowBase::findByTags);
    _findByUidButton = new QPushButton("by UID");
    connect(_findByUidButton, &QPushButton::clicked, this, &SearchWindowBase::findByUid);
    _findByNameButton = new QPushButton("by name");
    connect(_findByNameButton, &QPushButton::clicked, this, &SearchWindowBase::findByName);

    _resultsList = new QListWidget;
    _resultsList->setAlternatingRowColors(true);
    _resultsList->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(_resultsList, &QListWidget::customContextMenuRequested, this, &SearchWindowBase::resultsContextMenuRequested);
    connect(_resultsList, &QListWidget::currentItemChanged, this, &SearchWindowBase::resultsItemSelected);

    _searchPanel = Ori::Layouts::LayoutV({
                                             makeTitleLabel("Search:"),
                                             Ori::Layouts::LayoutH({
                                                 _searchBox,
                                                 _findByTagsButton,
                                                 _findByUidButton,
                                                 _findByNameButton,
                                             }).setSpacing(3),
                                             Ori::Layouts::Space(6),
                                             _titleResults,
                                         })
            .setMargin(0)
            .setSpacing(0)
            .makeWidget();

    auto listPanel = Ori::Layouts::LayoutV({
                                               _searchPanel,
                                               _resultsList,
                                           })
            .setMargin(0)
            .setSpacing(0)
            .makeWidget();

    auto splitter = new QSplitter;
    splitter->addWidget(listPanel);
    splitter->addWidget(_infoPanel);
    splitter->setStretchFactor(0, 1);
    splitter->setStretchFactor(1, 2);

    Ori::Layouts::LayoutV({Ori::Layouts::Space(3), splitter})
            .setMargin(0)
            .setSpacing(0)
            .useFor(this);

    _resultsContextMenu = new QMenu(this);
    _resultsContextMenuHeader = makeHeaderItem(_resultsContextMenu);
    _resultsContextMenu->addSeparator();
    _resultsContextMenu->addAction("Copy UID", this, SLOT(copyEnvUid()));
    _resultsContextMenu->addSeparator();
    _resultsContextMenu->addAction("Edit meta", this, SLOT(editMeta()));
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

QLabel* SearchWindowBase::makeTitleLabel(const QString& title)
{
    auto label = new QLabel(title);
    auto f = label->font();
    f.setBold(true);
    label->setFont(f);
    return label;
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

void SearchWindowBase::findByName()
{
    Ori::Dlg::info("Not implemented");
}

void SearchWindowBase::applyFeatures()
{
    auto f = features();
    _searchPanel->setVisible(f & CanSearch);
    _findByTagsButton->setVisible(f & CanSearchByTags);
    _findByUidButton->setVisible(f & CanSearchByUid);
    _findByNameButton->setVisible(f & CanSearchByName);
}

void SearchWindowBase::setResults(SearchResults &&r)
{
    // Move results into local container and sort them by title
    SearchResults results(r);
    std::sort(results.begin(), results.end(),
        [](auto a, auto b){ return a.title < b.title; });

    _resultsList->clear();
    for (auto &result : results)
    {
        auto item = new QListWidgetItem(_resultsList);
        item->setText(result.title);
        item->setToolTip(result.id);
        item->setData(Qt::UserRole, result.id);
        _resultsList->addItem(item);
    }
    _titleResults->setText(QString("Results (%1):").arg(results.size()));
}
