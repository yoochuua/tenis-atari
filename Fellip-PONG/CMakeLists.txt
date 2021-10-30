project(PONG)
add_executable(${PROJECT_NAME} main.cpp openglwindow.cpp Player1.cpp Player2.cpp ball.cpp)
enable_abcg(${PROJECT_NAME})