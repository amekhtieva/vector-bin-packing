#include "bin_centric.h"

#include <algorithm>

BinCentricSolver::BinCentricSolver(const MaxFitFunction& maxFit)
    : MaxFit_(maxFit)
{}

Bins BinCentricSolver::Solve(Items items, const Size& binCapacity) {
    Bins bins;
    uint32_t numUnpackedItems = items.size();

    while (numUnpackedItems) {
        auto& bin = bins.emplace_back(binCapacity);
        auto maxItemIndex = MaxFit_(items, bin);

        while (maxItemIndex.has_value()) {
            auto& item = items[maxItemIndex.value()];
            bin.AddItem(item);
            --numUnpackedItems;

            maxItemIndex = MaxFit_(items, bin);
        }
    }

    return bins;
}


std::optional<size_t> MaxFitByDotProduct(const Items& items, const Bin& currentBin) {
    return MaximizeFitFunction(
        items,
        currentBin,
        [](const Item& item, const Bin& bin) {
            return DotProduct(item.GetSize(), bin.GetResidualCapacity());
        }
    );
}

std::optional<size_t> MaxFitByL1Norm(const Items& items, const Bin& currentBin) {
     return MinimizeFitFunction(
        items,
        currentBin,
        [](const Item& item, const Bin& bin) {
            return L1(bin.GetResidualCapacity() - item.GetSize());
        }
    );
}

std::optional<size_t> MaxFitByL2Norm(const Items& items, const Bin& currentBin) {
     return MinimizeFitFunction(
        items,
        currentBin,
        [](const Item& item, const Bin& bin) {
            return L2(bin.GetResidualCapacity() - item.GetSize());
        }
    );
}

std::optional<size_t> MaxFitByLInfNorm(const Items& items, const Bin& currentBin) {
     return MinimizeFitFunction(
        items,
        currentBin,
        [](const Item& item, const Bin& bin) {
            return LInf(bin.GetResidualCapacity() - item.GetSize());
        }
    );
}


SolverPtr CreateBinCentricSolver(const MaxFitFunction& maxFit) {
    return std::make_shared<BinCentricSolver>(maxFit);
}
