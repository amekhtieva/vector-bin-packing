#include "utils.h"

#include <algorithm>
#include <functional>

void SortBy(Items& items, VolumeFunction volume) {
    std:sort(
        items.begin(),
        items.end(),
        [&volume](const Item& lhs, const Item& rhs) {
            return volume(lhs.GetSize()) > volume(rhs.GetSize());
        }
    );
}


template <typename Comparator>
std::optional<size_t> Fit(const Items& items, const Bin& bin, FitFunction fit, Comparator fitComparator) {
    std::optional<size_t> resFitItemIndex;
    uint64_t resFitValue;

    for (size_t i = 0; i < items.size(); ++i) {
        if (items[i].IsPacked() || !bin.CanAddItem(items[i])) {
            continue;
        }

        uint64_t fitValue = fit(items[i], bin);
        if (!resFitItemIndex.has_value() || fitComparator(fitValue, resFitValue)) {
            resFitValue = fitValue;
            resFitItemIndex = i;
        }
    }

    return resFitItemIndex;
}

std::optional<size_t> MaximizeFitFunction(const Items& items, const Bin& bin, FitFunction fit) {
    return Fit(items, bin, fit, std::greater<uint64_t>());
}

std::optional<size_t> MinimizeFitFunction(const Items& items, const Bin& bin, FitFunction fit) {
    return Fit(items, bin, fit, std::less<uint64_t>());
}


template <typename Comparator>
std::optional<size_t> Fit(const Item& item, const Bins& bins, FitFunction fit, Comparator fitComparator) {
    
    std::optional<size_t> resFitItemIndex;
    uint64_t resFitValue;

    for (size_t i = 0; i < bins.size(); ++i) {
        if (!bins[i].CanAddItem(item)) {
            continue;
        }

        uint64_t fitValue = fit(item, bins[i]);
        if (!resFitItemIndex.has_value() || fitComparator(fitValue, resFitValue)) {
            resFitValue = fitValue;
            resFitItemIndex = i;
        }
    }

    return resFitItemIndex;
}

std::optional<size_t> MaximizeFitFunction(const Item& item, const Bins& bins, FitFunction fit) {
    return Fit(item, bins, fit, std::greater<uint64_t>());
}

std::optional<size_t> MinimizeFitFunction(const Item& item, const Bins& bins, FitFunction fit) {
    return Fit(item, bins, fit, std::less<uint64_t>());
}
