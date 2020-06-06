#include <string>
#include <iomanip>
#include <sstream>
#include "format.h"
using std::string;

string Format::ElapsedTime(long seconds) { 
    
    int sec = seconds % 60;
    int min = seconds / 60 % 60;
    int hour = seconds / 60 / 60;

    std::ostringstream strstream;
    strstream << std::setfill('0') << std::setw(2) << hour << ":"
              << std::setfill('0') << std::setw(2) << min << ":"
              << std::setfill('0') << std::setw(2) << sec;
    
    return strstream.str();
    }