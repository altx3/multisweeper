#pragma once

#include <string>
#include <vector>

#include <nlohmann/json.hpp>

#include "types.hpp"

using json = nlohmann::json;

class Lobby
{
public:
  Lobby(const lobby_id_t &id, const player_id_t &host_id);

  const lobby_id_t &get_id() const;
  const player_id_t &get_host_id() const;

  void add_player(const player_id_t &player_id);
  void remove_player(const player_id_t &player_id);

  json get_state() const;

private:
  lobby_id_t id_;
  player_id_t host_id_;
  std::vector<player_id_t> player_ids_;
};
