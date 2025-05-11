#include <nlohmann/json.hpp>

#include "logger.hpp"
#include "websocket_handler.hpp"

#include <iostream>

using json = nlohmann::json;

WebSocketHandler::WebSocketHandler(LobbyManager *lobby_manager, uWS::App *app)
    : lobby_manager_(lobby_manager), app_(app)
{
}

void WebSocketHandler::register_routes()
{
  app_->ws<WebSocketData>(
    "/*",
    {.open = [this](auto *ws) { on_open(ws); },
     .message = [this](auto *ws, std::string_view msg, uWS::OpCode op_code)
     { on_message(ws, msg, op_code); },
     .close = [this](auto *ws, int code, std::string_view reason)
     { on_close(ws, code, reason); }});
}

void WebSocketHandler::on_open(
  [[maybe_unused]] uWS::WebSocket<false, true, WebSocketData> *ws)
{
  Logger::log("WebSocket connection opened");
}

void WebSocketHandler::on_message(
  uWS::WebSocket<false, true, WebSocketData> *ws, std::string_view message,
  [[maybe_unused]] uWS::OpCode op_code)
{
  auto *ws_data = ws->getUserData();
  try
  {
    json j = json::parse(message);
    if (j["type"] == "init")
    {
      ws_data->lobby_id = lobby_id_t(j["lobby_id"].get<std::string>());
      ws_data->player_id = player_id_t(j["player_id"].get<std::string>());
      Logger::log(
        "Initialized ws_data - lobby_id: " + std::string(ws_data->lobby_id) +
        ", player_id: " + std::string(ws_data->player_id));
      connections_[ws_data->player_id] = ws;
    }
    else if (j["type"] == "mouse_move")
    {
      Lobby *lobby = lobby_manager_->get_lobby(ws_data->lobby_id);
      std::vector<player_id_t> players = lobby->get_players();

      json message = {{"type", "mouse_update"},
                      {"player_id", std::string(ws_data->player_id)},
                      {"x", j["x"]},
                      {"y", j["y"]}};

      std::string message_str = message.dump();

      for (const auto &[id, conn] : connections_)
      {
        if (id != ws_data->player_id && lobby->is_player_in_lobby(id))
        {
          conn->send(message_str, uWS::OpCode::TEXT);
        }
      }
    }
    else
    {
      Logger::warn("Unhandled message type: " + j["type"].get<std::string>());
    }
  }
  catch (const std::exception &e)
  {
    Logger::warn(std::string(message));
    Logger::warn("Error parsing message: " + std::string(e.what()));
  }
}

void WebSocketHandler::on_close(uWS::WebSocket<false, true, WebSocketData> *ws,
                                [[maybe_unused]] int code,
                                [[maybe_unused]] std::string_view reason)
{
  auto *ws_data = ws->getUserData();
  if (!ws_data->lobby_id.empty() && !ws_data->player_id.empty())
  {
    Logger::log("Player: " + std::string(ws_data->player_id) +
                " disconnected from Lobby: " + std::string(ws_data->lobby_id));
    lobby_manager_->leave_lobby(ws_data->lobby_id, ws_data->player_id);
    connections_.erase(ws_data->player_id);
  }
  else
  {
    Logger::log("Warning: Incomplete ws_data in on_close");
  }
}
