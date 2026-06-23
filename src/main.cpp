#include <iostream>
#include <cassert>
#include "../include/containers/VTensor.hpp"

using Vlib::VTensor;

int main() {
    // Constructor + metadata
    VTensor<int> t({2, 3, 4});

    assert(t.size() == 24);
    assert(t.rank() == 3);
    assert((t.shape() == std::vector<std::size_t>{2, 3, 4}));
    assert((t.strides() == std::vector<std::size_t>{12, 4, 1}));

    // Indexing
    int value = 0;
    for (std::size_t i = 0; i < 2; ++i)
        for (std::size_t j = 0; j < 3; ++j)
            for (std::size_t k = 0; k < 4; ++k)
                t({i, j, k}) = value++;

    assert(t({0, 0, 0}) == 0);
    assert(t({0, 0, 1}) == 1);
    assert(t({0, 1, 0}) == 4);
    assert(t({1, 0, 0}) == 12);
    assert(t({1, 2, 3}) == 23);

    // Fill
    t.fill(7);
    for (std::size_t i = 0; i < t.size(); ++i)
        assert(t.data()[i] == 7);

    // Copy constructor: deep copy
    VTensor<int> copy(t);
    assert(copy.size() == t.size());
    assert(copy.shape() == t.shape());
    assert(copy.strides() == t.strides());
    assert(copy({1, 2, 3}) == 7);

    copy({1, 2, 3}) = 99;
    assert(copy({1, 2, 3}) == 99);
    assert(t({1, 2, 3}) == 7);

    // Copy assignment
    VTensor<int> assigned({1});
    assigned = copy;
    assert(assigned.shape() == copy.shape());
    assert(assigned({1, 2, 3}) == 99);

    assigned({0, 0, 0}) = 123;
    assert(copy({0, 0, 0}) == 7); // deep copy check

    // Move constructor
    VTensor<int> moved(std::move(assigned));
    assert(moved.shape() == std::vector<std::size_t>({2, 3, 4}));
    assert(moved({1, 2, 3}) == 99);
    assert(moved({0, 0, 0}) == 123);

    // Move assignment
    VTensor<int> moved_assign({2, 2});
    moved_assign = std::move(moved);
    assert(moved_assign.shape() == std::vector<std::size_t>({2, 3, 4}));
    assert(moved_assign({1, 2, 3}) == 99);
    assert(moved_assign({0, 0, 0}) == 123);

    // Reshape
    moved_assign.reshape({4, 6});
    assert(moved_assign.size() == 24);
    assert(moved_assign.rank() == 2);
    assert((moved_assign.shape() == std::vector<std::size_t>{4, 6}));
    assert((moved_assign.strides() == std::vector<std::size_t>{6, 1}));

    // Same memory, new interpretation
    assert(moved_assign({0, 0}) == 123);
    assert(moved_assign({3, 5}) == 99);

    // Invalid reshape
    bool caught = false;
    try {
        moved_assign.reshape({5, 5});
    } catch (const std::invalid_argument&) {
        caught = true;
    }
    assert(caught);

    // Wrong rank
    caught = false;
    try {
        moved_assign({0, 0, 0});
    } catch (const std::invalid_argument&) {
        caught = true;
    }
    assert(caught);

    // Out of range
    caught = false;
    try {
        moved_assign({4, 0});
    } catch (const std::out_of_range&) {
        caught = true;
    }
    assert(caught);

    std::cout << "All VTensor tests passed.\n";
    return 0;
}