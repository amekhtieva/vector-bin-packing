#pragma once

#include "solver.h"

#include "../library/utils.h"

class WFDSolver : public ISolver {
public:
    explicit WFDSolver(const VolumeFunction& volume);

    Bins Solve(Items items, const Size& binCapacity) override;

private:
    VolumeFunction Volume_;
};

SolverPtr CreateWFDSolver(const VolumeFunction& volume);
