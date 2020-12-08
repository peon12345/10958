#ifndef CALC_H
#define CALC_H
#include <QObject>
#include <QString>
#include <iostream>
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <mutex>



class Calc : public QObject
{

    Q_OBJECT
public:
    Calc (QObject *parent = nullptr);
    bool stopThread;

    static constexpr unsigned long long MAX_VALUE = 18446744073709551615;
    void startSearch(double number,unsigned long long startStep,unsigned long long endStep);


signals:
    void signalExpressionReady(QString expression);
    void signalSearchComplete(QString expression);

private:


    //работа с выражениями
    bool checkExpression(QString expression);
    double calculateExpression(QString expression);
    double calculate(double val1 ,double val2 , QString operation);
    double getValue(QString expression,int index,int navigation);
    double calculateExpressionNonBox(QString nonBoxExpression);
    QString get_operator(QChar num);


};

#endif // CALC_H
