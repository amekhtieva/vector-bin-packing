#include "next_fit.h"

#include <optional>

NFDSolver::NFDSolver(const VolumeFunction& volume)
    : Volume_(volume)
{}

Bins NFDSolver::Solve(Items items, const Size& binCapacity) {
    Bins bins;
    SortBy(items, Volume_);

    bins.emplace_back(binCapacity);
    for (auto& item: items) {
        auto& current = bins.back();
        if (current.CanAddItem(item)) {
            current.AddItem(item);
        } else {
            bins.emplace_back(binCapacity);
            bins.back().AddItem(item);
        }
    }

    return bins;
}


SolverPtr CreateNFDSolver(const VolumeFunction& volume) {
    return std::make_shared<NFDSolver>(volume);
}
