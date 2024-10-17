#ifndef BLOCKCHAIN_A2_Q2_NFT_CONTROLLER_H
#define BLOCKCHAIN_A2_Q2_NFT_CONTROLLER_H

#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <algorithm>
#include "nft.h"
#include "../user/user.h"
#include "../blockchain/blockchain.h"
#include "../blockchain//transaction.h"

class NFTController {
public:
    // mint nft
    static NFT* mint_nft(const std::string& name, const std::string& description, User* owner, Blockchain& blockchain) {
        // create nft
        NFT* nft = new NFT(name, description);

        // assign nft
        owner->add_nft(nft);

        // create transaction
        Transaction mint_tx(TransactionType::Mint, nft->get_unique_id(), nft->get_name(),
                            "", owner->get_username());

        // add to blockchain
        blockchain.add_block({ mint_tx });

        std::cout << "NFT minted and assigned to " << owner->get_username() << "." << std::endl;
        return nft;
    }

    // transfer nft ownership
    static bool transfer_nft(NFT* nft, User* from_user, User* to_user, Blockchain& blockchain) {

        if (!from_user->owns_nft(nft)) {
            std::cout << "Transfer failed: '" << from_user->get_username()
                      << "' does not own the NFT." << std::endl;
            return false;
        }

        // remove nft from user
        if (!from_user->remove_nft(nft)) {
            std::cout << "Transfer failed: Unable to remove NFT from '"
                      << from_user->get_username() << "'." << std::endl;
            return false;
        }

        // add nft
        to_user->add_nft(nft);

        // create transfer transaction
        Transaction transfer_tx(TransactionType::Transfer, nft->get_unique_id(), nft->get_name(),
                                from_user->get_username(), to_user->get_username());

        // add to blockchain
        blockchain.add_block({ transfer_tx });

        std::cout << "Transfer successful: NFT transferred to "
                  << to_user->get_username() << "." << std::endl;
        return true;
    }

    // find owner
    static User* find_owner(NFT* nft, const std::vector<User*>& users) {
        for (User* user : users) {
            if (user->owns_nft(nft)) {
                return user;
            }
        }
        return nullptr;
    }

    // get ownership history
    static std::vector<Transaction> get_nft_history(uint64_t nft_id, const Blockchain& blockchain) {
        std::vector<Transaction> history;

        for (const auto& block : blockchain.get_chain()) {
            for (const auto& tx : block.get_transactions()) {
                if (tx.get_nft_id() == nft_id) {
                    history.push_back(tx);
                }
            }
        }
        return history;
    }
};

#endif // BLOCKCHAIN_A2_Q2_NFT_CONTROLLER_H
