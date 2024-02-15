#include "first_fit.h"

#include <algorithm>

ItemCentricFFDSolver::ItemCentricFFDSolver(const SortFunction& sort)
    : Sort_(sort)
{}

Bins ItemCentricFFDSolver::Solve(Items items, const Size& binCapacity) {
    Bins bins;
    Sort_(items);

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


BinCentricFFDSolver::BinCentricFFDSolver(const MaxFitFunction& maxFit)
    : MaxFit_(maxFit)
{}

Bins BinCentricFFDSolver::Solve(Items items, const Size& binCapacity) {
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


void SortBySum(Items& items) {
    std::sort(
        items.begin(),
        items.end(),
        [](const Item& lhs, const Item& rhs) {
            return SumOfElements(lhs.GetSize()) > SumOfElements(rhs.GetSize());
        }
    );
}

void SortByProduct(Items& items) {
    std::sort(
        items.begin(),
        items.end(),
        [](const Item& lhs, const Item& rhs) {
            return ProductOfElements(lhs.GetSize()) > ProductOfElements(rhs.GetSize());
        }
    );
}


std::optional<size_t> DotProductMax(const Items& items, const Bin& currentBin) {
    std::optional<size_t> maxItemIndex;
    uint64_t maxValue;

    for (size_t i = 0; i < items.size(); ++i) {
        if (items[i].IsPacked() || !currentBin.CanAddItem(items[i])) {
            continue;
        }
        uint64_t dotProduct = DotProduct(items[i].GetSize(), currentBin.GetResidualCapacity());
        if (!maxItemIndex.has_value() || maxValue < dotProduct) {
            maxValue = dotProduct;
            maxItemIndex = i;
        }
    }

    return maxItemIndex;
}

std::optional<size_t> L2NormMax(const Items& items, const Bin& currentBin) {
    std::optional<size_t> minItemIndex;
    uint64_t minValue;

    for (size_t i = 0; i < items.size(); ++i) {
        if (items[i].IsPacked() || !currentBin.CanAddItem(items[i])) {
            continue;
        }
        uint64_t l2 = SumOfElements(Square(currentBin.GetResidualCapacity() - items[i].GetSize()));
        if (!minItemIndex.has_value() || l2 < minValue) {
            minValue = l2;
            minItemIndex = i;
        }
    }

    return minItemIndex;
}


SolverPtr CreateItemCentricFFDSolver(const SortFunction& sort) {
    return std::make_shared<ItemCentricFFDSolver>(sort);
}

SolverPtr CreateBinCentricFFDSolver(const MaxFitFunction& maxFit) {
    return std::make_shared<BinCentricFFDSolver>(maxFit);
}
