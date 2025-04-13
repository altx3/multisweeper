#include "logger.hpp"
#include <iostream>

void Logger::init()
{
  std::cout << "Logger initialized" << std::endl;
}

void Logger::log(const std::string &message)
{
  std::cout << "[LOG] " << message << std::endl;
}
