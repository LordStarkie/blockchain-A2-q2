#ifndef BLOCKCHAIN_A2_Q2_NFT_H
#define BLOCKCHAIN_A2_Q2_NFT_H

#include <string>
#include <atomic>
#include <utility>

class NFT {
public:
    NFT(std::string name, std::string description)
            : name(std::move(name)), description(std::move(description)), unique_id(generate_unique_id()) {}

    // getter
    [[nodiscard]] std::string get_name() const { return name; }
    [[nodiscard]] std::string get_description() const { return description; }
    [[nodiscard]] uint64_t get_unique_id() const { return unique_id; }

private:
    std::string name;
    std::string description;
    uint64_t unique_id;

    static uint64_t generate_unique_id() {
        static std::atomic<uint64_t> counter{0};
        return ++counter;
    }
};

#endif //BLOCKCHAIN_A2_Q2_NFT_H
