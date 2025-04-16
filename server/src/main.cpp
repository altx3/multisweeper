#include <string>

#include "logger.hpp"

#include "App.h"

constexpr int PORT = 9001;

auto main() -> int
{
  Logger::init();
  Logger::log("Backend server starting...");

  struct EmptyData
  {
  };

  uWS::App()
    // Define WebSocket behavior for the route "/*" (all routes)
    .ws<EmptyData>("/*",
                   {// This callback runs when a client connects
                    .open = [](auto *ws [[maybe_unused]])
                    { Logger::log("A client connected"); },

                    // This callback runs when a client sends a message
                    .message =
                      [](auto *ws, std::string_view message, uWS::OpCode opCode)
                    {
                      Logger::log("Received message: " + std::string(message));

                      // Simple ping-pong: if client sends "ping", respond with
                      // "pong"
                      if (message == "ping")
                      {
                        Logger::log("Sent pong");
                        ws->send("pong", opCode);
                      }
                      else
                      {
                        // Echo the message back
                        ws->send(message, opCode);
                      }
                    },

                    // This callback runs when a client disconnects
                    .close =
                      [](auto *, int code, std::string_view)
                    {
                      Logger::log("A client disconnected, code: " +
                                  std::to_string(code));
                    }})

    // Start listening on port 9001
    .listen(PORT,
            [](auto *listen_socket)
            {
              if (listen_socket)
              {
                Logger::log("Server listening on port " + std::to_string(PORT));
              }
              else
              {
                Logger::log("Failed to listen on port " + std::to_string(PORT));
              }
            })
    .run();
  return 0;
}
