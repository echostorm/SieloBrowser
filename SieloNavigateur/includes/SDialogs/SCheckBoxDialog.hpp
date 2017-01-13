#ifndef SCHECKBOXDIALOG_HPP
#define SCHECKBOXDIALOG_HPP

#include <QDialog>
#include <QDialogButtonBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QDialogButtonBox>
#include <QCheckBox>
#include <QSpacerItem>

class SCheckBoxDialog : public QDialog
{
  Q_OBJECT
public:
    SCheckBoxDialog(const QDialogButtonBox::StandardButtons &buttons, QWidget *parent = nullptr);

    void setIcon(const QIcon &icon);

    void setText(const QString &text);
    void setCheckBoxText(const QString &text);

    bool isChecked() const;
    void setDefaultCheckState(Qt::CheckState state);

public slots:
    int exec();

private:
    void setupUi();

    QVBoxLayout *m_layout{ nullptr };
    QHBoxLayout *m_textLayout{ nullptr };
    QHBoxLayout *m_buttonLayout{ nullptr };

    QLabel *m_iconLabel{ nullptr };
    QLabel *m_textLabel{ nullptr };

    QDialogButtonBox *m_buttonBox{ nullptr };
    QCheckBox *m_checkBox{ nullptr };
    QSpacerItem *m_spacerItem{ nullptr };
};

#endif // SCHECKBOXDIALOG_HPP
