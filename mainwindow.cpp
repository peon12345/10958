#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    Calc *calc = new Calc();


    //     connect(calc , SIGNAL( signalExpressionReady(QString) ), this , SLOT( updateTextEdit(QString)) );
    //     connect(calc , SIGNAL( SearchComplete(QString )), this , SLOT( setResult(QString)) );




    ui->groupBoxProcess->hide();
    ui->groupBoxMain->show();
}

MainWindow::~MainWindow()
{
    delete calc;
    delete ui;
}


void MainWindow::on_runButton_clicked()
{


    ui->groupBoxMain->hide();
    ui->groupBoxProcess->show();


    std::queue<std::future<void>> queueTask;


    int numThreads = std::thread::hardware_concurrency() * 2;

    if(numThreads == 0){
        numThreads = 4;
    }

    unsigned long long stepInterval = Calc::MAX_VALUE / numThreads;


    int i = 0;
    unsigned long long begin = 0;
    unsigned long long end = 0;

// разбиваем число на диапазоны , запусукаем таски
    while(i <= numThreads){
        end += stepInterval;
        queueTask.push(std::async(std::launch::async,&Calc::startSearch,calc,ui->lineEditGetNum->text().toDouble(),begin,end));
        begin += stepInterval;
        i++;
    }

    queueTask.push(std::async(std::launch::async,&Calc::startSearch,calc,ui->lineEditGetNum->text().toDouble(),0,0));

}


void MainWindow::updateTextEdit(QString expression){

    ui->textEditProcess->append(expression);
}



void MainWindow::setResult(QString expression){
    ui->groupBoxProcess->hide();
    ui->groupBoxMain->show();
    ui->textEditResult->setText(expression);
}




