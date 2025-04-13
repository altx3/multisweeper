#include "logger.hpp"

auto main() -> int
{
  Logger::init();
  Logger::log("Backend server starting...");
  return 0;
}
