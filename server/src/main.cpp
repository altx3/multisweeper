#include <App.h>

#include "http_handler.hpp"
#include "logger.hpp"
#include "websocket_handler.hpp"

constexpr int PORT = 9001;

auto main() -> int
{
  Logger::init();
  Logger::log("Starting server...");

  uWS::App app;
  LobbyManager lobby_manager;
  HTTPHandler http_handler(&lobby_manager, &app);
  WebSocketHandler websocket_handler(&lobby_manager, &app);

  http_handler.register_routes();
  websocket_handler.register_routes();

  app
    .listen(PORT,
            [](auto *token)
            {
              if (token)
              {
                Logger::log("Server listening on port " + std::to_string(PORT));
              }
              else
              {
                Logger::log("Failed to listen on port " + std::to_string(PORT));
              }
            })
    .run();

  Logger::log("Server stopped");
  return 0;
}
