#include "lobby_manager.hpp"
#include "logger.hpp"

auto LobbyManager::create_lobby(const player_id_t &host_id) -> lobby_id_t
{
  auto lobby_id = generate_random_id<lobby_id_t>("lobby_");
  lobbies_.emplace(lobby_id, Lobby(lobby_id, host_id));
  return lobby_id;
}

auto LobbyManager::get_lobby(const lobby_id_t &lobby_id) -> Lobby *
{
  auto it = lobbies_.find(lobby_id);
  if (it != lobbies_.end())
  {
    return &it->second;
  }
  return nullptr;
}

auto LobbyManager::get_lobbies() -> const std::map<lobby_id_t, Lobby> &
{
  return lobbies_;
}

auto LobbyManager::join_lobby(const lobby_id_t &lobby_id,
                              const player_id_t &player_id) -> bool
{
  Lobby *lobby = get_lobby(lobby_id);
  if (lobby != nullptr)
  {
    lobby->add_player(player_id);
    return true;
  }
  return false;
}

void LobbyManager::leave_lobby(const lobby_id_t &lobby_id,
                               const player_id_t &player_id)
{
  Lobby *lobby = get_lobby(lobby_id);
  if (lobby != nullptr)
  {
    lobby->remove_player(player_id);
    if (lobby->get_state()["players"].empty())
    {
      lobbies_.erase(lobby_id);
      Logger::log("deleting lobby: " + static_cast<std::string>(lobby_id));
    }
  }
}
