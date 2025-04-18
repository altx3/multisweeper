#pragma once

#include <random>
#include <string>

constexpr int MIN_ID = 1000;
constexpr int MAX_ID = 9999;

template <typename IdType>
auto generate_random_id(const std::string &prefix) -> IdType
{
  static std::random_device rd;
  static std::mt19937 gen(rd());
  static std::uniform_int_distribution<> dis(MIN_ID, MAX_ID);

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

  auto empty() const -> decltype(std::declval<const T &>().empty(), bool())
  {
    return value.empty();
  }
};

using lobby_id_t = StrongTypedef<std::string, struct lobby_id_tag>;
using player_id_t = StrongTypedef<std::string, struct player_id_tag>;
