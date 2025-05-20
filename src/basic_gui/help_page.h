#ifndef _HELP_PAGE_
#define _HELP_PAGE_
// HelpDialog.h
#include <QDialog>
#include <QListWidget>
#include <QTextBrowser>
#include <QSplitter>

class HelpPage : public QDialog {
    Q_OBJECT
public:
    explicit HelpPage(QWidget *parent = nullptr);

private:
    void setupUI();
    void loadContents();

    QTextBrowser *contentView;
    QListWidget *contentsList;
    QPushButton *closeBtn;
};


#endif
