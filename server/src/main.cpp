#include <iostream>
#include <string>

#include "logger.hpp"

#include "App.h"

constexpr int PORT = 9001;

auto main() -> int
{
  Logger::init();

  struct PerSocketData
  {
    /* Define your user data */
    int something;
  };

  uWS::App()
      .ws<PerSocketData>(
          "/*",
          {
              // This callback runs when a client connects
              .open = [](auto *ws [[maybe_unused]])
              { std::cout << "A client connected" << std::endl; },
          })
      .listen(PORT,
              [](auto *listen_socket)
              {
                if (listen_socket)
                {
                  std::cout << "Server listening on port 9001" << std::endl;
                }
                else
                {
                  std::cout << "Failed to listen on port 9001" << std::endl;
                }
              })

      // Start the server
      .run();
  Logger::log("Backend server starting...");
  return 0;
}
