#pragma once

#include "solver.h"

#include "../library/utils.h"

class BFDSolver : public ISolver {
public:
    explicit BFDSolver(const VolumeFunction& volume);

    Bins Solve(Items items, const Size& binCapacity) override;

private:
    VolumeFunction Volume_;
};

SolverPtr CreateBFDSolver(const VolumeFunction& volume);
