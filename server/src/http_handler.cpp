#include "http_handler.hpp"
#include "logger.hpp"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

constexpr int PORT = 8080;

HTTPHandler::HTTPHandler(LobbyManager &lobby_manager)
    : lobby_manager_(lobby_manager)
{
  app_.post("/lobbies",
            [this](auto *res, auto *req) { handle_create_lobby(res, req); });
  app_.post("/lobbies/:lobby_id/join",
            [this](auto *res, auto *req) { handle_join_lobby(res, req); });
}

void HTTPHandler::run()
{
  app_
    .listen(PORT,
            [](auto *token)
            {
              if (token)
              {
                Logger::log("HTTP server listening on port " +
                            std::to_string(PORT));
              }
              else
              {
                Logger::log("Failed to listen on port " + std::to_string(PORT));
              }
            })
    .run();
}

void HTTPHandler::handle_create_lobby(uWS::HttpResponse<false> *res,
                                      uWS::HttpRequest * /*req*/)
{
  player_id_t host_id = generate_random_id<player_id_t>("player_");
  lobby_id_t lobby_id = lobby_manager_.create_lobby(host_id);
  res->writeStatus("201 Created");
  res->end(json{{"lobby_id", lobby_id}, {"player_id", host_id}}.dump());
}

void HTTPHandler::handle_join_lobby(uWS::HttpResponse<false> *res,
                                    uWS::HttpRequest *req)
{
  lobby_id_t lobby_id(std::string(req->getParameter("lobby_id")));
  player_id_t player_id = generate_random_id<player_id_t>("player_");
  if (lobby_manager_.join_lobby(lobby_id, player_id))
  {
    res->writeStatus("200 OK");
    res->end(json{{"player_id", player_id}}.dump());
  }
  else
  {
    res->writeStatus("404 Not Found");
    res->end(json{{"error", "Lobby not found"}}.dump());
  }
}
