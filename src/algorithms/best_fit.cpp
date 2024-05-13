#include "best_fit.h"

#include <optional>

BFDSolver::BFDSolver(const VolumeFunction& volume)
    : Volume_(volume)
{}

Bins BFDSolver::Solve(Items items, const Size& binCapacity) {
    Bins bins;
    SortBy(items, Volume_);

    for (auto& item: items) {
        std::optional<size_t> bestFitBinIndex = MinimizeFitFunction(
            item,
            bins,
            [this](const Item& item, const Bin& bin) {
                return Volume_(bin.GetResidualCapacity() - item.GetSize());
            }
        );

        if (bestFitBinIndex.has_value()) {
            bins[bestFitBinIndex.value()].AddItem(item);
        } else {
            bins.emplace_back(binCapacity);
            bins.back().AddItem(item);
        }
    }

    return bins;
}


SolverPtr CreateBFDSolver(const VolumeFunction& volume) {
    return std::make_shared<BFDSolver>(volume);
}
