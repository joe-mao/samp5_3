#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QTextCodec>

void MainWindow::iniModelFromStringList(QStringList & aFileContent)
{
    //从一个StringList获取数据，初始化数据模型
    int rowCnt = aFileContent.count();//文本行数,第一行时标题  rowCnt = 15

    theModel->setRowCount(rowCnt - 1);
    //设置表头，一个或多个空格，TAB等分隔符隔开的字符串，分解为StringList
    QString header = aFileContent.at(0);//第一行是表头
    QStringList headerList = header.split(QRegExp("\\s+"), QString::SkipEmptyParts);

    theModel->setHorizontalHeaderLabels(headerList);//设置表头文字
    //设置表格数据
    QStandardItem * aItem;
    QStringList tmpList;
    int j;
    for(int i = 1; i < rowCnt; ++i){
        QString aLineText = aFileContent.at(i);
        tmpList = aLineText.split(QRegExp("\\s+"), QString::SkipEmptyParts);
       // qDebug()<<tmpList;
        for(j = 0; j < FixedColumnCount - 1; ++j){
            //不包含最后一列
            aItem = new QStandardItem(tmpList.at(j));
            theModel->setItem(i-1, j, aItem);//为模型的某个行列位置设置Item
        }
        aItem = new QStandardItem(headerList.at(j));//最后一列
       // qDebug()<<"aItem"<<aItem->text();
        aItem->setCheckable(true);
        if(tmpList.at(j) == "0"){
            aItem->setCheckState(Qt::Unchecked);
        }else{
            aItem->setCheckState(Qt::Checked);
        }
        theModel->setItem(i-1, j, aItem);
    }

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


void MainWindow::on_actAppend_triggered()
{
    //在表格最后添加行
    QList<QStandardItem *> aItemList;//列表类
    QStandardItem * aItem;
    for(int i = 0; i < FixedColumnCount - 1; ++i){
        aItem = new QStandardItem("0");//创建Item
        aItemList<<aItem;//添加到列表
    }
    //获取最后一列的表头文字
    QString str = theModel->headerData(theModel->columnCount() - 1, Qt::Horizontal, Qt::DisplayRole).toString();
    aItem = new QStandardItem(str);//创建和最后一列标题一样文本的item
    aItem->setCheckable(true);
    aItemList<<aItem;//添加到列表

    theModel->insertRow(theModel->rowCount(), aItemList);//插入一行
    QModelIndex curIndex = theModel->index(theModel->rowCount() - 1, 0);
    theSelection->clearSelection();
    theSelection->setCurrentIndex(curIndex, QItemSelectionModel::Select);

}

void MainWindow::on_actDelete_triggered()
{
    //删除行
    QModelIndex curIndex = theSelection->currentIndex();//获取模型索引
    if(curIndex.row() == theModel->rowCount() - 1){
        theModel->removeRow(curIndex.row());//删除最后一行
    }else{
        qDebug()<<curIndex.row();
        theModel->removeRow(curIndex.row());//删除一行，并重新设置当前行选择
        theSelection->setCurrentIndex(curIndex, QItemSelectionModel::Select);
    }
}
