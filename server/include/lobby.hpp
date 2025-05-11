#pragma once

#include <string>
#include <vector>

#include <nlohmann/json.hpp>

#include "types.hpp"

using json = nlohmann::json;

class Lobby
{
public:
  Lobby(lobby_id_t id, const player_id_t &host_id);

  [[nodiscard]] auto get_id() const -> const lobby_id_t &;
  [[nodiscard]] auto get_host_id() const -> const player_id_t &;
  [[nodiscard]] auto get_players() const -> const std::vector<player_id_t> &;
  [[nodiscard]] auto is_player_in_lobby(const player_id_t &player_id) const
    -> bool;

  void add_player(const player_id_t &player_id);
  void remove_player(const player_id_t &player_id);

  [[nodiscard]] auto get_state() const -> json;

private:
  lobby_id_t id_;
  player_id_t host_id_;
  std::vector<player_id_t> player_ids_;
};
