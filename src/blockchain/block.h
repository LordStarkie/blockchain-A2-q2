#ifndef BLOCKCHAIN_A2_Q2_BLOCK_H
#define BLOCKCHAIN_A2_Q2_BLOCK_H

#include <utility>
#include <vector>
#include <string>
#include <iomanip>
#include <cstdint>
#include <sstream>
#include "transaction.h"
#include <openssl/sha.h>

class Block {
public:
    // genesis block
    Block()
            : index(0), previous_hash("0"), timestamp(std::time(nullptr)) {
        hash = calculate_hash();
    }

    // constructor
    Block(uint64_t index, std::string previous_hash, const std::vector<Transaction> &transactions)
            : index(index), previous_hash(std::move(previous_hash)), timestamp(std::time(nullptr)),
              transactions(transactions) {
        hash = calculate_hash();
    }

    // getter
    [[nodiscard]] uint64_t get_index() const { return index; }

    [[nodiscard]] std::string get_previous_hash() const { return previous_hash; }

    [[nodiscard]] std::string get_hash() const { return hash; }

    [[nodiscard]] std::time_t get_timestamp() const { return timestamp; }

    [[nodiscard]] const std::vector<Transaction> &get_transactions() const { return transactions; }

private:
    uint64_t index;
    std::string previous_hash;
    std::string hash;
    std::time_t timestamp;
    std::vector<Transaction> transactions;

    // calculate the hash of the block
    [[nodiscard]] std::string calculate_hash() const {
        std::string data = std::to_string(index) + previous_hash + std::to_string(timestamp);

        for (const auto &tx: transactions) {
            data += std::to_string(static_cast<int>(tx.get_type())) +
                    std::to_string(tx.get_nft_id()) +
                    tx.get_nft_name() +
                    tx.get_from_username() +
                    tx.get_to_username() +
                    std::to_string(tx.get_timestamp());
        }

        // SHA-256
        unsigned char hash_result[SHA256_DIGEST_LENGTH];
        SHA256(reinterpret_cast<const unsigned char *>(data.c_str()), data.size(), hash_result);

        // convert hash to hex
        std::stringstream ss;
        for (unsigned char i: hash_result) {
            ss << std::hex << std::setw(2) << std::setfill('0') << (int) i;
        }
        return ss.str();
    }
};

#endif //BLOCKCHAIN_A2_Q2_BLOCK_H
