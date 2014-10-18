#include <fit/by.h>
#include <fit/placeholders.h>
#include "test.h"

#include <memory>

struct foo
{
    constexpr foo(int x) : x(x)
    {}
    int x;
};

struct select_x
{
    template<class T>
    constexpr auto operator()(T&& x) const FIT_RETURNS(x.x);
};

FIT_TEST_CASE()
{
    constexpr auto add = fit::_ + fit::_;
    FIT_STATIC_TEST_CHECK(fit::by(select_x(), add)(foo(1), foo(2)) == 3);
    FIT_TEST_CHECK(fit::by(std::mem_fn(&foo::x), add)(foo(1), foo(2)) == 3);
}

struct select_x_1
{
    std::unique_ptr<int> i;
    select_x_1() : i(new int(2))
    {}
    template<class T>
    auto operator()(T&& x) const FIT_RETURNS(x.x * (*i));
};

struct sum_1
{
    std::unique_ptr<int> i;
    sum_1() : i(new int(1))
    {}
    template<class T, class U>
    auto operator()(T&& x, U&& y) const FIT_RETURNS(x + y + *i);
};

FIT_TEST_CASE()
{
    FIT_TEST_CHECK(fit::by(select_x_1(), sum_1())(foo(1), foo(2)) == 7);
}
