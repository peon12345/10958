#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <calc.h>
#include <queue>
#include <stack>
#include <future>
#include <math.h>

#include <thread>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_runButton_clicked();
   void updateTextEdit(QString expression);
   void setResult(QString expression);



private:
    Calc *calc;
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
