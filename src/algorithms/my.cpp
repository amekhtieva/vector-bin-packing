#include "my.h"

#include <optional>
#include <vector>

std::optional<size_t> MySolver::FindBestFitBin(const Item& item, const Bins& bins) {
    return MinimizeFitFunction(
        item,
        bins,
        [this](const Item& item, const Bin& bin) {
            return Volume_(bin.GetResidualCapacity() - item.GetSize());
        }
    );
}

std::optional<size_t> MySolver::FindFirstFitBin(const Item& item, const Bins& bins) {
    for (size_t i = 0; i < bins.size(); ++i) {
        if (bins[i].CanAddItem(item)) {
            return i;
        }
    }
    return std::nullopt;
}

std::optional<size_t> MySolver::FindWorstFitBin(const Item& item, const Bins& bins) {
    return MaximizeFitFunction(
        item,
        bins,
        [this](const Item& item, const Bin& bin) {
            return Volume_(bin.GetResidualCapacity() - item.GetSize());
        }
    );
}

void MySolver::RecursiveSearch(
    Bins& bins,
    const Size& capacity,
    Items& items,
    size_t currentItem,
    Bins& resultBins
) {
    if (currentItem == items.size()) {
        if (bins.size() < resultBins.size()) {
            resultBins = bins;
        }
        return;
    }

    if (bins.size() >= resultBins.size()) {
        return;
    }

    std::vector<std::optional<size_t>> consideredIndices = {
        FindBestFitBin(items[currentItem], bins),
        FindFirstFitBin(items[currentItem], bins),
        FindWorstFitBin(items[currentItem], bins),
    };

    for (auto binIndex : consideredIndices) {
        if (binIndex.has_value()) {
            bins[binIndex.value()].AddItem(items[currentItem]);
            RecursiveSearch(bins, capacity, items, currentItem + 1, resultBins);
            bins[binIndex.value()].RemoveItem(items[currentItem]);
        }
    }

    bins.emplace_back(capacity);
    bins.back().AddItem(items[currentItem]);
    RecursiveSearch(bins, capacity, items, currentItem + 1, resultBins);
    bins.back().RemoveItem(items[currentItem]);
    bins.pop_back();
}

MySolver::MySolver(const VolumeFunction& volume)
    : Volume_(volume)
{}

MySolver::MySolver(size_t numberOfItemsForRecursiveSearch, const VolumeFunction& volume)
    : Volume_(volume)
    , NumberOfItemsForRecursiveSearch_(numberOfItemsForRecursiveSearch)
{}

Bins MySolver::Solve(Items items, const Size& binCapacity) {
    Bins bins;
    SortBy(items, Volume_);

    size_t lastIndex = std::max(items.size(), NumberOfItemsForRecursiveSearch_)
                                                - NumberOfItemsForRecursiveSearch_;

    for (size_t i = 0; i < lastIndex; ++i) {
        std::optional<size_t> bestFitBinIndex = MinimizeFitFunction(
            items[i],
            bins,
            [this](const Item& item, const Bin& bin) {
                return Volume_(bin.GetResidualCapacity() - item.GetSize());
            }
        );

        if (bestFitBinIndex.has_value()) {
            bins[bestFitBinIndex.value()].AddItem(items[i]);
        } else {
            bins.emplace_back(binCapacity);
            bins.back().AddItem(items[i]);
        }
    }

    uint64_t result = items.size() - lastIndex;
    Bins resultBins(items.size(), Bin(binCapacity));
    RecursiveSearch(bins, binCapacity, items, lastIndex, resultBins);

    return resultBins;
}


SolverPtr CreateMySolver(const VolumeFunction& volume) {
    return std::make_shared<MySolver>(volume);
}

SolverPtr CreateMySolver(size_t numberOfItemsForRecursiveSearch, const VolumeFunction& volume) {
    return std::make_shared<MySolver>(numberOfItemsForRecursiveSearch, volume);
}

