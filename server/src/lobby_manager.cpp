#include <random>

#include "lobby_manager.hpp"

lobby_id_t generate_random_id(const std::string &prefix)
{
  static std::random_device rd;
  static std::mt19937 gen(rd());
  static std::uniform_int_distribution<> dis(1000, 9999);

  lobby_id_t lobby_id(prefix + std::to_string(dis(gen)));
  return lobby_id;
}

lobby_id_t LobbyManager::create_lobby(const player_id_t &host_id)
{
  lobby_id_t lobby_id = generate_random_id("lobby_");
  lobbies_.emplace(lobby_id, Lobby(lobby_id, host_id));
  return lobby_id;
}

bool LobbyManager::join_lobby(const lobby_id_t &lobby_id,
                              const player_id_t &player_id)
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

Lobby *LobbyManager::get_lobby(const lobby_id_t &lobby_id)
{
  auto it = lobbies_.find(lobby_id);
  if (it != lobbies_.end())
  {
    return &it->second;
  }
  return nullptr;
}
