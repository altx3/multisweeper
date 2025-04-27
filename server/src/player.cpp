#include "player.hpp"

#include <utility>

Player::Player(player_id_t id) : id_(std::move(id)), lobby_id_("") {}

auto Player::get_id() const -> const player_id_t &
{
  return id_;
}

auto Player::get_lobby_id() const -> const lobby_id_t &
{
  return lobby_id_;
}

void Player::set_lobby_id(const lobby_id_t &lobby_id)
{
  lobby_id_ = lobby_id;
}

void Player::leave_lobby()
{
  lobby_id_.value = "";
}
