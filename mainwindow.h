#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QLabel>
#include <QTableView>
#include <QStandardItemModel>
#include <QTableView>
#include <QItemSelectionModel>
#include <QFileDialog>
#include <QModelIndexList>

namespace Ui {
class MainWindow;
}


#define FixedColumnCount 6 //文件固定6列

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    QLabel * LabCurFile;//当前文件
    QLabel * LabCellPos;//当前单元格行列号
    QLabel * LabCellText;//当前单元格内容
    QStandardItemModel * theModel;//数据模型
    QItemSelectionModel * theSelection;//选择模型
    void iniModelFromStringList(QStringList & aFileContent);//从StringList初始化数据模型

private slots:
    //当前选择单元格发生变化
    void on_currentChanged(const QModelIndex & current, const QModelIndex & previous);




    void on_actOpen_triggered();

    void on_actAppend_triggered();

    void on_actDelete_triggered();

    void on_actAlignCenter_triggered();

    void on_actAlignLeft_triggered();

    void on_actAlignRight_triggered();

    void on_actFontBold_toggled(bool arg1);

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
