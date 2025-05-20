#include "help_page.h"

#include <qguiapplication_platform.h>
#include <QVBoxLayout>

HelpPage::HelpPage(QWidget *parent):QDialog(parent) {
    setupUI();
    loadContents();
}

void HelpPage::loadContents() {
    QStringList topics = {tr("布局解释"),tr("导入地图"),tr("属性面板"),tr("路径规划"),tr("关于作者")};
    contentsList->setCurrentRow(0);
    contentsList->addItems(topics);
    connect(contentsList,&QListWidget::currentRowChanged,[this](int row){contentView->setSource(QUrl("qrc:/help/resources/"+QString::number(row)+".html"));});

}

void HelpPage::setupUI() {
    QHBoxLayout *layout = new QHBoxLayout(this);
    this->setLayout(layout);
    QSplitter *splitter = new QSplitter(this);
    contentsList = new QListWidget(this);
    contentsList->setFixedWidth(200);
    contentView = new QTextBrowser(this);
    contentView->setOpenExternalLinks(true);
    layout->addWidget(splitter);
    splitter->addWidget(contentsList);
    splitter->addWidget(contentView);
    this->setWindowTitle(tr("Help Page"));
    contentView->setFrameShape(QFrame::Box);
    contentView->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    contentsList->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

}
