#include "calc.h"

#include <iostream>
#include <QString>
#include <math.h>

using namespace std;

Calc::Calc(QObject *parent) : QObject(parent)
{
 stopThread = false;

}

QString Calc::get_operator(QChar num){

    if (num == "1"){
        return "+";
    }
    else if (num == "2"){
        return  "-";
    }
    else if (num == "3"){
        return "*";
    }
    else if (num == "4"){
        return "/";
    }
    else if (num == "5"){
        return "(";
    }
    else if (num == "6"){
       return ")";
    }
    else if (num == "7"){
        return "^";
    }
    else if (num == "8"){
        return "|";
    }

    else {
        return "error";
    }

}

void Calc::startSearch(double number,unsigned long long startStep,unsigned long long endStep){




    for (unsigned long long counter = startStep; counter <= MAX_VALUE ; counter++) {

        if (counter == MAX_VALUE || stopThread) {

            break;
        }

        QString counterStr;
        int counterStrLenght;

        QString  expressionStr = "";
        int num = 0;

        QString operation;
        double result;

        //наша строка цифр
        counterStr = QString::number(counter);
        reverse(counterStr.begin() , counterStr.end());

        //длина для цикла
        counterStrLenght = counterStr.length();

        for (int j = 0; num < 10; ++j) {
            num++;

            if (num == 10){
                break;
            }

            if (num == 9){
                //набрали что то типа 1+2+(3+4)*5*(6-7)+8^9 и считаем
                expressionStr += QString::number(num);

                if (checkExpression(expressionStr)){ // здесь можно выполнить проверки со строкой


                    result = calculateExpression(expressionStr);

                    if (result == number) {
                      //правильный ответ получен
                      //выйти из потоков
                     // emit  signalSearchComplete(expressionStr);
                        stopThread = true;

                        QFile file(QDir::currentPath() + QDir::separator() + "result.txt");

                        if(file.open(QIODevice::WriteOnly)){

                          QTextStream stream(&file);
                          file.resize(0);
                          stream << expressionStr;
                          file.close();
                        }




                    } else {
                     //   emit signalExpressionReady(expressionStr);
                    }



                    break;
                }

            }

            if ( counterStrLenght < j){
                break;
            }

            operation = get_operator(counterStr[j]);

            if (operation == "error"){
                num--;
                continue;
            }

            expressionStr += QString::number(num);

            if (  operation == ")"){
                expressionStr += operation;
                j++;
                operation = get_operator(counterStr[j]);
                if (operation != "error"){
                    expressionStr += operation;
                }
            }
            else {
                expressionStr += operation;
            }
        }
    }



}



 double Calc::calculate(double val1 , double val2 , QString operation){

   // double result;

    if (operation == "+"){
        return val1 + val2;
    }
    else if (operation == "-"){
        return val1 - val2;
    }
    else if (operation == "*"){
        return val1 * val2;
    }
    else if (operation == "/"){
        return val1 / val2;
    }
    else if (operation == "^"){
        return pow(val1,val2);
    }
    else if (operation == "|"){
        QString concatenate;
        concatenate = QString::number(val1);
        concatenate += QString::number(val2);

        return concatenate.toDouble();
    }



}

double Calc::calculateExpressionNonBox(QString expresion){


    QString operation;

    QString exprStageSquare;
    QString exprStageConcatenate;
    QString exprStageMultDiv;
    QString exprStageMinPlus;


    double value1 = 0;
    double value2 = 0;
    double result = 0;

    int length;

    QString index_inc;
    int index;

   int lengthPrevStageStr = expresion.length();

    //степень
    for (int i = 0; i < lengthPrevStageStr;i++) {
        if (expresion[i] == "^"){

            if (exprStageSquare != ""){
                length = exprStageSquare.length();
                value1 =  getValue(exprStageSquare,length,0);
            }
            else {
                value1 = getValue(expresion,i,0);
            }


            value2 = getValue(expresion,i,1);

            index_inc = QString::number(value2);

            operation = expresion[i];
            result = calculate(value1,value2,operation);

            length = exprStageSquare.length();
            length--;

            while(exprStageSquare[length].isDigit() ||  exprStageSquare[length] == "." ){
                exprStageSquare.remove(length,1);
                length--;
                if(length < 0){
                    break;
                }
            }

            exprStageSquare += QString::number(result);
            i = i + index_inc.length();
            continue;
        }
        if (i != lengthPrevStageStr){
            exprStageSquare += expresion[i];
        }

    }

    //конкатенация
    lengthPrevStageStr = exprStageSquare.length();
    for (int j = 0; j < lengthPrevStageStr;j++) {

        if (exprStageSquare[j] == "|"){


            if (exprStageSquare != ""){
                length = exprStageConcatenate.length();
                value1 = getValue(exprStageConcatenate,length,0);
            }
            else {
                value1 = getValue(exprStageSquare,j,0);
            }

            value2 = getValue(exprStageSquare,j,1);
            index_inc = QString::number(value2);

            operation = exprStageSquare[j];
            result = calculate(value1,value2,operation);


            length = exprStageConcatenate.length();
            length--;

            while(exprStageConcatenate[length].isDigit() ||  exprStageConcatenate[length] == "." ){
                exprStageConcatenate.remove(length,1);
                length--;
                if(length < 0){
                    break;
                }
            }

            exprStageConcatenate += QString::number(result);
            j = j + index_inc.length();

            continue;
        }

        if (j != lengthPrevStageStr){
            exprStageConcatenate += exprStageSquare[j];
        }

    }

    //умножение деление
    lengthPrevStageStr = exprStageConcatenate.length();
    for (int g = 0; g < lengthPrevStageStr;g++) {

        if (exprStageConcatenate[g] == "*" || exprStageConcatenate[g] == "/" ){

            if (exprStageMultDiv != ""){
                length = exprStageMultDiv.length();
                value1 = getValue(exprStageMultDiv,length,0);
            }
            else {
                value1 = getValue(exprStageConcatenate,g,0);
            }

            value2 = getValue(exprStageConcatenate,g,1);
            index_inc = QString::number(value2);

            operation = exprStageConcatenate[g];
            result = calculate(value1,value2,operation);

            length = exprStageMultDiv.length();
            length--;

            while(exprStageMultDiv[length].isDigit() || exprStageMultDiv[length] == "."){
                exprStageMultDiv.remove(length,1);
                length--;
                if(length < 0){
                    break;
                }
            }

            exprStageMultDiv += QString::number(result);

            g = g + index_inc.length();
            continue;
        }
        if (g != lengthPrevStageStr){
            exprStageMultDiv += exprStageConcatenate[g];
        }


    }

    //минус плюс

    lengthPrevStageStr = exprStageMultDiv.length();
    for (int k = 0; k < lengthPrevStageStr;k++) {

        if(k == 13)
        {
            k = 13;
        }

        if (exprStageMultDiv[k] == "+" || exprStageMultDiv[k] == "-" ){

            if (exprStageMinPlus != ""){
                length = exprStageMinPlus.length();
                value1 = getValue(exprStageMinPlus,length,0);
            }
            else {
                value1 = getValue(exprStageMultDiv,k,0);
            }



            value2 = getValue(exprStageMultDiv,k,1);
            index_inc = QString::number(value2);


            operation = exprStageMultDiv[k];
            result = calculate(value1,value2,operation);

            length = exprStageMinPlus.length();
            length--;

       while(exprStageMinPlus[length].isDigit() ||
             exprStageMinPlus[length] == "." ||
             exprStageMinPlus[length] == "-")
       {

                if (exprStageMinPlus[length] == "-"){
                    index = length;
                    index--;

                    if (index > 0) {
                    if (exprStageMinPlus[index].isDigit() ||
                            exprStageMinPlus[index] == "(" ||
                            exprStageMinPlus[index] == ")"){
                        break;
                    }
                    }

                }

                exprStageMinPlus.remove(length,1);
                length--;
                if(length < 0){
                    break;
                }
            }


            exprStageMinPlus += QString::number(result);

            k = k + index_inc.length();
            continue;
        }
        if (k != lengthPrevStageStr){
            exprStageMinPlus += exprStageMultDiv[k];
        }
    }

    return exprStageMinPlus.toDouble();


}

double Calc::getValue(QString expression,int index,int navigation){
    int length;
    QString number;


    length = expression.length();
    if (navigation == 1){
        index++;
        while(expression[index].isDigit() || expression[index] == "." || expression[index] == "-")
        {

            if (expression[index] == "-"){

                if (index + 1 <= length) {
                if (expression[index + 1].isDigit() || expression[index + 1] == "(" || expression[index + 1] == ")"){
                    break;
                }
              }

            }

            number += expression[index];
            index++;

            if (index > length){
                break;
            }

        }
    } else if (navigation == 0) {
        index--;
        while(expression[index].isDigit() ||   expression[index] == "." || expression[index] == "-")
        {
            if (expression[index] == "-"){

                if (index - 1 > 0) {
                if (expression[index - 1].isDigit() || expression[index - 1] == "(" || expression[index - 1] == ")"){
                    break;
                }
               }

            }


            number += expression[index];
            index--;
            if (index < 0){
                break;
            }
        }
    }

    if (navigation == 0){
        std::reverse(number.begin(), number.end());

    }
    return number.toDouble();
}


double Calc::calculateExpression(QString expression){

    bool boxOpen = false;
    QString expressionBox;
    QString my_operator;
    QString nonBoxExpression;

    //посчитаем что там в скобках
    for (int i = 0; i < expression.length();i++) {

        if (expression[i] == "("){
            boxOpen = true;
            continue;
        }
        if (expression[i] == ")"){
            boxOpen = false;
            nonBoxExpression += QString::number(calculateExpressionNonBox(expressionBox)); //  считаем что в скобках
            continue;

        }

        if (boxOpen == true){

            expressionBox += expression[i];
        }
        else {
            if (i != expression.length()){
                nonBoxExpression += expression[i];
            }

        }


    }

    double result;
    result = calculateExpressionNonBox(nonBoxExpression);

    return result;

}

//проверяем положения скобок
bool Calc::checkExpression(QString expression){


    int length = expression.length();

    int openBoxCounter = 0;
    int closeBoxCounter = 0;
    bool error = false;

    for (int i = 0;length > i; i++) {

        if (expression[i] == "("){
            openBoxCounter++;

        }

        if (expression[i] == ")"){
            closeBoxCounter++;
        }

        if (expression[i] == "(" || expression[i] == ")"){

            if (expression[i + 1] == "|"){

                error = true;
            }
        }

//        if(i == length - 1){
//            int o;
//            o = 235;
//        }

    }

    if (openBoxCounter != closeBoxCounter){

        return false;
    }
    if (error == true){
        return false;
    }

    return true;

}











