#ifndef BLOCKCHAIN_A2_Q2_UI_H
#define BLOCKCHAIN_A2_Q2_UI_H
#include <vector>
#include "src/user/user.h"
#include "src/nft/nft.h"
#include "src/blockchain/blockchain.h"

// UI Functions
void ShowUserNFTs(const std::vector<User*>& users);
void ShowMintNFTWindow(std::vector<User*>& users, Blockchain& blockchain, std::vector<NFT*>& all_nfts);
void ShowTransferNFTWindow(std::vector<User*>& users, Blockchain& blockchain);
void ShowAllNFTs(const std::vector<NFT*>& all_nfts);
void ShowBlockchain(const Blockchain& blockchain);
void ShowNFTHistory(const std::vector<NFT*>& all_nfts, const Blockchain& blockchain);
#endif //BLOCKCHAIN_A2_Q2_UI_H
