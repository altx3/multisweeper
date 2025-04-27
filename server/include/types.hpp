#pragma once

#include <random>
#include <string>
#include <utility>

#include "uuid.hpp"

template <typename IdType>
auto generate_random_id(const std::string &prefix) -> IdType
{
  return IdType(prefix + generate_uuid_v4());
}

template <typename T, typename Tag> struct StrongTypedef
{
  T value;

  explicit StrongTypedef(T v) : value(std::move(v)) {}
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

  [[nodiscard]] auto empty() const -> bool
  {
    return value.empty();
  }
};

using lobby_id_t = StrongTypedef<std::string, struct lobby_id_tag>;
using player_id_t = StrongTypedef<std::string, struct player_id_tag>;
