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

void WebSocketHandler::on_open(uWS::WebSocket<false, true, WebSocketData> *ws
                               [[maybe_unused]])
{
  Logger::log("WebSocket connection opened");
}

void WebSocketHandler::on_message(
  uWS::WebSocket<false, true, WebSocketData> *ws, std::string_view message,
  uWS::OpCode op_code)
{
  auto *ws_data = ws->getUserData();
  try
  {
    json j = json::parse(message);
    if (j["type"] == "init")
    {
      ws_data->lobby_id = lobby_id_t(j["lobbyId"].get<std::string>());
      ws_data->player_id = player_id_t(j["playerId"].get<std::string>());
      Logger::log(
        "Initialized ws_data - lobby_id: " + std::string(ws_data->lobby_id) +
        ", player_id: " + std::string(ws_data->player_id));
      connections_[ws_data->player_id] = ws;
    }
  }
  catch (const std::exception &e)
  {
    Logger::log(std::string(message));
    Logger::log("Error parsing message: " + std::string(e.what()));
  }
}

void WebSocketHandler::on_close(uWS::WebSocket<false, true, WebSocketData> *ws,
                                int code, std::string_view reason)
{
  auto *ws_data = ws->getUserData();
  Logger::log("on_close - lobby_id: " + std::string(ws_data->lobby_id) +
              ", player_id: " + std::string(ws_data->player_id));
  if (!ws_data->lobby_id.empty() && !ws_data->player_id.empty())
  {
    lobby_manager_->leave_lobby(ws_data->lobby_id, ws_data->player_id);
    connections_.erase(ws_data->player_id);
    Logger::log("Player " + std::string(ws_data->player_id) + " disconnected");
  }
  else
  {
    Logger::log("Warning: Incomplete ws_data in on_close");
  }
}
