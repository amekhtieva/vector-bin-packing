#pragma once

#include "solver.h"

#include<memory>
#include <functional>
#include <optional>

using SortFunction = std::function<void(Items&)>;
using MaxFitFunction = std::function<std::optional<size_t>(const Items&, const Bin&)>;

// First Fit Solvers

class ItemCentricFFDSolver : public ISolver {
public:
    explicit ItemCentricFFDSolver(const SortFunction& sort);

    Bins Solve(Items items, const Size& binCapacity) override;

private:
    SortFunction Sort_;
};

class BinCentricFFDSolver : public ISolver {
public:
    explicit BinCentricFFDSolver(const MaxFitFunction& maxFit);

    Bins Solve(Items items, const Size& binCapacity) override;

private:
    MaxFitFunction MaxFit_;
};

// Sort functions for Item Centric First Fit Solver

void SortBySum(Items& items);
void SortByProduct(Items& items);

// Max functions for Bin Centric First Fit Solver

std::optional<size_t> DotProductMax(const Items& items, const Bin& currentBin);
std::optional<size_t> L2NormMax(const Items& items, const Bin& currentBin);


SolverPtr CreateItemCentricFFDSolver(const SortFunction& sort);
SolverPtr CreateBinCentricFFDSolver(const MaxFitFunction& maxFit);
