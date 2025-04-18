#include "player.hpp"

Player::Player(const player_id_t &id) : id_(id), lobby_id_("") {}

const player_id_t &Player::get_id() const
{
  return id_;
}

const lobby_id_t &Player::get_lobby_id() const
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
