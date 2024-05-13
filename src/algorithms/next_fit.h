#pragma once

#include "solver.h"

#include "../library/utils.h"

class NFDSolver : public ISolver {
public:
    explicit NFDSolver(const VolumeFunction& volume);

    Bins Solve(Items items, const Size& binCapacity) override;

private:
    VolumeFunction Volume_;
};

SolverPtr CreateNFDSolver(const VolumeFunction& volume);
