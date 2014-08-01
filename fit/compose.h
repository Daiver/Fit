/*=============================================================================
    Copyright (c) 2014 Paul Fultz II
    compose.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef FIT_GUARD_FUNCTION_COMPOSE_H
#define FIT_GUARD_FUNCTION_COMPOSE_H

/// compose
/// ======
/// 
/// Description
/// -----------
/// 
/// The `compose` function adaptor provides function composition. It produces
/// a function object that composes a set of functions, ie the output of one
/// function becomes the input of the second function. So, `compose(f, g)(0)`
/// is equivalent to `f(g(0))`.
/// 
/// 
/// Synopsis
/// --------
/// 
///     template<class F1, class F2, ...>
///     compose_adaptor<F1, F2, ...> compose(F1 f1, F2 f2, ...);
/// 
/// Example
/// -------
/// 
///     struct increment
///     {
///         template<class T>
///         T operator()(T x) const
///         {
///             return x + 1;
///         }
///     };
/// 
///     struct decrement
///     {
///         template<class T>
///         T operator()(T x) const
///         {
///             return x - 1;
///         }
///     };
/// 
///     int r = compose(increment(), decrement(), increment())(3);
///     assert(r == 4);
/// 

#include <fit/returns.h>
#include <fit/always.h>
#include <tuple>

namespace fit { namespace detail {

// TODO: Try to use inheritance but make each base class unique
template<class F1, class F2>
struct compose_kernel
{
    F1 f1;
    F2 f2;
    constexpr compose_kernel()
    {}

    template<class A, class B>
    constexpr compose_kernel(A f1, B f2) : f1(f1), f2(f2)
    {}


    template<class... Ts>
    constexpr auto operator()(Ts&&... xs) const FIT_RETURNS
    (
        this->f1(this->f2(std::forward<Ts>(xs)...))
    );
};
}

template<class F, class... Fs>
struct compose_adaptor : detail::compose_kernel<F, compose_adaptor<Fs...>>
{
    typedef compose_adaptor<Fs...> tail;
    typedef detail::compose_kernel<F, tail> base;
    // std::tuple<Fs...> fs;
    // static const long N = sizeof...(Fs);
    constexpr compose_adaptor() {}

    template<class X, class... Xs>
    constexpr compose_adaptor(X f1, Xs ... fs) 
    : base(f1, tail(fs...))
    {}
};

template<class F>
struct compose_adaptor<F> : F
{
    constexpr compose_adaptor() {}

    template<class X>
    constexpr compose_adaptor(X f1) 
    : F(f1)
    {}

};

template<class... Fs>
constexpr compose_adaptor<Fs...> compose(Fs... fs)
{
    return compose_adaptor<Fs...>(fs...);
}

}

#endif
