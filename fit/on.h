/*=============================================================================
    Copyright (c) 2014 Paul Fultz II
    on.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef FIT_GUARD_FUNCTION_ON_H
#define FIT_GUARD_FUNCTION_ON_H

/// on
/// ==
/// 
/// Description
/// -----------
/// 
/// The `on` function adaptor applies a projection onto the parameters of
/// another function. This is useful, for example, to define a function for
/// sorting such that the ordering is based off of the value of one of its
/// member fields.
/// 
/// Synopsis
/// --------
/// 
///     template<class Projection, class F>
///     constexpr on_adaptor<Projection, F> on(Projection p, F f);
/// 
/// Requirements
/// ------------
/// 
/// Projection must be:
/// 
///     UnaryFunctionObject
///     MoveConstructible
/// 
/// F must be:
/// 
///     FunctionObject
///     MoveConstructible
/// 
/// Example
/// -------
/// 
///     struct foo
///     {
///         foo(int x) : x(x)
///         {}
///         int x;
///     };
///     assert(fit::on(std::mem_fn(&foo::x), _ + _)(foo(1), foo(2)) == 3);
/// 



#include <utility>
#include <fit/always.h>
#include <fit/detail/delegate.h>
#include <fit/returns.h>

namespace fit {

template<class Projection, class F>
struct on_adaptor : Projection, F
{
    template<class... Ts>
    constexpr const F& base_function(Ts&&... xs) const
    {
        return always_ref(*this)(xs...);
    }

    template<class... Ts>
    constexpr const Projection& base_projection(Ts&&... xs) const
    {
        return always_ref(*this)(xs...);
    }

    template<class P, class G, FIT_ENABLE_IF_CONVERTIBLE(P, Projection), FIT_ENABLE_IF_CONVERTIBLE(G, F)>
    constexpr on_adaptor(P&& p, G&& f) 
    : Projection(std::forward<P>(p)), F(std::forward<G>(f))
    {}

    template<class... Ts>
    constexpr auto operator()(Ts&&... xs) const FIT_RETURNS
    (
        this->base_function(xs...)(this->base_projection(xs...)(xs)...)
    );
};

template<class Projection, class F>
constexpr on_adaptor<Projection, F> on(Projection p, F f)
{
    return on_adaptor<Projection, F>(std::move(p), std::move(f));
}

}
#endif