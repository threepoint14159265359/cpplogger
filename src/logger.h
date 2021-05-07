/**
 *  @uthor: Hussain Izhar  
 *  Dated: October 2020
 */
#pragma once

#include <iostream>
#include <string> 
#include <sstream>
#include <fstream>
#include <vector>
#include <map>
#include <ctime> 
#include <thread>


using namespace std; //standard

namespace Std //my standard
{

	

class logger{
public: 

/////////////////////////////////////////////////////////////////////////////
//Log Levels, Log output mode, and Log Format
////////////////////////////////
    enum class logLevels{
        DEBUG, INFO, WARNING, ERROR,
    };

    enum class outputMode{
        file,console,both,
    };
	
    enum class outputFormat{
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
        auto itr = logLevelStrings.find(ilevel); 
        if(itr != logLevelStrings.end()){
            result = itr->second;
        } 
        return result;  
    }
    ~logger(){} 



/////////////////////////////////////////////////////////////////////////////
//Configurations
////////////////////////////////
    void config(const outputFormat iformat){
        if(iformat == outputFormat::DATE_TIME){
            format = outputFormat::DATE_TIME;
        }
        if(iformat == outputFormat::ID){
            format = outputFormat::ID;
        }
        if(iformat == outputFormat::DEFAULT){
            format = outputFormat::DEFAULT;
        }
    }
    
    void config(const outputMode iout){
        if(iout == outputMode::console){
            mode = outputMode::console;
        }
        if(iout == outputMode::file){
            mode = outputMode::file;
        }
        if(iout == outputMode::both){
            mode = outputMode::both;
        }
    }
    void config(const logLevels ilevel){
        userLogLevel = ilevel;
    }
    void config(const outputMode iout, const logLevels ilevel){
        config(iout);
        config(ilevel);
    }
    void config(const outputMode iout, const outputFormat iform){
        config(iout);
        config(iform);
    }
    void config(const logLevels ilevel,const outputFormat iform){
        config(ilevel);
        config(iform);
    }
    void config(const outputMode iout, const logLevels ilevel,const outputFormat iform){
        config(iout);
        config(ilevel);
        config(iform);
    }



/////////////////////////////////////////////////////////////////////////////
//Constructor Replacement Function
////////////////////////////////    
    static logger& NEW(){
        static logger newlog; 
        return newlog;
    }


/////////////////////////////////////////////////////////////////////////////
//Following functions are only to be used in the log() function
////////////////////////////////
	
    void openFile(){
        if (file.is_open()) {
            return;
        }
        file.open(filename, ios_base::app | ios::out);
        isOpen = file.is_open();
        if (!isOpen){
            throw std::runtime_error("Couldn't open the log file " + filename + ".");
        }
    }
    void closeFile(){
        if(file.is_open()){
            file.close();
        }
    }    
    
    string wrap(string s){
        string prefix = "[";
        string suffix = "] ";
        return prefix + s + suffix; 
    }

    string outputFormating(const string message,logLevels ilevel){
        string OUTPUT;
        if(format == outputFormat::ID){
            OUTPUT = wrap(threadID()) +   wrap(Time())  + wrap(message) + wrap(TO_STRING(ilevel)) + "\n";
        }
        if(format == outputFormat::DATE_TIME){
            OUTPUT = wrap(Time())  + wrap(message) + wrap(TO_STRING(ilevel)) + "\n";
        }
        if(format == outputFormat::DEFAULT){
            OUTPUT =  wrap(message)  + wrap(TO_STRING(ilevel)) + "\n";
        } 
        return OUTPUT;
    }

    void log(const string& message, logLevels ilevel){
        if(userLogLevel > ilevel){
            return; 
        }
        string endl = "\n",OUTPUT;
        OUTPUT = outputFormating(message, ilevel); 
        if(ilevel == logLevels::ERROR){
            errors.push_back(make_pair(uId++, OUTPUT));
        }
        if(mode == outputMode::console){
            cout << OUTPUT;
        }else if(mode == outputMode::file){
            openFile();
            file << OUTPUT;
            closeFile();
        }else{
            cout << OUTPUT;
            openFile();
            file << OUTPUT;
            closeFile();
        }
    }

    template<class T>
    void log(const T& t, const logLevels llevels){
        string data;
        stringstream ss; 
        ss << t; 
        data = ss.str();
        log(data, llevels); 
    }


/////////////////////////////////////////////////////////////////////////////
//Get All the Errors in the current thread
////////////////////////////////  
    void getErrors(){
        for(auto it = 0;  it < errors.size(); it++){
            cout <<errors.at(it).second ;//<< endl;
        }
    }



private:
    outputMode mode = outputMode::both;
    outputFormat format = outputFormat::DEFAULT; 
    logLevels userLogLevel = logLevels::WARNING; 
    string filename = "log.txt"; 
    fstream file; 
    bool isOpen = false;  
    vector<pair<int, string>> errors; 
    int uId = -1; //unique ID .. You can search for a specific error in the veroct errors
    static string Time(){ 
        time_t now = time(0);
        struct tm tstruct;
        char buf[80];
        tstruct = *localtime(&now); 
        strftime(buf, sizeof(buf), "%Y-%m-%d %X", &tstruct);
        return buf; 
    }

    //current threads ID
    static string threadID(){ 
        stringstream ss; 
        ss << this_thread::get_id();
        return ss.str();
    }
    logger(){} 

}; //end class logger


} //end namespace Std;
