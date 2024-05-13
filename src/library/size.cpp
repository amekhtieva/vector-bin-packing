#include "size.h"

#include <algorithm>

bool operator<(const Size& lhs, const Size& rhs) {
    for (size_t i = 0; i < lhs.size(); ++i) {
        if (lhs[i] >= rhs[i]) {
            return false;
        }
    }
    return true;
}

bool operator<=(const Size& lhs, const Size& rhs) {
    for (size_t i = 0; i < lhs.size(); ++i) {
        if (lhs[i] > rhs[i]) {
            return false;
        }
    }
    return true;
}

Size operator+(const Size& lhs, const Size& rhs) {
    Size result;
    result.reserve(lhs.size());

    for (size_t i = 0; i < lhs.size(); ++i) {
        result.push_back(lhs[i] + rhs[i]);
    }
    return result;
}

Size& operator+=(Size& lhs, const Size& rhs) {
    for (size_t i = 0; i < lhs.size(); ++i) {
        lhs[i] += rhs[i];
    }
    return lhs;
}

Size operator-(const Size& lhs, const Size& rhs) {
    Size result;
    result.reserve(lhs.size());

    for (size_t i = 0; i < lhs.size(); ++i) {
        result.push_back(lhs[i] - rhs[i]);
    }
    return result;
}

Size& operator-=(Size& lhs, const Size& rhs) {
    for (size_t i = 0; i < lhs.size(); ++i) {
        lhs[i] -= rhs[i];
    }
    return lhs;
}

Size operator*(const Size& lhs, const Size& rhs) {
    Size result;
    result.reserve(lhs.size());

    for (size_t i = 0; i < lhs.size(); ++i) {
        result.push_back(lhs[i] * rhs[i]);
    }
    return result;
}

Size& operator*=(Size& lhs, const Size& rhs) {
    for (size_t i = 0; i < lhs.size(); ++i) {
        lhs[i] *= rhs[i];
    }
    return lhs;
}

uint64_t SumOfElements(const Size& vec) {
    uint64_t result = 0;

    for (uint64_t element : vec) {
        result += element;
    }
    return result;
}

uint64_t ProductOfElements(const Size& vec) {
    uint64_t result = 1;

    for (uint64_t element : vec) {
        result *= element;
    }
    return result;
}

uint64_t DotProduct(const Size& lhs, const Size& rhs) {
    return SumOfElements(lhs * rhs);
}

Size Square(const Size& vec) {
    return vec * vec;
}

uint64_t L1(const Size& vec) {
    return SumOfElements(vec);
}

uint64_t L2(const Size& vec) {
    // without sqrt because it is only used for comparison
    return SumOfElements(Square(vec));
}

uint64_t LInf(const Size& vec) {
    return *std::max_element(vec.begin(), vec.end());
}
