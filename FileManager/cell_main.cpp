#include "cell_main.h"
#include "ui_cell_main.h"

#include "QApplication"
#include "QDir"
#include "QStringList"
#include "QDebug"
#include "QStandardItem"
#include "QFileInfoList"
#include "QHeaderView"
#include "QFileDialog"
#include "QFile"
#include "QMessageBox"
#include "QDesktopServices"
#include <QUrl>



Cell_Main::Cell_Main(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Cell_Main)
{
    ui->setupUi(this);

    //状态栏图标改变
    this->setWindowIcon(QIcon(":/img/res/Document.ico"));


    myStrDataPath = QApplication::applicationDirPath()+"/data";
    QDir d(myStrDataPath);

    if(!d.exists())
    {
        d.mkdir(myStrDataPath);
    }



}

Cell_Main::~Cell_Main()
{
    delete ui;

}

void Cell_Main::upDataFile()
{
    //读取文件
    QDir d(myStrDataPath);


    //过滤得到所有文件信息
    QStringList lFilter;
    lFilter<<"*.txt"<<"*.md";
    QFileInfoList lFilesInfo = d.entryInfoList(lFilter,QDir::Files);

    QString myFilter = ui->lineEdit->text();

    myModel.clear();

    //每个文件信息行添加
    QList<QStandardItem * > lRow;
    for (int i = 0 ; i< lFilesInfo.size();i++)
    {

        lRow.clear();
        QFileInfo &info = lFilesInfo[i];

        if(!info.completeBaseName().contains(myFilter))
        {
            continue;
        }

        QStandardItem *p1 = new QStandardItem (info.absoluteFilePath());
        QStandardItem *p2 = new QStandardItem (info.suffix());
        QStandardItem *p3 = new QStandardItem (info.baseName());

        lRow <<p1<<p2<<p3;
        myModel.appendRow(lRow);
    }

    QStringList lHeaders {"路径","文件类型","名称"};
    myModel.setHorizontalHeaderLabels(lHeaders);

    ui->Show_TableView->setModel(&myModel);

    QHeaderView* headerView = ui->Show_TableView->horizontalHeader();
    headerView->setSectionResizeMode(QHeaderView::Stretch);
    ui->Show_TableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);



}



void Cell_Main::on_Upload_Button_clicked()
{
    //选择文件
    auto strPath =QFileDialog::getOpenFileName(nullptr,
                                               "文件上传",QDir::homePath(),
                                               tr("Images (*.png *.xpm *.jpg);;Text files (*.txt);;XML files (*.xml)"));

    if(strPath.isEmpty())
    {
        return;
    }

    //被复制后的文件路径
    auto uploadPath = myStrDataPath +"/"+ strPath.section("/",-1);

    qDebug()<<"strPath:"<<strPath;
    qDebug()<<"myStrDataPath:"<<myStrDataPath;
    qDebug()<<"uploadPath:"<<uploadPath;

    if(QFile::exists(uploadPath))
    {
        QMessageBox::warning(nullptr,"错误","已存在同名文件");
        return;

    }

    bool ret = QFile::copy(strPath, uploadPath);
    QMessageBox::warning (nullptr,"信息",ret?"上传成功":"上传失败");

}

void Cell_Main::on_Delete_Button_clicked()
{
    //获得当前选中数据的索引
    auto currentIndex = ui->Show_TableView->currentIndex();

    if (!currentIndex.isValid())
    {
        return;
    }

    //获得当前选中数据的索引的第一列和改行的索引
    auto itemp = myModel.index(currentIndex.row(),0);
    auto str = myModel.data(itemp).toString();


    bool ret= QFile::remove(str);

    if (ret)
    {
        QMessageBox::information(nullptr,"信息","删除成功");
    }
    else
    {
        QMessageBox::warning(nullptr,"信息","删除失败");
    }




}

void Cell_Main::on_OpenFile_Button_clicked()
{
    //打开文件夹
    QDesktopServices::openUrl(QUrl(myStrDataPath));
    ui->lineEdit_2->setText(myStrDataPath);

}

void Cell_Main::on_Search_Button_clicked()
{
    upDataFile();
}
