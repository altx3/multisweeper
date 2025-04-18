#include "lobby_manager.hpp"

auto LobbyManager::create_lobby(const player_id_t &host_id) -> lobby_id_t
{
  lobby_id_t lobby_id = generate_random_id<lobby_id_t>("lobby_");
  lobbies_.emplace(lobby_id, Lobby(lobby_id, host_id));
  return lobby_id;
}

auto LobbyManager::join_lobby(const lobby_id_t &lobby_id,
                              const player_id_t &player_id) -> bool
{
  auto it = lobbies_.find(lobby_id);
  if (it != lobbies_.end())
  {
    it->second.add_player(player_id);
    return true;
  }
  return false;
}

void LobbyManager::leave_lobby(const lobby_id_t &lobby_id,
                               const player_id_t &player_id)
{
  auto it = lobbies_.find(lobby_id);
  if (it != lobbies_.end())
  {
    it->second.remove_player(player_id);
    if (it->second.get_state()["players"].empty())
    {
      lobbies_.erase(it);
    }
  }
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
