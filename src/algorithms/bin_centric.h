#pragma once

#include "solver.h"

#include "../library/utils.h"

using MaxFitFunction = std::function<std::optional<size_t>(const Items&, const Bin&)>;

class BinCentricSolver : public ISolver {
public:
    explicit BinCentricSolver(const MaxFitFunction& maxFit);

    Bins Solve(Items items, const Size& binCapacity) override;

private:
    MaxFitFunction MaxFit_;
};

// Max fit functions for Bin Centric Solver

std::optional<size_t> MaxFitByDotProduct(const Items& items, const Bin& currentBin);
std::optional<size_t> MaxFitByL1Norm(const Items& items, const Bin& currentBin);
std::optional<size_t> MaxFitByL2Norm(const Items& items, const Bin& currentBin);
std::optional<size_t> MaxFitByLInfNorm(const Items& items, const Bin& currentBin);


SolverPtr CreateBinCentricSolver(const MaxFitFunction& maxFit);
