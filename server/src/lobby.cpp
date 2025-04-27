#include "lobby.hpp"
#include "logger.hpp"

#include <utility>

Lobby::Lobby(lobby_id_t id, const player_id_t &host_id)
    : id_(std::move(id)), host_id_(host_id)
{
  player_ids_.push_back(host_id);
}

auto Lobby::get_id() const -> const lobby_id_t &
{
  return id_;
}

auto Lobby::get_host_id() const -> const player_id_t &
{
  return host_id_;
}

void Lobby::add_player(const player_id_t &player_id)
{
  Logger::log("adding " + static_cast<std::string>(player_id));

  // Check if player_id already exists in player_ids_
  if (std::find(player_ids_.begin(), player_ids_.end(), player_id) !=
      player_ids_.end())
  {
    Logger::warn("Player " + static_cast<std::string>(player_id) +
                 " already in lobby, skipping");
    return;
  }

  player_ids_.push_back(player_id);
}

void Lobby::remove_player(const player_id_t &player_id)
{
  player_ids_.erase(
    std::remove(player_ids_.begin(), player_ids_.end(), player_id),
    player_ids_.end());
}

auto Lobby::get_state() const -> json
{
  return {{"lobby_id", id_}, {"host_id", host_id_}, {"players", player_ids_}};
}
