#include "http_handler.hpp"
#include "logger.hpp"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

constexpr auto CLIENT_URL = "http://localhost:5173";

HTTPHandler::HTTPHandler(LobbyManager *lobby_manager, uWS::App *app)
    : lobby_manager_(lobby_manager), app_(app)
{
}

void HTTPHandler::add_cors_headers(uWS::HttpResponse<false> *res)
{
  res->writeHeader("Access-Control-Allow-Origin", CLIENT_URL);
  res->writeHeader("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
  res->writeHeader("Access-Control-Allow-Headers", "Content-Type");
}

void HTTPHandler::register_routes()
{
  // cors stuff

  app_->options("/user/create",
                [this](auto *res, [[maybe_unused]] auto *req)
                {
                  HTTPHandler::add_cors_headers(res);
                  res->end();
                });

  app_->options("/lobbies/create",
                [this](auto *res, [[maybe_unused]] auto *req)
                {
                  HTTPHandler::add_cors_headers(res);
                  res->end();
                });

  app_->options("/lobbies/:lobby_id/join",
                [this](auto *res, [[maybe_unused]] auto *req)
                {
                  HTTPHandler::add_cors_headers(res);
                  res->end();
                });

  app_->options("/lobbies/:lobby_id",
                [this](auto *res, [[maybe_unused]] auto *req)
                {
                  HTTPHandler::add_cors_headers(res);
                  res->end();
                });

  app_->options("/lobbies",
                [this](auto *res, [[maybe_unused]] auto *req)
                {
                  HTTPHandler::add_cors_headers(res);
                  res->end();
                });

  // create a user
  app_->post("/user/create", [this](auto *res, auto *req)
             { HTTPHandler::handle_create_user(res, req); });

  // create a lobby
  app_->post("/lobbies/create",
             [this](auto *res, auto *req) { handle_create_lobby(res, req); });

  // join a lobby from an id
  app_->post("/lobbies/:lobby_id/join",
             [this](auto *res, auto *req) { handle_join_lobby(res, req); });

  // check if a lobby exist if you directly enter the id
  app_->get("/lobbies/:lobby_id",
            [this](auto *res, auto *req) { handle_get_lobby(res, req); });

  // get a list of all lobbies
  app_->get("/lobbies",
            [this](auto *res, auto *req) { handle_get_lobbies(res, req); });
}

void HTTPHandler::handle_create_user(uWS::HttpResponse<false> *res,
                                     [[maybe_unused]] uWS::HttpRequest *req)
{
  auto player_id = generate_random_id<player_id_t>("player_");
  res->writeStatus("201 Created");
  HTTPHandler::add_cors_headers(res);
  res->end(json{{"player_id", player_id}}.dump());
}

void HTTPHandler::handle_create_lobby(uWS::HttpResponse<false> *res,
                                      uWS::HttpRequest *req)
{
  // Buffer to store the incoming request body
  std::string body;
  // Handle incoming data chunks
  res->onData(
    [res, req, body = std::move(body), this](std::string_view chunk,
                                             bool isLast) mutable
    {
      body.append(chunk);

      // Process the body only when all data is received
      if (isLast)
      {
        try
        {
          // Parse the JSON body
          auto json_body = json::parse(body);
          const player_id_t host_id(json_body["host_id"].get<std::string>());

          // Create the lobby
          lobby_id_t lobby_id = lobby_manager_->create_lobby(host_id);

          // Send response
          res->writeStatus("201 Created");
          HTTPHandler::add_cors_headers(res);
          res->end(json{{"lobby_id", lobby_id}, {"host_id", host_id}}.dump());
        }
        catch (const std::exception &e)
        {
          // Handle invalid JSON or missing host_id
          Logger::warn("Error: " + static_cast<std::string>(e.what()));
          res->writeStatus("400 Bad Request");
          HTTPHandler::add_cors_headers(res);
          res->end(json{{"error", "Invalid request body"}}.dump());
        }
      }
    });

  // Handle case where no data is sent
  res->onAborted(
    [res]()
    {
      Logger::warn("No data sent, create lobby");
      res->writeStatus("400 Bad Request");
      HTTPHandler::add_cors_headers(res);
      res->end(json{{"error", "Request aborted"}}.dump());
    });
}

void HTTPHandler::handle_join_lobby(uWS::HttpResponse<false> *res,
                                    uWS::HttpRequest *req)
{
  // Buffer to store the incoming request body
  std::string body;
  // Handle incoming data chunks
  res->onData(

    [res, req, body = std::move(body), this](std::string_view chunk,
                                             bool isLast) mutable
    {
      body.append(chunk);

      // Process the body only when all data is received
      if (isLast)
      {
        try
        {
          // Parse the JSON body
          auto json_body = json::parse(body);
          Logger::log(body);
          const player_id_t player_id(
            json_body["player_id"].get<std::string>());

          // Get lobby id from url
          const lobby_id_t lobby_id(std::string(req->getParameter("lobby_id")));

          if (lobby_manager_->join_lobby(lobby_id, player_id))
          {
            res->writeStatus("200 OK");
            HTTPHandler::add_cors_headers(res);
            res->end(
              json{{"lobby_id", lobby_id}, {"player_id", player_id}}.dump());
          }
          else
          {
            res->writeStatus("404 Not Found");
            HTTPHandler::add_cors_headers(res);
            res->end(json{{"error", "Lobby not found"}}.dump());
          }
        }
        catch (const std::exception &e)
        {
          // Handle invalid JSON or missing player_id
          Logger::warn("Error: " + static_cast<std::string>(e.what()));
          res->writeStatus("400 Bad Request");
          HTTPHandler::add_cors_headers(res);
          res->end(json{{"error", "Invalid request body"}}.dump());
        }
      }
    });

  // Handle case where no data is sent
  res->onAborted(
    [res]()
    {
      Logger::warn("No data sent, join lobby");
      res->writeStatus("400 Bad Request");
      HTTPHandler::add_cors_headers(res);
      res->end(json{{"error", "Request aborted"}}.dump());
    });
}

void HTTPHandler::handle_get_lobby(uWS::HttpResponse<false> *res,
                                   uWS::HttpRequest *req)
{
  const lobby_id_t lobby_id(std::string(req->getParameter("lobby_id")));
  if (lobby_manager_->get_lobby(lobby_id) != nullptr)
  {
    res->writeStatus("200 OK");
    HTTPHandler::add_cors_headers(res);
    res->end(json{{"lobby_id", lobby_id}}.dump());
  }
  else
  {
    res->writeStatus("404 Not Found");
    HTTPHandler::add_cors_headers(res);
    res->end(json{{"error", "Lobby not found"}}.dump());
  }
}

void HTTPHandler::handle_get_lobbies(uWS::HttpResponse<false> *res,
                                     [[maybe_unused]] uWS::HttpRequest *req)
{
  const auto &lobbies = lobby_manager_->get_lobbies();
  nlohmann::json json_lobbies = nlohmann::json::array();
  for (const auto &[id, lobby] : lobbies)
  {
    json_lobbies.push_back(lobby.get_state());
  }

  const nlohmann::json response = {{"lobbies", json_lobbies}};

  res->writeStatus("200 OK");
  HTTPHandler::add_cors_headers(res);
  res->end(response.dump());
}
