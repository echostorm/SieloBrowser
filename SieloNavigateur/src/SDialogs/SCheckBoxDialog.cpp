#include "includes/SDialogs/SCheckBoxDialog.hpp"

SCheckBoxDialog::SCheckBoxDialog(const QDialogButtonBox::StandardButtons &buttons, QWidget *parent) :
    QDialog(parent, Qt::MSWindowsFixedSizeDialogHint | Qt::WindowTitleHint | Qt::WindowSystemMenuHint | Qt::WindowCloseButtonHint)
{
    setupUi();

    m_buttonBox->setStandardButtons(buttons);
}

void SCheckBoxDialog::setIcon(const QIcon &icon)
{
    m_iconLabel->setPixmap(icon.pixmap(48, 48));
    m_iconLabel->setFixedWidth(48);
}

void SCheckBoxDialog::setText(const QString &text)
{
    m_textLabel->setText(text);
}

void SCheckBoxDialog::setCheckBoxText(const QString &text)
{
    m_checkBox->setText(text);
}

bool SCheckBoxDialog::isChecked() const
{
    return m_checkBox->isChecked();
}

void SCheckBoxDialog::setDefaultCheckState(Qt::CheckState state)
{
    m_checkBox->setChecked(state == Qt::Checked);
}

int SCheckBoxDialog::exec()
{
    m_buttonBox->setFocus();
    setMaximumSize(size());

    return QDialog::exec();
}

void SCheckBoxDialog::setupUi()
{
    m_layout = new QVBoxLayout(this);
    m_textLayout = new QHBoxLayout();
    m_buttonLayout = new QHBoxLayout();

    m_iconLabel = new QLabel(this);
    m_textLabel = new QLabel(this);

    m_textLayout->addWidget(m_iconLabel);
    m_textLayout->addWidget(m_textLabel);

    m_buttonBox = new QDialogButtonBox(this);
    m_checkBox = new QCheckBox(this);
    m_spacerItem = new QSpacerItem(10, 10, QSizePolicy::Expanding);

    m_buttonLayout->addItem(m_spacerItem);
    m_buttonLayout->addWidget(m_checkBox);
    m_buttonLayout->addWidget(m_buttonBox);

    m_layout->addLayout(m_textLayout);
    m_layout->addLayout(m_buttonLayout);
    setLayout(m_layout);
}
