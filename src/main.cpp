#include <iostream>
#include "logger.h" //including our HEADER FILE


using namespace std;
using namespace Std;




int sum(int a, int b){
    return a+b;
}

int main(){
    int s;
    //instanciate your objects HERE
    logger& logger1 = logger::NEW();

    //Configure your settings HERE
    logger1.config(logger::outputMode::both, logger::logLevels::INFO,logger::outputFormat::ID);
    

    std::cout << "Followings are all the log enteries that a user made." << std::endl;
    //TEST .log() with different data types
    logger1.log(to_string(s = sum(INT32_MAX, INT32_MAX)) + " The result of the sum function doesn't work for INT_MAX", logger::logLevels::ERROR);
    logger1.log(12.23, logger::logLevels::ERROR);
    logger1.log('R', logger::logLevels::DEBUG);
    logger1.log("msg", logger::logLevels::DEBUG);
    logger1.log(10, logger::logLevels::WARNING);
    logger1.log(10, logger::logLevels::DEBUG);
    logger1.log(10, logger::logLevels::DEBUG);
    logger1.log(10, logger::logLevels::INFO);
    logger1.log(10, logger::logLevels::ERROR);
    logger1.log(10, logger::logLevels::ERROR);

    //TEST .getErrors()
    std::cout << "Followings are all the active errors in the current thread." << std::endl;
    logger1.getErrors();
}