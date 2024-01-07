cmake_minimum_required(VERSION 3.5)
project(TankShooter VERSION 0.1 LANGUAGES CXX)
cmake_policy(SET CMP0072 NEW)

file(GLOB TankShooterSRC "src/*/*.cpp")
file(GLOB SocketSRC "src/socket/*/*.cpp")
file(GLOB NetworkingSRC "src/socket/Networking/*/*.cpp")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2_mixer REQUIRED)
find_package(nlohmann_json REQUIRED)

include_directories(
  ${SDL2_INCLUDE_DIRS}
  ${SDL2_IMAGE_INCLUDE_DIRS}
  ${SDL2_TTF_INCLUDE_DIRS}
  ${SDL2_MIXER_INCLUDE_DIRS}
)

include_directories(include)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(${PROJECT_NAME}
  src/main.cpp
  ${NetworkingSRC}
  ${SocketSRC}
  ${TankShooterSRC}
)

add_executable(Server src/server.cpp ${SocketSRC} ${NetworkingSRC})

add_executable(Client src/client.cpp ${SocketSRC} ${NetworkingSRC})

target_link_libraries(TankShooter
  ${SDL2_LIBRARIES}
  ${SDL2_IMAGE_LIBRARIES}
  ${SDL2_TTF_LIBRARIES}
  ${SDL2_MIXER_LIBRARIES}
  ${SDL_IMAGE_LIBRARIES}
  pthread
  nlohmann_json::nlohmann_json
  ${CMAKE_DL_LIBS}
)
