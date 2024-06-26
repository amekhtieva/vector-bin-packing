#pragma once

#include "solver.h"

#include "../library/utils.h"

class FFDSolver : public ISolver {
public:
    explicit FFDSolver(const VolumeFunction& volume);

    Bins Solve(Items items, const Size& binCapacity) override;

private:
    VolumeFunction Volume_;
};

SolverPtr CreateFFDSolver(const VolumeFunction& volume);
