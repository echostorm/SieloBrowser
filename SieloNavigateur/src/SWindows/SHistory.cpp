#include "includes/SWindows/SHistory.hpp"
#include "includes/SMainWindow.hpp"
#include "includes/SWidgets/STabWidget.hpp"
#include "includes/SApplication.hpp"

#include <QHeaderView>
#include <QDate>

SHistoryWindow::SHistoryWindow(SMainWindow * parent) :
	QDialog(parent),
	m_parent(parent)
{
	// Set attributes of the window
	setAttribute(Qt::WA_DeleteOnClose);
	setWindowTitle(tr("Historique de navigation"));
	resize(758, 450);

	// Set attribute of the view
	m_view->setEditTriggers(QAbstractItemView::NoEditTriggers);
	m_view->setProperty("showDropIndicator", QVariant(false));
	m_view->setAlternatingRowColors(true);
	m_view->setAnimated(true);
	m_view->header()->setDefaultSectionSize(300);
	m_view->setModel(m_model);

	// Add all widgets in layouts
	m_btnLayout->addWidget(m_deleteOneBtn);
	m_btnLayout->addWidget(m_deleteAllBtn);
	m_btnLayout->addItem(m_hSpacer);
	m_btnLayout->addWidget(m_boxBtn);
	m_layout->addWidget(m_view);
	m_layout->addLayout(m_btnLayout);

	// Connections
	connect(m_boxBtn, &QDialogButtonBox::accepted, this, &SHistoryWindow::accept);
	connect(m_boxBtn, &QDialogButtonBox::rejected, this, &SHistoryWindow::reject);

	// Fill the history view
	QList<QStandardItem*> historyItemTitle{};
	QList<QStandardItem*> historyItemUrl{};

	for(int i{ SMainWindow::curSessionHistory.size() - 1 }; i >= 0; --i) {
		historyItemTitle.append(new QStandardItem(SMainWindow::curSessionHistory[i].title));
		historyItemUrl.append(new QStandardItem(SMainWindow::curSessionHistory[i].url.toString()));
	}
	QStandardItem *currentSessionItem{ new QStandardItem(tr("Session actuelle")) };
	currentSessionItem->appendColumn(historyItemTitle);
	currentSessionItem->appendColumn(historyItemUrl);
	m_model->appendRow(currentSessionItem);

	m_view->expandAll();

	historyItemTitle.clear();
	historyItemUrl.clear();

	QDate date{ QDate::currentDate() };
	QDate dateToShow{ QDate::currentDate().addDays(-7) };

	for (date = QDate::currentDate(); date >= dateToShow; date = date.addDays(-1)) {
        mApp->settings()->beginGroup("History/" + QString::number(date.year()) + "/" + QString::number(date.month()) + "/" + QString::number(date.day()));

        if(!mApp->settings()->contains("itemNum") || mApp->settings()->value("itemNum", 0).toInt() == 0) {
            mApp->settings()->endGroup();
			continue;
		}

        for (int i{ mApp->settings()->value("itemNum", 0).toInt() - 1 }; i >= 0; --i) {
            if(!mApp->settings()->contains(QString::number(i) + "/title")) {
				continue;
			}

            QString title{ mApp->settings()->value(QString::number(i) + "/title", "nullTitle").toString() };
            QString url{ mApp->settings()->value(QString::number(i) + "/url", "nullUrl").toString() };

			historyItemTitle.append(new QStandardItem(title));
			historyItemUrl.append(new QStandardItem(url));

		}
        mApp->settings()->endGroup();

		QStandardItem *dateItem{ new QStandardItem(QString::number(date.month()) + "/" + QString::number(date.day())) };

		if(date == QDate::currentDate())
			dateItem->setText(tr("Aujourd'hui"));
		if(date == QDate(QDate::currentDate().year(), QDate::currentDate().month(), QDate::currentDate().day() - 1))
			dateItem->setText(tr("Hier"));

		dateItem->insertColumn(0, historyItemTitle);
		dateItem->insertColumn(1, historyItemUrl);

		historyItemTitle.clear();
		historyItemUrl.clear();

		m_model->appendRow(dateItem);

	}

	QStringList headersNams;
	headersNams.push_back(tr("Nom des pages"));
	headersNams.push_back(tr("Adresses"));
	m_model->setHorizontalHeaderLabels(headersNams);

	// Connections
	connect(m_view, &QTreeView::doubleClicked, this, &SHistoryWindow::load);
	connect(m_view, &QTreeView::entered, this, &SHistoryWindow::load);
	connect(m_deleteOneBtn, &QPushButton::clicked, this, &SHistoryWindow::deleteOne);
	connect(m_deleteAllBtn, &QPushButton::clicked, this, &SHistoryWindow::deleteAll);

}

SHistoryWindow::~SHistoryWindow()
{
	// Empty
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
	SMainWindow::curSessionHistory.clear();
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
		SMainWindow::curSessionHistory.clear();
	else if(m_model->data(m_view->currentIndex()).toString() == tr("Aujourd'hui"))
		dateForDeletation = QDate::currentDate();
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

	if(m_model->data(m_view->currentIndex().parent()).toString() == tr("Session actuelle"))
		SMainWindow::curSessionHistory.remove(SMainWindow::curSessionHistory.size() - 1 - m_view->currentIndex().row());
	if(m_model->data(m_view->currentIndex().parent()).toString() == tr("Aujourd'hui"))
		dateForDeletation = QDate::currentDate();
	else if(m_model->data(m_view->currentIndex().parent()).toString() == tr("Hier"))
		dateForDeletation = QDate::currentDate().addDays(-1);
	else
		dateForDeletation = QDate::fromString(m_model->data(m_view->currentIndex().parent()).toString(), "M/d");

    mApp->settings()->beginGroup("History/" + QString::number(QDate::currentDate().year()) + "/" + QString::number(dateForDeletation.month()) + "/" + QString::number(dateForDeletation.day()));

    int indexToDelete{ mApp->settings()->value("itemNum", 0).toInt() - 1 - m_view->currentIndex().row() };

    mApp->settings()->remove(QString::number(indexToDelete));
    for(int i{ indexToDelete }; i < mApp->settings()->value("itemNum").toInt() - 1; ++i) {
        mApp->settings()->setValue(QString::number(i) + "/title", mApp->settings()->value(QString::number(i + 1) + "/title", "nullTitle").toString());
        mApp->settings()->setValue(QString::number(i) + "/url", mApp->settings()->value(QString::number(i + 1) + "/url", "nullUrl").toUrl());
	}

    mApp->settings()->setValue("itemNum", mApp->settings()->value("itemNum", 0).toInt() - 1);
    mApp->settings()->remove(QString::number(mApp->settings()->value("itemNum", 0).toInt()) + "/title");
    mApp->settings()->remove(QString::number(mApp->settings()->value("itemNum", 0).toInt()) + "/url");

    mApp->settings()->endGroup();

	m_model->removeRow(m_view->currentIndex().row(), m_view->currentIndex().parent());
}
