#include <iostream>
#include "logger.h" //including our HEADER FILE


using namespace std;
using namespace nokialog;




int sum(int a, int b){
    return a+b;
}

int main(){
    int num = 100; 
    int num1 = 200;
    int s;
    //instanciate your objects HERE
    Logger& logger = Logger::NEW();

    //Configure your settings HERE
    logger.config(Logger::logOUT::console, Logger::logLevels::DEBUG,Logger::format::ID);
    
    //TEST .log() with different data types
    logger.log(s = sum(num, num1), Logger::logLevels::INFO);
    logger.log(12.23, Logger::logLevels::INFO);
    logger.log('R', Logger::logLevels::DEBUG);
    logger.log("msg", Logger::logLevels::ERROR);
    logger.log(10, Logger::logLevels::WARNING);
    logger.log(10, Logger::logLevels::DEBUG);
    logger.log(10, Logger::logLevels::ERROR);
    logger.log(10, Logger::logLevels::ERROR);
    logger.log(10, Logger::logLevels::ERROR);
    logger.log(10, Logger::logLevels::ERROR);


    cout << "BEFOR: " << endl;

    //TEST .getErrors()
    logger.getErrors();


    //TEST .clear(unique ID)
    logger.clear(-1);

    cout << "AFTER" << endl << endl << endl << endl;
    logger.getErrors();
}