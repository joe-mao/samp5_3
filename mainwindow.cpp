#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QTextCodec>

void MainWindow::iniModelFromStringList(QStringList & aFileContent)
{
    //从一个StringList获取数据，初始化数据模型
    int rowCnt = aFileContent.count();//文本行数,第一行时标题
    theModel->setRowCount(rowCnt - 1);
}

void MainWindow::on_currentChanged(const QModelIndex &current, const QModelIndex &previous)
{
    //选择单元格变化时的响应
    if(current.isValid()){
        LabCellPos->setText(QString::asprintf("当前单元格：%d 行, %d 列", current.row(), current.column()));
        QStandardItem * aItem = theModel->itemFromIndex(current);
        this->LabCellText->setText("单元格内容：" + aItem->text());
        QFont font = aItem->font();
        this->ui->actFontBold->font();
        this->ui->actFontBold->setChecked(font.bold());
    }
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setCentralWidget(this->ui->splitter);
    theModel = new QStandardItemModel(2, FixedColumnCount, this);//数据模型
    theSelection = new QItemSelectionModel(theModel);//选择模型
    connect(theSelection, SIGNAL(currentChanged(QModelIndex, QModelIndex)), this, SLOT(on_currentChanged(QModelIndex, QModelIndex)));
    this->ui->tableView->setModel(theModel);
    this->ui->tableView->setSelectionModel(theSelection);
    this->ui->tableView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    this->ui->tableView->setSelectionBehavior(QAbstractItemView::SelectItems);

    LabCurFile = new QLabel(this);
    LabCellPos = new QLabel(this);
    LabCellText = new QLabel(this);
    this->ui->statusBar->addWidget(LabCurFile);
    this->ui->statusBar->addWidget(LabCellPos);
    this->ui->statusBar->addWidget(LabCellText);

    qDebug()<<QCoreApplication::applicationDirPath();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actOpen_triggered()
{

    QTextCodec * codec = QTextCodec::codecForName("utf-8");//1
    //打开文件
    QString curPath = QCoreApplication::applicationDirPath();
    QString aFileName = QFileDialog::getOpenFileName(this, "打开一个文件", curPath, "#数据文件(*txt);;所有文件(*.*)");
    if(aFileName.isEmpty()){
        return;
    }
    QStringList fFileContent;
    QFile aFile(aFileName);
    //打开文件
    if(aFile.open(QIODevice::ReadOnly | QIODevice::Text)){
        //QTextStream aStream(&aFile);//用文本流读取文件
        this->ui->plainTextEdit->clear();
        while(!aFile.atEnd()){
            QString str = codec->toUnicode(aFile.readLine());
            this->ui->plainTextEdit->appendPlainText(str);
            fFileContent.append(str);
        }
        aFile.close();
        this->LabCurFile->setText("当前文件："+aFileName);//状态栏显示
        iniModelFromStringList(fFileContent);//初始化数据模型
    }
}

