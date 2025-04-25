#include "uuid.hpp"
#include <iomanip>
#include <random>
#include <sstream>

auto generate_uuid_v4() -> std::string
{
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<uint32_t> dis(0, 0xFFFFFFFF);

  uint32_t data[4];
  for (int i = 0; i < 4; ++i)
  {
    data[i] = dis(gen);
  }

  // Set UUIDv4 version and variant bits
  data[1] = (data[1] & 0xFFFF0FFF) | 0x00004000;
  data[2] = (data[2] & 0x3FFFFFFF) | 0x80000000;

  std::stringstream ss;
  ss << std::hex << std::setfill('0');
  ss << std::setw(8) << data[0] << '-' << std::setw(4)
     << ((data[1] >> 16) & 0xFFFF) << '-' << std::setw(4) << (data[1] & 0xFFFF)
     << '-' << std::setw(4) << ((data[2] >> 16) & 0xFFFF) << '-' << std::setw(4)
     << (data[2] & 0xFFFF) << std::setw(8) << data[3];

  return ss.str();
}
