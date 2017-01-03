#include "includes/SWindows/History/SHistoryManager.hpp"
#include "includes/SWindows/History/SHistory.hpp"
#include "includes/SWindows/History/SHistoryView.hpp"
#include "includes/SWindows/History/SHistoryModel.hpp"
#include "includes/SMainWindow.hpp"
#include "includes/SWidgets/STabWidget.hpp"
#include "includes/SApplication.hpp"


#include <QHeaderView>
#include <QDate>

SHistoryWindow::SHistoryWindow(SMainWindow * parent) :
    QDialog(parent),
    m_parent(parent),
    m_view(new SHistoryView(this)),
    m_model(new SHistoryModel(this))
{
    // Set attributes of the window
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowTitle(tr("Historique de navigation"));
    resize(758, 450);

    m_view->setModel(m_model);
    m_view->expandAll();

    setupUI();

    // Connections
    connect(m_boxBtn, &QDialogButtonBox::accepted, this, &SHistoryWindow::accept);
    connect(m_boxBtn, &QDialogButtonBox::rejected, this, &SHistoryWindow::reject);


    // Connections
    connect(m_view, &SHistoryView::doubleClicked, this, &SHistoryWindow::load);
    connect(m_view, &SHistoryView::entered, this, &SHistoryWindow::load);
    connect(m_deleteOneBtn, &QPushButton::clicked, this, &SHistoryWindow::deleteOne);
    connect(m_deleteAllBtn, &QPushButton::clicked, this, &SHistoryWindow::deleteAll);

}

SHistoryWindow::~SHistoryWindow()
{
    // Empty
}

void SHistoryWindow::setupUI()
{
    // Add all widgets in layouts
    m_btnLayout->addWidget(m_deleteOneBtn);
    m_btnLayout->addWidget(m_deleteAllBtn);
    m_btnLayout->addItem(m_hSpacer);
    m_btnLayout->addWidget(m_boxBtn);
    m_layout->addWidget(m_view);
    m_layout->addLayout(m_btnLayout);
}

void SHistoryWindow::load()
{
    // Variables
    QModelIndex index{ m_view->currentIndex() };
    QString title{};
    QUrl url{};

    if(m_model->itemFromIndex(index)->parent() == nullptr)
        return;

    if(index.column() == 1) {
        title = m_model->data(m_model->itemFromIndex(index)->parent()->child(index.row())->index()).toString();
        url = m_model->data(index).toUrl();
    }
    else {
        title = m_model->data(index).toString();
        url = m_model->data(m_model->itemFromIndex(index)->parent()->child(index.row(), 1)->index()).toUrl();
    }

    m_parent->getTabs()->createWebTab(title, url);
    m_parent->getTabs()->setCurrentIndex(m_parent->getTabs()->count() - 1);
}

void SHistoryWindow::deleteAll()
{
    mApp->history()->deleteAll();
    mApp->settings()->remove("History");

    m_model->clear();
}

void SHistoryWindow::deleteOne()
{
    if(m_model->itemFromIndex(m_view->currentIndex())->parent() == nullptr) {
        deleteOneFromParent();
    }
    else{
        deleteOneFromItem();
    }
}

void SHistoryWindow::deleteOneFromParent()
{
    QDate dateForDeletation{};

    if(m_model->data(m_view->currentIndex()).toString() == tr("Session actuelle"))
        mApp->history()->deleteAll();
    else if(m_model->data(m_view->currentIndex()).toString() == tr("Hier"))
        dateForDeletation = QDate::currentDate().addDays(-1);
    else
        dateForDeletation = QDate::fromString(m_model->data(m_view->currentIndex().parent()).toString(), "M/d");

    mApp->settings()->remove("History/" + QString::number(QDate::currentDate().year()) + "/" + QString::number(dateForDeletation.month()) + "/" + QString::number(dateForDeletation.day()));
    m_model->removeRow(m_view->currentIndex().row(), m_view->currentIndex().parent());
}

void SHistoryWindow::deleteOneFromItem()
{
    QDate dateForDeletation{};

    if(m_model->data(m_view->currentIndex().parent()).toString() == tr("Aujourd'hui"))
        mApp->history()->deleteOne(mApp->history()->items()[mApp->history()->items().size() - 1 - m_view->currentIndex().row()]);
    else if(m_model->data(m_view->currentIndex().parent()).toString() == tr("Hier"))
        dateForDeletation = QDate::currentDate().addDays(-1);
    else
        dateForDeletation = QDate::fromString(m_model->data(m_view->currentIndex().parent()).toString(), "M/d");

    mApp->settings()->beginGroup("History/" + QString::number(QDate::currentDate().year()) + "/" + QString::number(dateForDeletation.month()) + "/" + QString::number(dateForDeletation.day()));
    int size{ mApp->settings()->beginReadArray("history") };
    int indexToDelete{ mApp->settings()->value("itemNum", 0).toInt() - 1 - m_view->currentIndex().row() };
    QList<SHistoryItem> itemsOfDate{};

    for (int i{ 0 }; i < size; ++i) {
        mApp->settings()->setArrayIndex(i);
        itemsOfDate.append(mApp->settings()->value("item").value<SHistoryItem>());
    }
    mApp->settings()->endArray();

    itemsOfDate.removeAt(indexToDelete);
    mApp->settings()->remove("history");

    mApp->settings()->beginWriteArray("history");

    for (int i{ 0 }; i < itemsOfDate.size(); ++i) {
        mApp->settings()->setArrayIndex(i);
        mApp->settings()->setValue("item", QVariant::fromValue(itemsOfDate[i]));
    }

    mApp->settings()->endArray();
    mApp->settings()->endGroup();

    m_model->removeRow(m_view->currentIndex().row(), m_view->currentIndex().parent());
}
