#pragma once

#include <map>
#include <string>

#include "lobby.hpp"
class LobbyManager
{
public:
  lobby_id_t create_lobby(const player_id_t &host_id);
  bool join_lobby(const lobby_id_t &lobby_id, const player_id_t &player_id);
  void leave_lobby(const lobby_id_t &lobby_id, const player_id_t &player_id);
  Lobby *get_lobby(const lobby_id_t &lobby_id);

private:
  std::map<lobby_id_t, Lobby> lobbies_;
};
