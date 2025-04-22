#include "http_handler.hpp"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

constexpr auto CLIENT_URL = "http://localhost:5173";

HTTPHandler::HTTPHandler(LobbyManager *lobby_manager, uWS::App *app)
    : lobby_manager_(lobby_manager), app_(app)
{
}

// will write 200 OK look in the writeHeader function
void HTTPHandler::add_cors_headers(uWS::HttpResponse<false> *res)
{
  res->writeHeader("Access-Control-Allow-Origin", CLIENT_URL);
  res->writeHeader("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
  res->writeHeader("Access-Control-Allow-Headers", "Content-Type");
}

void HTTPHandler::register_routes()
{
  app_->options("/lobbies/create",
                [this](auto *res, auto *req)
                {
                  add_cors_headers(res);
                  res->end();
                });

  app_->options("/lobbies/join/:lobby_id",
                [this](auto *res, auto *req)
                {
                  add_cors_headers(res);
                  res->end();
                });

  app_->options("/lobbies/:lobby_id",
                [this](auto *res, auto *req)
                {
                  add_cors_headers(res);
                  res->end();
                });

  app_->post("/lobbies/create",
             [this](auto *res, auto *req) { handle_create_lobby(res, req); });

  app_->post("/lobbies/join/:lobby_id",
             [this](auto *res, auto *req) { handle_join_lobby(res, req); });

  app_->get("/lobbies/:lobby_id",
            [this](auto *res, auto *req) { handle_get_lobby(res, req); });
}

void HTTPHandler::handle_create_lobby(uWS::HttpResponse<false> *res,
                                      uWS::HttpRequest *req [[maybe_unused]])
{
  auto host_id = generate_random_id<player_id_t>("player_");
  lobby_id_t lobby_id = lobby_manager_->create_lobby(host_id);
  res->writeStatus("201 Created");
  add_cors_headers(res);
  res->end(json{{"lobby_id", lobby_id}, {"player_id", host_id}}.dump());
}

void HTTPHandler::handle_join_lobby(uWS::HttpResponse<false> *res,
                                    uWS::HttpRequest *req)
{
  lobby_id_t const lobby_id(std::string(req->getParameter("lobby_id")));
  auto player_id = generate_random_id<player_id_t>("player_");
  if (lobby_manager_->join_lobby(lobby_id, player_id))
  {
    res->writeStatus("200 OK");
    add_cors_headers(res);
    res->end(json{{"lobby_id", lobby_id}, {"player_id", player_id}}.dump());
  }
  else
  {
    res->writeStatus("404 Not Found");
    add_cors_headers(res);
    res->end(json{{"error", "Lobby not found"}}.dump());
  }
}

void HTTPHandler::handle_get_lobby(uWS::HttpResponse<false> *res,
                                   uWS::HttpRequest *req)
{
  lobby_id_t const lobby_id(std::string(req->getParameter("lobby_id")));
  if (lobby_manager_->lobby_exists(lobby_id))
  {
    res->writeStatus("200 OK");
    add_cors_headers(res);
    res->end(json{{"lobby_id", lobby_id}}.dump());
  }
  else
  {
    res->writeStatus("404 Not Found");
    add_cors_headers(res);
    res->end(json{{"error", "Lobby not found"}}.dump());
  }
}
