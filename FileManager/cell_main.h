#ifndef CELL_MAIN_H
#define CELL_MAIN_H

#include <QMainWindow>
#include <QStandardItemModel>


QT_BEGIN_NAMESPACE
namespace Ui { class Cell_Main; }
QT_END_NAMESPACE

class Cell_Main : public QMainWindow
{
    Q_OBJECT

public:
    Cell_Main(QWidget *parent = nullptr);
    ~Cell_Main();



private slots:


    void on_Upload_Button_clicked();

    void on_Delete_Button_clicked();

    void on_OpenFile_Button_clicked();

    void on_Search_Button_clicked();

private:

    void upDataFile();

    QString myStrDataPath;
    QStandardItemModel myModel;



    Ui::Cell_Main *ui;
};
#endif // CELL_MAIN_H
