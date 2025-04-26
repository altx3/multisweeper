#pragma once

#include <map>
#include <string>

#include "lobby.hpp"
class LobbyManager
{
public:
  auto create_lobby(const player_id_t &host_id) -> lobby_id_t;
  auto get_lobby(const lobby_id_t &lobby_id) -> Lobby *;
  auto get_lobbies() -> const std::map<lobby_id_t, Lobby> &;
  auto join_lobby(const lobby_id_t &lobby_id, const player_id_t &player_id)
    -> bool;
  void leave_lobby(const lobby_id_t &lobby_id, const player_id_t &player_id);

private:
  std::map<lobby_id_t, Lobby> lobbies_;
};
