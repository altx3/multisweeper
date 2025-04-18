#include "websocket_handler.hpp"
#include "logger.hpp"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

constexpr int PORT = 9001;

WebSocketHandler::WebSocketHandler(LobbyManager &lobby_manager)
    : lobby_manager_(lobby_manager)
{
  app_.ws<WebSocketData>(
    "/*",
    {.open = [this](auto *ws) { on_open(ws); },
     .message = [this](auto *ws, std::string_view msg, uWS::OpCode op_code)
     { on_message(ws, msg, op_code); },
     .close = [this](auto *ws, int code, std::string_view reason)
     { on_close(ws, code, reason); }});
}

void WebSocketHandler::run()
{
  app_
    .listen(PORT,
            [](auto *token)
            {
              if (token)
              {
                Logger::log("WebSocket server listening on port " +
                            std::to_string(PORT));
              }
              else
              {
                Logger::log("Failed to listen on port " + std::to_string(PORT));
              }
            })
    .run();
}

void WebSocketHandler::on_open(uWS::WebSocket<false, true, WebSocketData> *ws)
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
      lobby_id_t lobby_id(data["lobby_id"]);
      player_id_t player_id(data["player_id"]);
      if (lobby_manager_.get_lobby(lobby_id))
      {
        ws_data->player_id = player_id;
        ws_data->lobby_id = lobby_id;
        connections_[player_id] = ws;
        json state = lobby_manager_.get_lobby(lobby_id)->get_state();
        ws->send(state.dump(), op_code);
        Logger::log("Player " + player_id.value + " subscribed to lobby " +
                    lobby_id.value);
      }
      else
      {
        json error = {{"type", "error"}, {"message", "Lobby not found"}};
        ws->send(error.dump(), op_code);
      }
    }
    else if (data["type"] == "player_update")
    {
      // Broadcast player data (e.g., location, name, ID) to other players in
      // the lobby
      lobby_id_t lobby_id(ws_data->lobby_id);
      if (Lobby *lobby = lobby_manager_.get_lobby(lobby_id))
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
                                int /*code*/, std::string_view /*reason*/)
{
  auto *ws_data = ws->getUserData();
  if (!ws_data->lobby_id.empty())
  {
    lobby_id_t lobby_id(ws_data->lobby_id);
    player_id_t player_id(ws_data->player_id);
    lobby_manager_.leave_lobby(lobby_id, player_id);
    connections_.erase(ws_data->player_id);
    Logger::log("Player " + ws_data->player_id + " disconnected");
  }
}
