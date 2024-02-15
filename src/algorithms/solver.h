#pragma once

#include "../library/item.h"
#include "../library/bin.h"

class ISolver {
public:
    virtual Bins Solve(Items items, const Size& binCapacity) = 0;
    virtual ~ISolver() = default;
};

using SolverPtr = std::shared_ptr<ISolver>;
