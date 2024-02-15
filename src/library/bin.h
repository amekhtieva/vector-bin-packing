#pragma once

#include "item.h"
#include "size.h"

#include <vector>

class Bin {
public:
    explicit Bin(const Size& capacity);

    const Size& GetResidualCapacity() const;
    const std::vector<uint32_t>& GetIdsOfContainedItems() const;

    bool CanAddItem(const Item& item) const;
    void AddItem(Item& item);

private:
    Size Capacity_;
    std::vector<uint32_t> ContainedItemIds_;
};

using Bins = std::vector<Bin>;
