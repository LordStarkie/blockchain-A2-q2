#ifndef BLOCKCHAIN_A2_Q2_BLOCKCHAIN_H
#define BLOCKCHAIN_A2_Q2_BLOCKCHAIN_H

#include <vector>
#include "block.h"

class Blockchain {
public:
    Blockchain() {
        // create the genesis block
        Block genesis_block;
        chain.push_back(genesis_block);
    }

    // add a block to the blockchain
    void add_block(const std::vector<Transaction>& transactions) {
        uint64_t index = chain.size();
        std::string previous_hash = chain.back().get_hash();
        Block new_block(index, previous_hash, transactions);
        chain.push_back(new_block);
    }

    // get the blockchain
    [[nodiscard]] const std::vector<Block>& get_chain() const {
        return chain;
    }

private:
    std::vector<Block> chain;
};

#endif //BLOCKCHAIN_A2_Q2_BLOCKCHAIN_H
