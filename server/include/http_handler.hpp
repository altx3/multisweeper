#ifndef HTTP_HANDLER_HPP
#define HTTP_HANDLER_HPP

#include <App.h>

#include "lobby_manager.hpp"

class HTTPHandler
{
public:
  HTTPHandler(LobbyManager &lobby_manager);
  void run();

private:
  void handle_create_lobby(uWS::HttpResponse<false> *res,
                           uWS::HttpRequest *req);
  void handle_join_lobby(uWS::HttpResponse<false> *res, uWS::HttpRequest *req);

  LobbyManager &lobby_manager_;
  uWS::App app_;
};

#endif
