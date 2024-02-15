#pragma once

#include "size.h"

#include <vector>

class Item {
public:
    Item(uint32_t id, const Size& size);

    uint32_t GetId() const;
    const Size& GetSize() const;

    void MarkPacked();
    bool IsPacked() const;

private:
    uint32_t Id_;
    Size Size_;
    bool Packed_ = false;
};

using Items = std::vector<Item>;
