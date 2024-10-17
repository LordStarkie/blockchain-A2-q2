#ifndef BLOCKCHAIN_A2_Q2_USER_H
#define BLOCKCHAIN_A2_Q2_USER_H
#include <string>
#include <utility>
#include "../nft/nft.h"

class User {
public:
    // constructor
    explicit User(std::string username)
            : username(std::move(username)) {}

    [[nodiscard]] std::string get_username() const { return username; }
    void set_username(const std::string& new_username) { username = new_username; }

    // add nft to user
    void add_nft(NFT* nft) {
        owned_nfts.push_back(nft);
    }

    // remove nft from user
    bool remove_nft(NFT* nft) {
        auto it = std::find(owned_nfts.begin(), owned_nfts.end(), nft);
        if (it != owned_nfts.end()) {
            owned_nfts.erase(it);
            return true;
        }
        return false;
    }

    // get list of owned nft
    [[nodiscard]] const std::vector<NFT*>& get_owned_nfts() const {
        return owned_nfts;
    }

    // check if user owns nft
    bool owns_nft(NFT* nft) const {
        return std::find(owned_nfts.begin(), owned_nfts.end(), nft) != owned_nfts.end();
    }

private:
    std::string username;
    std::vector<NFT*> owned_nfts;
};

#endif //BLOCKCHAIN_A2_Q2_USER_H
