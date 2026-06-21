#include <cassert>
#include <iostream>
#include <stdexcept>
#include <utility>

#include "../include/containers/VMatrix.hpp"
#include "../include/containers/VVector.hpp"

#include "../include/memory/VSharedPointer.hpp"

int main()
{
    using Vlib::Matrix;

    // constructor, rows, cols, size, empty
    Matrix<int> a(2, 3);
    assert(a.rows() == 2);
    assert(a.cols() == 3);
    assert(a.size() == 6);
    assert(!a.empty());

    // fill + operator()
    a.fill(1);
    assert(a(0, 0) == 1);
    assert(a(1, 2) == 1);

    a(0, 0) = 1;
    a(0, 1) = 2;
    a(0, 2) = 3;
    a(1, 0) = 4;
    a(1, 1) = 5;
    a(1, 2) = 6;

    std::cout << "Matrix a:\n" << a << '\n';

    // at()
    assert(a.at(1, 2) == 6);

    bool threw = false;
    try {
        a.at(2, 0);
    } catch (const std::out_of_range&) {
        threw = true;
    }
    assert(threw);

    // copy constructor: deep copy
    Matrix<int> b(a);
    b(0, 0) = 100;
    assert(a(0, 0) == 1);
    assert(b(0, 0) == 100);

    // move constructor
    Matrix<int> c(std::move(b));
    assert(c(0, 0) == 100);
    assert(b.empty());

    // copy assignment / copy-and-swap
    Matrix<int> d(1, 1);
    d = a;
    assert(d.rows() == 2);
    assert(d.cols() == 3);
    assert(d(1, 2) == 6);

    d(1, 2) = 999;
    assert(a(1, 2) == 6);
    assert(d(1, 2) == 999);

    // move assignment
    Matrix<int> e(1, 1);
    e = std::move(d);
    assert(e.rows() == 2);
    assert(e.cols() == 3);
    assert(e(1, 2) == 999);
    assert(d.empty());

    // addition
    Matrix<int> x(2, 2);
    x(0, 0) = 1; x(0, 1) = 2;
    x(1, 0) = 3; x(1, 1) = 4;

    Matrix<int> y(2, 2);
    y(0, 0) = 10; y(0, 1) = 20;
    y(1, 0) = 30; y(1, 1) = 40;

    Matrix<int> sum = x + y;
    assert(sum(0, 0) == 11);
    assert(sum(0, 1) == 22);
    assert(sum(1, 0) == 33);
    assert(sum(1, 1) == 44);

    // subtraction
    Matrix<int> diff = y - x;
    assert(diff(0, 0) == 9);
    assert(diff(0, 1) == 18);
    assert(diff(1, 0) == 27);
    assert(diff(1, 1) == 36);

    // += and -=
    x += y;
    assert(x(0, 0) == 11);
    assert(x(1, 1) == 44);

    x -= y;
    assert(x(0, 0) == 1);
    assert(x(1, 1) == 4);

    // scalar multiplication
    Matrix<int> scaled = x * 5;
    assert(scaled(0, 0) == 5);
    assert(scaled(0, 1) == 10);
    assert(scaled(1, 0) == 15);
    assert(scaled(1, 1) == 20);

    // matrix multiplication
    Matrix<int> m1(2, 3);
    m1(0, 0) = 1; m1(0, 1) = 2; m1(0, 2) = 3;
    m1(1, 0) = 4; m1(1, 1) = 5; m1(1, 2) = 6;

    Matrix<int> m2(3, 2);
    m2(0, 0) = 7;  m2(0, 1) = 8;
    m2(1, 0) = 9;  m2(1, 1) = 10;
    m2(2, 0) = 11; m2(2, 1) = 12;

    Matrix<int> product = m1 * m2;

    assert(product.rows() == 2);
    assert(product.cols() == 2);
    assert(product(0, 0) == 58);
    assert(product(0, 1) == 64);
    assert(product(1, 0) == 139);
    assert(product(1, 1) == 154);

    std::cout << "Product:\n" << product << '\n';

    // identity
    Matrix<int> I = Matrix<int>::identity(3);
    assert(I.rows() == 3);
    assert(I.cols() == 3);
    assert(I(0, 0) == 1);
    assert(I(1, 1) == 1);
    assert(I(2, 2) == 1);
    assert(I(0, 1) == 0);

    // iterators
    int total = 0;
    for (const auto& value : product) {
        total += value;
    }
    assert(total == 58 + 64 + 139 + 154);

    // clear
    product.clear();
    assert(product.empty());
    assert(product.size() == 0);
    assert(product.data() == nullptr);

    // empty matrices
    Matrix<int> empty1(0, 5);
    Matrix<int> empty2(5, 0);

    assert(empty1.empty());
    assert(empty2.empty());
    assert(empty1.rows() == 0);
    assert(empty1.cols() == 5);
    assert(empty2.rows() == 5);
    assert(empty2.cols() == 0);

    std::cout << "All Matrix tests passed.\n";

    return 0;
}