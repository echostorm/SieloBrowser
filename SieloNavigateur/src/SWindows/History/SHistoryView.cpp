#include "includes/SWindows/History/SHistoryView.hpp"

#include <QAbstractItemView>
#include <QVariant>
#include <QHeaderView>

SHistoryView::SHistoryView(SHistoryWindow *parent) :
    QTreeView(parent),
    m_parent(parent)
{
    setEditTriggers(QAbstractItemView::NoEditTriggers);
    setProperty("showDropIndicator", QVariant(false));
    setAlternatingRowColors(true);
    setAnimated(true);
    header()->setDefaultSectionSize(300);
}

SHistoryView::~SHistoryView()
{
    // Empty
}
