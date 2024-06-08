#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <iostream>

#define LOGINFO(message) std::cout << "[info] " << message << std::endl; 
#define LOGERROR(message) std::cout << "[error] " << message << std::endl; 
#define LOGDEBUG(message) std::cout << "[debug] " << message << std::endl; 
#define LOGWARN(message) std::cout << "[warn] " << message << std::endl; 

#endif 