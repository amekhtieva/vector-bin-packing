#include "worst_fit.h"

#include <optional>

WFDSolver::WFDSolver(const VolumeFunction& volume)
    : Volume_(volume)
{}

Bins WFDSolver::Solve(Items items, const Size& binCapacity) {
    Bins bins;
    SortBy(items, Volume_);

    for (auto& item: items) {
        std::optional<size_t> worstFitBinIndex = MaximizeFitFunction(
            item,
            bins,
            [this](const Item& item, const Bin& bin) {
                return Volume_(bin.GetResidualCapacity() - item.GetSize());
            }
        );

        if (worstFitBinIndex.has_value()) {
            bins[worstFitBinIndex.value()].AddItem(item);
        } else {
            bins.emplace_back(binCapacity);
            bins.back().AddItem(item);
        }
    }

    return bins;
}


SolverPtr CreateWFDSolver(const VolumeFunction& volume) {
    return std::make_shared<WFDSolver>(volume);
}
