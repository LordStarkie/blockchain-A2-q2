#ifndef BLOCKCHAIN_A2_Q2_TRANSACTION_H
#define BLOCKCHAIN_A2_Q2_TRANSACTION_H

#include <string>
#include <cstdint>
#include <utility>
#include "../nft/nft.h"
#include "../user/user.h"

enum class TransactionType {
    Mint,
    Transfer
};

class Transaction {
public:
    Transaction(TransactionType type, uint64_t nft_id, std::string  nft_name,
                const std::string& from_username, std::string  to_username)
            : type(type), nft_id(nft_id), nft_name(std::move(nft_name)),
              from_username(from_username), to_username(std::move(to_username)),
              timestamp(std::time(nullptr)) {}

    // Getters
    [[nodiscard]] TransactionType get_type() const { return type; }
    [[nodiscard]] uint64_t get_nft_id() const { return nft_id; }
    [[nodiscard]] std::string get_nft_name() const { return nft_name; }
    [[nodiscard]] std::string get_from_username() const { return from_username; }
    [[nodiscard]] std::string get_to_username() const { return to_username; }
    [[nodiscard]] std::time_t get_timestamp() const { return timestamp; }

private:
    TransactionType type;
    uint64_t nft_id;
    std::string nft_name;
    std::string from_username;
    std::string to_username;
    std::time_t timestamp;
};

#endif //BLOCKCHAIN_A2_Q2_TRANSACTION_H
