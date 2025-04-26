#pragma once

#include <App.h>

#include "lobby_manager.hpp"

class HTTPHandler
{
public:
  HTTPHandler(LobbyManager *lobby_manager, uWS::App *app);
  void register_routes();

private:
  static void add_cors_headers(uWS::HttpResponse<false> *res);
  void handle_create_lobby(uWS::HttpResponse<false> *res,
                           uWS::HttpRequest *req);
  void handle_join_lobby(uWS::HttpResponse<false> *res, uWS::HttpRequest *req);
  void handle_get_lobby(uWS::HttpResponse<false> *res, uWS::HttpRequest *req);
  void handle_get_lobbies(uWS::HttpResponse<false> *res, uWS::HttpRequest *req);

  LobbyManager *lobby_manager_;
  uWS::App *app_;
};
