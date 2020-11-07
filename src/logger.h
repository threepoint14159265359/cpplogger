/**
 *  @uthor: Hussain Izhar  
 *  Purpose: NOKIA TASK
 */




#pragma once //only compilation

#include <iostream>
#include <string> 
#include <sstream>
#include <fstream>
#include <vector>
#include <map>
#include <ctime> 
#include <thread>

using namespace std;

namespace nokialog
{

class Logger{
public: 

    enum class logLevels{
        DEBUG, INFO, WARNING, ERROR,
    };

    enum class logOUT{
        file,console,both,
    };

    enum class format{
        DEFAULT,DATE_TIME,ID,
    };

    string TO_STRING(const logLevels ilevel){
        string result = "NONE";
        const static map<logLevels, string> logLevelStrings{
            {logLevels::DEBUG, "DEBUG"},
            {logLevels::INFO, "INFO" },
            {logLevels::ERROR, "ERROR"},
            {logLevels::WARNING, "WARNING" },
        };

        auto itr = logLevelStrings.find(ilevel); //find the input string in the map
        if(itr != logLevelStrings.end()){
            result = itr->second; //will return the second (string part) of the map
        } 
        return result;  
    }
    ~Logger(){}


    /*
    * Configurations: 
    * 1-> User can change four levels log levels, namely DEBUG INFO WARNING ERROR  
    * 2-> User can set the ouput into a console winodow as well as file, and both
    * 3-> User can set the output format from a very simple one to a more fancy one
    * 
    * DEFAULT VALUES: 
    * levels: WARNING 
    * format: DEFAULT -> which means the simplest output formate
    * ouput mode: both console and file
    */

    void config(format iformat){
        if(iformat == format::DATE_TIME){
            outFormate = format::DATE_TIME;
        }
        if(iformat == format::ID){
            outFormate = format::ID;
        }
        if(iformat == format::DEFAULT){
            outFormate = format::DEFAULT;
        }
    }
    
    void config(logOUT iout){
        if(iout == logOUT::console){
            outPlatform = logOUT::console;
        }
        if(iout == logOUT::file){
            outPlatform = logOUT::file;
        }
        if(iout == logOUT::both){
            outPlatform = logOUT::both;
        }
    }
    void config(logLevels ilevel){
        userChoiceLogLevel = ilevel;
    }
    void config(logOUT iout, logLevels ilevel){
        config(iout);
        config(ilevel);
    }
    void config(logOUT iout, format iform){
        config(iout);
        config(iform);
    }
    void config(logLevels ilevel, format iform){
        config(ilevel);
        config(iform);
    }
    void config(logOUT iout,logLevels ilevel, format iform){
        config(iout);
        config(ilevel);
        config(iform);
    }

    /**
     * Our constructor is private, instead we created a method which is looks more like C++ syntax of "new"
     **/
    static Logger& NEW(){
        static Logger newlog; 
        return newlog;
    }

    /**
     * void log() function
     * ->creates a log into a file and into a console depending on the config settings 
     */
    void openFile(){
        if (outFile.is_open()) {
            return;
        }
        outFile.open(outFileName, ios_base::app | ios::out);
        IS_OPEN = outFile.is_open();
        if (!IS_OPEN){
            throw std::runtime_error("Couldn't open the log file " + outFileName + ".");
        }
    }
    void closeFile(){
        if(outFile.is_open()){
            outFile.close();
        }
    }    
    string wrap(string s){
        string prefix = "[";
        string suffix = "] ";
        return prefix + s + suffix; 
    }

    string outputFormating(const string message,logLevels ilevel){
        string OUTPUT;
        if(outFormate == format::ID){
            OUTPUT = wrap(threadID()) +   wrap(CURRENT_TIME())  + wrap(message) + wrap(TO_STRING(ilevel)) + "\n";
        }
        if(outFormate == format::DATE_TIME){
            OUTPUT = wrap(CURRENT_TIME())  + wrap(message) + wrap(TO_STRING(ilevel)) + "\n";
        }
        if(outFormate == format::DEFAULT){
            OUTPUT =  wrap(message)  + wrap(TO_STRING(ilevel)) + "\n";
        } 
        return OUTPUT;
    }

    void log(const string& message, logLevels ilevel){
        if(userChoiceLogLevel > ilevel){
            return; 
        }
        string endl = "\n",OUTPUT;
        OUTPUT = outputFormating(message, ilevel); 
        if(ilevel == logLevels::ERROR){
            errors.push_back(make_pair(ui++, OUTPUT));
        }
        if(outPlatform == logOUT::console){
            cout << OUTPUT;
        }else if(outPlatform == logOUT::file){
            openFile();
            outFile << OUTPUT;
            closeFile();
        }else{
            cout << OUTPUT;
            openFile();
            outFile << OUTPUT;
            closeFile();
        }
    }


    /**
     *  Template funtion of the log() function: generic inputs
     */
    template<class T>
    void log(const T& t, const logLevels llevels){
        string data;
        stringstream ss; 
        ss << t; 
        data = ss.str();
        log(data, llevels); 
    }

    /**
     * getErrors() prints out current running errors on to the console
     **/
    void getErrors(){
        for(auto it = 0;  it < errors.size(); it++){
            cout <<errors.at(it).second ;//<< endl;
        }
    }

    /*
     * clear() deletes an error thread at any specified id in the log thread | linear search
     **/

    void clear(int id){
        for(int it = 0;  it < errors.size(); it++){
            if(id == errors.at(it).first){
                errors.erase(errors.begin()+it);
            }
        }
    }


private:
    logOUT outPlatform = logOUT::both;//ouput mode 
    format outFormate = format::DEFAULT; //ouput Format
    logLevels userChoiceLogLevel = logLevels::WARNING; //log levels
    string outFileName = "log.txt"; //active file name
    fstream outFile; //fstream fileObject
    bool IS_OPEN = false; //file check 
    vector<pair<int, string>> errors; //ERORRS container 
    int ui = -1; //unique IDiterator of the ERORRS container
    /*
    /*current date and time as string
     */
    static string CURRENT_TIME(){ 
        time_t now = time(0);
        struct tm tstruct;
        char buf[80];
        tstruct = *localtime(&now);
        strftime(buf, sizeof(buf), "%Y-%m-%d %X", &tstruct);
        return buf; 
    }
    /*
    *Curret thread ID 
    */
    static string threadID(){ // returns the id of the current log as a string 
        stringstream ss; 
        ss << this_thread::get_id();
        return ss.str();
    }
    Logger(){} 

}; //end class Logger


} //end namespace nokialog
