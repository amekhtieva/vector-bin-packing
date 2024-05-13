#pragma once

#include "solver.h"

#include "../library/utils.h"

class MySolver : public ISolver {
public:
    explicit MySolver(const VolumeFunction& volume);
    MySolver(size_t numberOfItemsForRecursiveSearch, const VolumeFunction& volume);

    Bins Solve(Items items, const Size& binCapacity) override;

private:
    void RecursiveSearch(
        Bins& bins,
        const Size& capacity,
        Items& items,
        size_t currentItem,
        Bins& resultBins
    );

    std::optional<size_t> FindBestFitBin(const Item& item, const Bins& bins);
    std::optional<size_t> FindFirstFitBin(const Item& item, const Bins& bins);
    std::optional<size_t> FindWorstFitBin(const Item& item, const Bins& bins);

private:
    const size_t NumberOfItemsForRecursiveSearch_ = 15;
    VolumeFunction Volume_;
};

SolverPtr CreateMySolver(const VolumeFunction& volume);
SolverPtr CreateMySolver(size_t numberOfItemsForRecursiveSearch, const VolumeFunction& volume);
