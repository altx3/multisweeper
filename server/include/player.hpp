#pragma once

#include <string>

#include "types.hpp"

class Player
{
public:
  Player(player_id_t id);

  [[nodiscard]] auto get_id() const -> const player_id_t &;
  [[nodiscard]] auto get_lobby_id() const -> const lobby_id_t &;

  void set_lobby_id(const lobby_id_t &lobby_id);
  void leave_lobby();

private:
  player_id_t id_;
  lobby_id_t lobby_id_;
};
