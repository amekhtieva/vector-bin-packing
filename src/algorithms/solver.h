#pragma once

#include "../library/bin.h"
#include "../library/item.h"

#include <memory>

class ISolver {
public:
    virtual Bins Solve(Items items, const Size& binCapacity) = 0;
    virtual ~ISolver() = default;
};

using SolverPtr = std::shared_ptr<ISolver>;
