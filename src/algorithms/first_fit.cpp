#include "first_fit.h"

FFDSolver::FFDSolver(const VolumeFunction& volume)
    : Volume_(volume)
{}

Bins FFDSolver::Solve(Items items, const Size& binCapacity) {
    Bins bins;
    SortBy(items, Volume_);

    for (auto& item: items) {
        for (auto& bin : bins) {
            if (bin.CanAddItem(item)) {
                bin.AddItem(item);
                break;
            }
        }

        if (!item.IsPacked()) {
            bins.emplace_back(binCapacity);
            bins.back().AddItem(item);
        }
    }

    return bins;
}


SolverPtr CreateFFDSolver(const VolumeFunction& volume) {
    return std::make_shared<FFDSolver>(volume);
}
