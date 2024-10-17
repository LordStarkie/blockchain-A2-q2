#include "ui.h"

#include <GLFW/glfw3.h>
#include "imgui/imgui.h"
#include "src/nft/nft_controller.h"

void ShowUserNFTs(const std::vector<User*>& users) {
    ImGui::Begin("User NFT Holdings");
    for (User* user : users) {
        if (ImGui::TreeNode(user->get_username().c_str())) {
            const auto& nfts = user->get_owned_nfts();
            if (nfts.empty()) {
                ImGui::Text("No NFTs owned.");
            } else {
                for (const auto& nft : nfts) {
                    ImGui::BulletText("%s (ID: %llu)", nft->get_name().c_str(), nft->get_unique_id());
                }
            }
            ImGui::TreePop();
        }
    }
    ImGui::End();
}

void ShowMintNFTWindow(std::vector<User*>& users, Blockchain& blockchain, std::vector<NFT*>& all_nfts) {
    static int selected_user_index = 0;
    static char nft_name[128] = "";
    static char nft_description[256] = "";

    ImGui::Begin("Mint NFT");

    // User Selection
    if (ImGui::BeginCombo("Select User", users[selected_user_index]->get_username().c_str())) {
        for (int i = 0; i < users.size(); ++i) {
            bool is_selected = (selected_user_index == i);
            if (ImGui::Selectable(users[i]->get_username().c_str(), is_selected)) {
                selected_user_index = i;
            }
            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }

    // NFT Name and Description
    ImGui::InputText("NFT Name", nft_name, IM_ARRAYSIZE(nft_name));
    ImGui::InputText("Description", nft_description, IM_ARRAYSIZE(nft_description));

    if (ImGui::Button("Mint NFT")) {
        if (strlen(nft_name) > 0 && strlen(nft_description) > 0) {
            User* selected_user = users[selected_user_index];
            NFT* minted_nft = NFTController::mint_nft(nft_name, nft_description, selected_user, blockchain);
            all_nfts.push_back(minted_nft);

            // Clear input fields
            nft_name[0] = '\0';
            nft_description[0] = '\0';
        } else {
            ImGui::OpenPopup("Error");
        }
    }

    // Error Popup
    if (ImGui::BeginPopupModal("Error", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::Text("Please enter both NFT name and description.");
        if (ImGui::Button("OK")) {
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }

    ImGui::End();
}

void ShowTransferNFTWindow(std::vector<User*>& users, Blockchain& blockchain) {
    static int from_user_index = 0;
    static int to_user_index = 0;
    static NFT* selected_nft = nullptr;

    ImGui::Begin("Transfer NFT");

    // From User Selection
    if (ImGui::BeginCombo("From User", users[from_user_index]->get_username().c_str())) {
        for (int i = 0; i < users.size(); ++i) {
            bool is_selected = (from_user_index == i);
            if (ImGui::Selectable(users[i]->get_username().c_str(), is_selected)) {
                from_user_index = i;
                selected_nft = nullptr;
            }
            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }

    User* from_user = users[from_user_index];
    const auto& nfts = from_user->get_owned_nfts();

    // NFT Selection
    if (!nfts.empty()) {
        if (ImGui::BeginCombo("Select NFT", selected_nft ? selected_nft->get_name().c_str() : "Select NFT")) {
            for (auto& nft : nfts) {
                bool is_selected = (selected_nft == nft);
                if (ImGui::Selectable(nft->get_name().c_str(), is_selected)) {
                    selected_nft = nft;
                }
                if (is_selected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }
    } else {
        ImGui::Text("No NFTs to transfer.");
    }

    // To User Selection
    if (ImGui::BeginCombo("To User", users[to_user_index]->get_username().c_str())) {
        for (int i = 0; i < users.size(); ++i) {
            if (i == from_user_index) continue; // Skip the from_user
            bool is_selected = (to_user_index == i);
            if (ImGui::Selectable(users[i]->get_username().c_str(), is_selected)) {
                to_user_index = i;
            }
            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }

    if (ImGui::Button("Transfer NFT")) {
        if (selected_nft) {
            User* to_user = users[to_user_index];
            NFTController::transfer_nft(selected_nft, from_user, to_user, blockchain);
            selected_nft = nullptr; // Reset after transfer
        } else {
            ImGui::OpenPopup("Error");
        }
    }

    // Error Popup
    if (ImGui::BeginPopupModal("Error", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::Text("Please select an NFT to transfer.");
        if (ImGui::Button("OK")) {
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }

    ImGui::End();
}

void ShowAllNFTs(const std::vector<NFT*>& all_nfts) {
    ImGui::Begin("All NFTs");
    if (all_nfts.empty()) {
        ImGui::Text("No NFTs available.");
    } else {
        for (const auto& nft : all_nfts) {
            if (ImGui::TreeNode((nft->get_name() + " (ID: " + std::to_string(nft->get_unique_id()) + ")").c_str())) {
                ImGui::Text("Description: %s", nft->get_description().c_str());
                ImGui::TreePop();
            }
        }
    }
    ImGui::End();
}

void ShowBlockchain(const Blockchain& blockchain) {
    ImGui::Begin("Blockchain Explorer");

    const auto& chain = blockchain.get_chain();
    for (const auto& block : chain) {
        if (ImGui::TreeNode((std::string("Block ") + std::to_string(block.get_index())).c_str())) {
            ImGui::Text("Hash: %s", block.get_hash().c_str());
            ImGui::Text("Previous Hash: %s", block.get_previous_hash().c_str());
            std::time_t timestamp = block.get_timestamp();
            ImGui::Text("Timestamp: %s", std::ctime(&timestamp));

            if (ImGui::TreeNode("Transactions")) {
                for (const auto& tx : block.get_transactions()) {
                    ImGui::BulletText("Type: %s", (tx.get_type() == TransactionType::Mint) ? "Mint" : "Transfer");
                    ImGui::Text("  NFT ID: %llu", tx.get_nft_id());
                    ImGui::Text("  NFT Name: %s", tx.get_nft_name().c_str());
                    ImGui::Text("  From: %s", tx.get_from_username().empty() ? "N/A" : tx.get_from_username().c_str());
                    ImGui::Text("  To: %s", tx.get_to_username().c_str());
                    std::time_t timestamp = tx.get_timestamp();
                    ImGui::Text("  Timestamp: %s", std::ctime(&timestamp));
                }
                ImGui::TreePop();
            }
            ImGui::TreePop();
        }
    }
    ImGui::End();
}

void ShowNFTHistory(const std::vector<NFT*>& all_nfts, const Blockchain& blockchain) {
    static NFT* selected_nft = nullptr;

    ImGui::Begin("NFT Ownership History");

    // NFT Selection
    if (ImGui::BeginCombo("Select NFT", selected_nft ? (selected_nft->get_name() + " (ID: " +
    std::to_string(selected_nft->get_unique_id()) + ")").c_str() : "Select NFT")) {
        for (auto& nft : all_nfts) {
            bool is_selected = (selected_nft == nft);
            if (ImGui::Selectable((nft->get_name() + " (ID: " + std::to_string(nft->get_unique_id()) + ")").c_str(), is_selected)) {
                selected_nft = nft;
            }
            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }

    if (selected_nft) {
        auto history = NFTController::get_nft_history(selected_nft->get_unique_id(), blockchain);
        if (history.empty()) {
            ImGui::Text("No history available for this NFT.");
        } else {
            if (ImGui::BeginChild("History", ImVec2(0, 0), true)) {
                for (const auto& tx : history) {
                    std::time_t timestamp = tx.get_timestamp();  // Store the timestamp in a variable
                    ImGui::BulletText("%s at %s", (tx.get_type() == TransactionType::Mint) ? "Minted" : "Transferred", std::ctime(&timestamp));
                    ImGui::Text("  From: %s", tx.get_from_username().empty() ? "N/A" : tx.get_from_username().c_str());
                    ImGui::Text("  To: %s", tx.get_to_username().c_str());
                }
                ImGui::EndChild();
            }
        }
    }

    ImGui::End();
}
