#include <nlohmann/json.hpp>

#include "logger.hpp"
#include "websocket_handler.hpp"

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
  try
  {
    json data = json::parse(message);
    auto *ws_data = ws->getUserData();

    if (data["type"] == "subscribe")
    {
      lobby_id_t const lobby_id(data["lobby_id"]);
      player_id_t const player_id(data["player_id"]);
      if (lobby_manager_->get_lobby(lobby_id) != nullptr)
      {
        ws_data->player_id = player_id;
        ws_data->lobby_id = lobby_id;
        connections_[player_id] = ws;
        json const state = lobby_manager_->get_lobby(lobby_id)->get_state();
        ws->send(state.dump(), op_code);
        Logger::log("Player " + player_id.value + " subscribed to lobby " +
                    lobby_id.value);
      }
      else
      {
        json const error = {{"type", "error"}, {"message", "Lobby not found"}};
        ws->send(error.dump(), op_code);
      }
    }
    else if (data["type"] == "player_update")
    {
      // Broadcast player data (e.g., location, name, ID) to other players in
      // the lobby
      lobby_id_t const lobby_id(ws_data->lobby_id);
      if (Lobby *lobby = lobby_manager_->get_lobby(lobby_id))
      {
        for (const auto &pid : lobby->get_state()["players"])
        {
          if (pid != ws_data->player_id &&
              connections_.find(pid) != connections_.end())
          {
            connections_[pid]->send(message, op_code);
          }
        }
      }
    }
  }
  catch (const json::exception &e)
  {
    Logger::log("JSON parse error: " + std::string(e.what()));
  }
}

void WebSocketHandler::on_close(uWS::WebSocket<false, true, WebSocketData> *ws,
                                int code [[maybe_unused]],
                                std::string_view reason [[maybe_unused]])
{
  auto *ws_data = ws->getUserData();
  if (!ws_data->lobby_id.empty())
  {
    lobby_id_t const lobby_id(ws_data->lobby_id);
    player_id_t const player_id(ws_data->player_id);
    lobby_manager_->leave_lobby(lobby_id, player_id);
    connections_.erase(ws_data->player_id);
    Logger::log("Player " + ws_data->player_id.value + " disconnected");
  }
}
