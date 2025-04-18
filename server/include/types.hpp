#pragma once

#include <string>

template <typename T, typename Tag> struct StrongTypedef
{
  T value;

  explicit StrongTypedef(const T &v) : value(v) {}
  StrongTypedef() = default;

  bool operator==(const StrongTypedef &other) const
  {
    return value == other.value;
  }
  bool operator!=(const StrongTypedef &other) const
  {
    return value != other.value;
  }
  bool operator<(const StrongTypedef &other) const
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
