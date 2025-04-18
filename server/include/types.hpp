#pragma once

#include <random>
#include <string>

template <typename IdType>
auto generate_random_id(const std::string &prefix) -> IdType
{
  static std::random_device rd;
  static std::mt19937 gen(rd());
  static std::uniform_int_distribution<> dis(1000, 9999);

  return IdType(prefix + std::to_string(dis(gen)));
}

template <typename T, typename Tag> struct StrongTypedef
{
  T value;

  explicit StrongTypedef(const T &v) : value(v) {}
  StrongTypedef() = default;

  auto operator==(const StrongTypedef &other) const -> bool
  {
    return value == other.value;
  }
  auto operator!=(const StrongTypedef &other) const -> bool
  {
    return value != other.value;
  }
  auto operator<(const StrongTypedef &other) const -> bool
  {
    return value < other.value;
  }

  operator const T &() const
  {
    return value;
  }
};

using lobby_id_t = StrongTypedef<std::string, struct lobby_id_tag>;
using player_id_t = StrongTypedef<std::string, struct player_id_tag>;
