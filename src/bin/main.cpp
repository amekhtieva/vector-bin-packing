#include "../library/bin.h"
#include "../library/item.h"
#include "../algorithms/solver.h"
#include "../algorithms/first_fit.h"

#include <iostream>

class InputReader {
public:
    InputReader() = default;

    void Read() {
        std::cout << "Enter number of items and dimension:" << std::endl;
        std::cin >> NumItems_ >> Dimension_;

        std::cout << "Enter bin capacity:" << std::endl;
        BinCapacity_.resize(Dimension_);
        for (auto& element : BinCapacity_) {
            std::cin >> element;
        }

        std::cout << "Enter item sizes:" << std::endl;
        for (size_t i = 0; i < NumItems_; ++i) {
            Size itemSize(Dimension_);
            for (auto& element : itemSize) {
                std::cin >> element;
            }
            Items_.emplace_back(i, itemSize);
        }

        std::cout << "Enter algorithm number, available algorithm types:" << std::endl;
        for (size_t i = 0; i < AlgorithmTypes_.size(); ++i) {
            std::cout << i + 1 << ". " << AlgorithmTypes_[i] << std::endl;
        }

        uint32_t algorithmNumber;
        std::cin >> algorithmNumber;

        switch (algorithmNumber)
        {
        case 1:
            Solver_ = CreateItemCentricFFDSolver(SortBySum);
            break;
        case 2:
            Solver_ = CreateItemCentricFFDSolver(SortByProduct);
            break;
        case 3:
            Solver_ = CreateBinCentricFFDSolver(DotProductMax);
            break;
        case 4:
            Solver_ = CreateBinCentricFFDSolver(L2NormMax);
            break;
        }
    }

    uint32_t GetNumberOfItems() const {
        return NumItems_;
    }

    uint32_t GetDimension() const {
        return Dimension_;
    }

    Items& GetItems() {
        return Items_;
    }

    const Size& GetBinCapacity() const {
        return BinCapacity_;
    }

    SolverPtr GetSolver() const {
        return Solver_;
    }

private:
    uint32_t NumItems_;
    uint32_t Dimension_;
    Items Items_;
    Size BinCapacity_;
    SolverPtr Solver_;

    const std::vector<std::string> AlgorithmTypes_ = {
        "Item centric first fit (sorting by sum of elements)",
        "Item centric first fit (sorting by product of elements)",
        "Bin centric first fit (max element by dot product)",
        "Bin centric first fit (max element by L2 norm)",
    };
};

void PrintPackingResult(const Bins& bins) {
    std::cout << "Number of required bins: " << bins.size() << std::endl;
    std::cout << "Items by bins" << std::endl;
    for (size_t i = 0; i < bins.size(); ++i) {
        std::cout << i + 1 << " bin:";
        const auto& itemIds = bins[i].GetIdsOfContainedItems();
        for (uint32_t id : itemIds) {
            std::cout << " " << id;
        }
        std::cout << std::endl;
    }
}

int main() {
    InputReader inputReader;
    inputReader.Read();

    SolverPtr solver = inputReader.GetSolver();;
    Bins result = solver->Solve(inputReader.GetItems(), inputReader.GetBinCapacity());

    PrintPackingResult(result);
    return 0;
}
