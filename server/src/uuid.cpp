#include "uuid.hpp"
#include <array>
#include <iomanip>
#include <random>
#include <sstream>

constexpr uint32_t MAX_INT = 0xFFFFFFFF;
constexpr uint32_t VERSION_MASK = 0xFFFF0FFF;
constexpr uint32_t VERSION_4_BITS = 0x00004000;
constexpr uint32_t VARIANT_MASK = 0x3FFFFFFF;
constexpr uint32_t VARIANT_RFC_BITS = 0x80000000;
constexpr int UUID_SEGMENT_32BIT_WIDTH = 8;
constexpr int UUID_SEGMENT_16BIT_WIDTH = 4;
constexpr int BITS_PER_HALF_UINT32 = 16;
constexpr uint32_t UINT16_MASK = 0xFFFF;

auto generate_uuid_v4() -> std::string
{
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<uint32_t> dis(0, MAX_INT);

  std::array<uint32_t, 4> data = {};
  for (uint32_t &i : data)
  {
    i = dis(gen);
  }

  // Set UUIDv4 version and variant bits
  data[1] = (data[1] & VERSION_MASK) | VERSION_4_BITS;
  data[2] = (data[2] & VARIANT_MASK) | VARIANT_RFC_BITS;

  std::stringstream ss;
  ss << std::hex << std::setfill('0');
  ss << std::setw(UUID_SEGMENT_32BIT_WIDTH) << data[0] << '-'
     << std::setw(UUID_SEGMENT_16BIT_WIDTH)
     << ((data[1] >> BITS_PER_HALF_UINT32) & UINT16_MASK) << '-'
     << std::setw(UUID_SEGMENT_16BIT_WIDTH) << (data[1] & UINT16_MASK) << '-'
     << std::setw(UUID_SEGMENT_16BIT_WIDTH)
     << ((data[2] >> BITS_PER_HALF_UINT32) & UINT16_MASK) << '-'
     << std::setw(UUID_SEGMENT_16BIT_WIDTH) << (data[2] & UINT16_MASK)
     << std::setw(UUID_SEGMENT_32BIT_WIDTH) << data[3];

  return ss.str();
}
