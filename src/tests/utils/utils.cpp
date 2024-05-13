#include "utils.h"

#include "../../algorithms/best_fit.h"
#include "../../algorithms/bin_centric.h"
#include "../../algorithms/first_fit.h"
#include "../../algorithms/my.h"
#include "../../algorithms/next_fit.h"
#include "../../algorithms/worst_fit.h"

#include <algorithm>
#include <chrono>
#include <numeric>
#include <random>

Items CreateItemsBySizes(std::vector<Size> itemSizes) {
    Items items;
    for (size_t i = 0; i < itemSizes.size(); ++i) {
        items.emplace_back(i, itemSizes[i]);
    }
    return items;
}

void TryFit(Bins& bins, const Size& capacity, Items& items, size_t currentItem, uint64_t& optimal) {
    if (currentItem == items.size()) {
        optimal = std::min<uint64_t>(optimal, bins.size());
        return;
    }

    if (bins.size() >= optimal) {
        return;
    }

    for (size_t i = 0; i < bins.size(); ++i) {
        if (bins[i].CanAddItem(items[currentItem])) {
            bins[i].AddItem(items[currentItem]);
            TryFit(bins, capacity, items, currentItem + 1, optimal);
            bins[i].RemoveItem(items[currentItem]);
        }
    }

    bins.emplace_back(capacity);
    bins.back().AddItem(items[currentItem]);
    TryFit(bins, capacity, items, currentItem + 1, optimal);
    bins.back().RemoveItem(items[currentItem]);
    bins.pop_back();
}

uint64_t LowerBound(const Size& capacity, const Items& items) {
    uint64_t lowerBound = 0;
    for (size_t i = 0; i < capacity.size(); ++i) {
        uint64_t sum1d = 0;
        for (const auto& item : items) {
            sum1d += item.GetSize()[i];
        }
        uint64_t lowerBound1d = (sum1d + capacity[i] - 1) / capacity[i];
        lowerBound = std::max(lowerBound, lowerBound1d);
    }
    return lowerBound;
}

uint64_t Optimal(const Size& capacity, Items items) {
    Bins bins;
    bins.reserve(items.size());
    uint64_t result = items.size();
    TryFit(bins, capacity, items, 0, result);
    return result;
}

uint64_t UpperBound(const Size&, const Items& items) {
    return items.size();
}

Size GenerateCapacity(uint64_t dimension, uint64_t min, uint64_t max) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<uint64_t> dist(min, max);

    Size capacity(dimension);
    for (auto& element : capacity) {
        element = dist(gen);
    }
    return capacity;
}

Items GenerateItems(uint64_t num, uint64_t, const Size& capacity) {
    std::vector<Size> itemSizes(num, Size(capacity));

    for (size_t i = 0; i < capacity.size(); ++i) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::normal_distribution<double> dist(capacity[i] / 3, capacity[i] / 10);

        for (auto& itemSize : itemSizes) {
            itemSize[i] = std::clamp<uint64_t>(dist(gen), 1, std::round(capacity[i]));
        }
    }

    return CreateItemsBySizes(itemSizes);
}

std::vector<Size> GenerateTriplet(const Size& capacity) {
    std::vector<Size> itemSizes(3, Size(capacity));

    for (size_t i = 0; i < capacity.size(); ++i) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<uint64_t> dist(0, capacity[i]);
        uint64_t firstGenerated = dist(gen);
        uint64_t secondGenerated = dist(gen);

        uint64_t firstSplit = std::min(firstGenerated, secondGenerated);
        uint64_t secondSplit = std::max(firstGenerated, secondGenerated);
        itemSizes[0][i] = firstSplit;
        itemSizes[1][i] = secondSplit - firstSplit;
        itemSizes[2][i] = capacity[i] - secondSplit;
    }

    return itemSizes;
}

Items GenerateTriplets(uint64_t numberOfTriplets, const Size& capacity) {
    std::vector<Size> itemSizes;
    itemSizes.reserve(3 * numberOfTriplets);

    for (size_t i = 0; i < numberOfTriplets; ++i) {
        auto newItemSizes = GenerateTriplet(capacity);
        itemSizes.insert(itemSizes.end(), newItemSizes.begin(), newItemSizes.end());
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::shuffle(itemSizes.begin(), itemSizes.end(), gen);

    return CreateItemsBySizes(itemSizes);
}

std::pair<uint64_t, double> RunSolver(SolverPtr solver, const Size& capacity, const Items& items) {
    auto start = std::chrono::high_resolution_clock::now();
    Bins bins = solver->Solve(items, capacity);
    auto stop = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double, std::milli> doubleMs = stop - start;

    return std::make_pair(bins.size(), doubleMs.count());
}

double Error(uint64_t lowerBoundOrOptimal, uint64_t algorithmResult) {
    double error = static_cast<double>(algorithmResult - lowerBoundOrOptimal) / lowerBoundOrOptimal;
    return error * 100;
}

double Improvement(uint64_t upperBound, uint64_t algorithmResult) {
    double improvement = (static_cast<double>(upperBound - algorithmResult) / algorithmResult);
    return improvement * 100;
}

std::vector<double> ToPercentage(std::vector<uint64_t> algorithmResults) {
    std::vector<double> percentage;
    uint64_t minValue = *std::min_element(algorithmResults.begin(), algorithmResults.end());

    for (uint64_t algorithmResult : algorithmResults) {
        percentage.push_back(Error(minValue, algorithmResult));
    }

    return percentage;
}

double Average(std::vector<double> elements) {
    return std::accumulate(elements.begin(), elements.end(), 0.0) / elements.size();
}

SolverPtr CreateSolverByAlgorithmName(const std::string& algorithmName) {
    if (algorithmName == "BFD_L1") {
        return CreateBFDSolver(L1);
    } else if (algorithmName == "BFD_L2") {
        return CreateBFDSolver(L2);
    } else if (algorithmName == "BFD_LInf") {
        return CreateBFDSolver(LInf);
    } else if (algorithmName == "FFD_L1") {
        return CreateFFDSolver(L1);
    } else if (algorithmName == "FFD_L2") {
        return CreateFFDSolver(L2);
    } else if (algorithmName == "FFD_LInf") {
        return CreateFFDSolver(LInf);
    } else if (algorithmName == "NFD_L1") {
        return CreateNFDSolver(L1);
    } else if (algorithmName == "NFD_L2") {
        return CreateNFDSolver(L2);
    } else if (algorithmName == "NFD_LInf") {
        return CreateNFDSolver(LInf);
    } else if (algorithmName == "WFD_L1") {
        return CreateWFDSolver(L1);
    } else if (algorithmName == "WFD_L2") {
        return CreateWFDSolver(L2);
    } else if (algorithmName == "WFD_LInf") {
        return CreateWFDSolver(LInf);
    } else if (algorithmName == "BC_DP") {
        return CreateBinCentricSolver(MaxFitByDotProduct);
    } else if (algorithmName == "BC_L1") {
        return CreateBinCentricSolver(MaxFitByL1Norm);
    } else if (algorithmName == "BC_L2") {
        return CreateBinCentricSolver(MaxFitByL2Norm);
    } else if (algorithmName == "BC_LInf") {
        return CreateBinCentricSolver(MaxFitByLInfNorm);
    } else if (algorithmName == "MY_L1") {
        return CreateMySolver(L1);
    } else if (algorithmName == "MY_L2") {
        return CreateMySolver(L2);
    } else if (algorithmName == "MY_LInf") {
        return CreateMySolver(LInf);
    } else {
        return nullptr;
    }
}
