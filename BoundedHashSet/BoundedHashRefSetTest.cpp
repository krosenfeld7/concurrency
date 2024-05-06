
#include <iostream>
#include <vector>

#include "BoundedHashRefSet.hpp"


class BasicObject {
public:
    BasicObject(int id) : id(id) { }
    auto getId() const -> int { return id; }

    std::size_t operator()(const BasicObject &bObject) const { 
        return static_cast<std::size_t>(bObject.getId());
    }
    bool operator==(const BasicObject &other) const { 
        return id == other.getId();
    }

private:
    int id;
};

namespace std {
    template<>
    struct hash<BasicObject> {
        std::size_t operator()(const BasicObject& b) const {
            return std::hash<int>()(b.getId());
        }
    };
}

int main() {
    std::vector<BasicObject> basicObjects = std::vector<BasicObject>();

    for (auto i = 0; i < 10; i++) {
        basicObjects.emplace_back(i);
    }

    BoundedHashRefSet<BasicObject> boundedSet(5);

    for (auto i = 0; i < 10; i++) {
        std::cout << "Adding object with id: " << basicObjects[i].getId() <<
            " success: " << boundedSet.add(basicObjects[i]) << "\n";
    }

    for (auto i = 0; i < 10; i++) {
        std::cout << "Finding object with id: " << basicObjects[i].getId() <<
            " success: " << boundedSet.contains(basicObjects[i]) << "\n";
    }

    for (auto i = 0; i < 10; i++) {
        std::cout << "Removing object with id: " << basicObjects[i].getId() <<
            " success: " << boundedSet.remove(basicObjects[i]) << "\n";
    }

    std::cout.flush();
}