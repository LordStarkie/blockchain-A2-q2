cmake_minimum_required(VERSION 3.29)
project(blockchain_A2_q2)

set(CMAKE_CXX_STANDARD 20)

add_executable(blockchain_A2_q2 main.cpp
        src/nft/nft.cpp
        src/user/user.cpp
        src/blockchain/transaction.cpp
        src/blockchain/block.cpp
        src/blockchain/blockchain.cpp
        src/nft/nft_controller.cpp
        ui.cpp

        imgui/backends/imgui_impl_opengl3.cpp
        imgui/backends/imgui_impl_glfw.cpp
        imgui/backends/imgui_impl_opengl3_loader.h

        imgui/imgui.cpp
        imgui/imgui_demo.cpp
        imgui/imgui_draw.cpp
        imgui/imgui_tables.cpp
        imgui/imgui_widgets.cpp
)

# Find OpenSSL package
find_package(OpenSSL REQUIRED)
include_directories(${OPENSSL_INCLUDE_DIR})

# Link OpenSSL libraries
target_link_libraries(blockchain_A2_q2 OpenSSL::SSL OpenSSL::Crypto)

# Find GLFW package
find_package(glfw3 REQUIRED)
include_directories(${GLFW3_INCLUDE_DIRS})

# Link GLFW libraries
target_link_libraries(blockchain_A2_q2 glfw)

# Find GLEW package
find_package(GLEW REQUIRED)
include_directories(${GLEW_INCLUDE_DIRS})

# Link GLEW libraries
target_link_libraries(blockchain_A2_q2 GLEW::GLEW)
