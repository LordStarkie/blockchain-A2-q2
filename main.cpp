#include <vector>
#include "src/user/user.h"
#include <GLFW/glfw3.h>
#include "src/nft/nft.h"
#include "src/blockchain/blockchain.h"
#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"
#include "ui.h"

void error_callback(int error, const char* description) {
    fprintf(stderr, "Error: %s\n", description);
}

int main() {
    // init blockchain
    Blockchain blockchain;

    User user1("user1");
    User user2("user2");
    User user3("user3");

    std::vector<User*> all_users = { &user1, &user2, &user3 };
    std::vector<NFT*> all_nfts;

    // set error callback
    glfwSetErrorCallback(error_callback);
    if (!glfwInit()) {
        fprintf(stderr, "Error: Failed to initialize GLFW.\n");
        return 1;
    }

    // GL+GLSL versions for Mac
    const char* glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac

    // create GLFW window
    GLFWwindow* window = glfwCreateWindow(1280, 720, "Q2 - NFT", nullptr, nullptr);
    if (window == nullptr) {
        fprintf(stderr, "Error: Failed to create GLFW window.\n");
        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    // Setup ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();

    // setup platform/renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // main application loop
    while (!glfwWindowShouldClose(window)) {
        // poll and handle events
        glfwPollEvents();

        // start ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // application UI code
        ShowUserNFTs(all_users);
        ShowMintNFTWindow(all_users, blockchain, all_nfts);
        ShowTransferNFTWindow(all_users, blockchain);
        ShowAllNFTs(all_nfts);
        ShowBlockchain(blockchain);
        ShowNFTHistory(all_nfts, blockchain);

        // rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    // cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
