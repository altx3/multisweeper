#pragma once

#include <string>

class Logger
{
public:
  static void init();
  static void log(const std::string &message);
  static void warn(const std::string &message);
};
