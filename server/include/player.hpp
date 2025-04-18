#pragma once

#include <string>

#include "types.hpp"

class Player
{
public:
  Player(const player_id_t &id);

  const player_id_t &get_id() const;
  const lobby_id_t &get_lobby_id() const;

  void set_lobby_id(const lobby_id_t &lobby_id);
  void leave_lobby();

private:
  player_id_t id_;
  lobby_id_t lobby_id_;
};
