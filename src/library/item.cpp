#include "item.h"

Item::Item(uint32_t id, const Size& size)
    : Id_(id)
    , Size_(size)
{}

uint32_t Item::GetId() const {
    return Id_;
}

const Size& Item::GetSize() const {
    return Size_;
}

void Item::MarkPacked() {
    Packed_ = true;
}

bool Item::IsPacked() const {
    return Packed_;
}
