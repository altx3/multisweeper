#ifndef WEBSOCKET_HANDLER_HPP
#define WEBSOCKET_HANDLER_HPP

#include <App.h>
#include <map>

#include "lobby_manager.hpp"
#include "player.hpp"

struct WebSocketData
{
  std::string player_id;
  std::string lobby_id;
};

class WebSocketHandler
{
public:
  WebSocketHandler(LobbyManager &lobby_manager);
  void run();

private:
  void on_open(uWS::WebSocket<false, true, WebSocketData> *ws);
  void on_message(uWS::WebSocket<false, true, WebSocketData> *ws,
                  std::string_view message, uWS::OpCode op_code);
  void on_close(uWS::WebSocket<false, true, WebSocketData> *ws, int code,
                std::string_view reason);

  LobbyManager &lobby_manager_;
  uWS::App app_;
  std::map<std::string, uWS::WebSocket<false, true, WebSocketData> *>
    connections_;
};

#endif
