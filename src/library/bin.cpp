#include "bin.h"

Bin::Bin(const Size& capacity)
    : Capacity_(capacity)
{}

const Size& Bin::GetResidualCapacity() const {
    return Capacity_;
}

const std::vector<uint32_t>& Bin::GetIdsOfContainedItems() const {
    return ContainedItemIds_;
}

bool Bin::CanAddItem(const Item& item) const {
    return item.GetSize() <= Capacity_;
}

void Bin::AddItem(Item& item) {
    Capacity_ -= item.GetSize();
    ContainedItemIds_.push_back(item.GetId());
    item.MarkPacked();
}
