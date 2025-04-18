#include "lobby.hpp"

Lobby::Lobby(const lobby_id_t &id, const player_id_t &host_id)
    : id_(id), host_id_(host_id)
{
  player_ids_.push_back(host_id);
}

const lobby_id_t &Lobby::get_id() const
{
  return id_;
}

const player_id_t &Lobby::get_host_id() const
{
  return host_id_;
}

void Lobby::add_player(const player_id_t &player_id)
{
  player_ids_.push_back(player_id);
}

void Lobby::remove_player(const player_id_t &player_id)
{
  player_ids_.erase(
    std::remove(player_ids_.begin(), player_ids_.end(), player_id),
    player_ids_.end());
}

json Lobby::get_state() const
{
  return {{"lobby_id", id_}, {"host_id", host_id_}, {"players", player_ids_}};
}
