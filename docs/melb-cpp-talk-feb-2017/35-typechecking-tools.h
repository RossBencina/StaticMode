#ifndef INCLUDED_TYPECHECKINGTOOLS_H
#define INCLUDED_TYPECHECKINGTOOLS_H

// This file provides utility templates that we use to implement type checking
// in 35-typechecking.cpp

#include <type_traits>

// ----------------------------------------------------------------------------

template<typename T>
using remove_cv_t_ = typename std::remove_cv<T>::type; // or just std::remove_cv_t from C++14 onwards

// ----------------------------------------------------------------------------
// and_, or_ combine a type-pack of boolean type values (e.g. and_<std::true_type, std::true_type, std::false_type>)
// equivalent to C++17 std::conjuction and std::disjunction
// provided for compatibility with non-C++17 std libraries

#if 0 // have std::conjuction and std::disjunction: MSVC 2015 does, clang 3.8 doesn't seem to

template<class... B>
using and_ = std::conjunction<B...>;

template<class... B>
using or_ = std::disjunction<B...>;

#else

// std::conjuction and std::disjunction are C++17 features. MSVC2015 has them.
// below are replacement implementations.

// http://stackoverflow.com/questions/29603364/type-trait-to-check-that-all-types-in-a-parameter-pack-are-copy-constructible

template<typename... Conds>
struct and_ : std::true_type {};

template<typename Cond, typename... Conds>
struct and_<Cond, Conds...> : std::conditional<Cond::value, and_<Conds...>, std::false_type>::type
{};

template<typename... Conds>
struct or_ : std::false_type {};

template<typename Cond, typename... Conds>
struct or_<Cond, Conds...> : std::conditional<Cond::value, std::true_type, or_<Conds...> >::type
{};

#endif

// ----------------------------------------------------------------------------

template<typename T, typename U>
using is_same_no_cv = std::is_same<remove_cv_t_<T>, remove_cv_t_<U> >;

// ----------------------------------------------------------------------------

// type_pack_ represents a collection of types. We only use it at the type-level,
// that's why it has no definition (value-level representation).

template<typename...>
struct type_pack_;

//-----------------------------------------------------------------------------

// Ts... version
// contains_duplicate_no_cv_: true if any type appears twice or more in Ts

template<typename... Ts>
struct contains_duplicate_no_cv_;

template<>
struct contains_duplicate_no_cv_<> : std::false_type {};

template<typename T, typename... Ts>
struct contains_duplicate_no_cv_<T, Ts...>
    : or_<is_same_no_cv<T, Ts>..., contains_duplicate_no_cv_<Ts...> >
{};

// type_pack_<Ts...> version
// contains_duplicate_no_cv: true if any type appears twice or more in type_pack_

template<typename TypePack>
struct contains_duplicate_no_cv;

template<typename... Ts>
struct contains_duplicate_no_cv<type_pack_<Ts...> >
    : contains_duplicate_no_cv_<Ts...>
{};

// ----------------------------------------------------------------------------

// has_type_no_cv: true if T appears in TypePack

template <typename T, typename TypePack>
struct has_type_no_cv;

template <typename T, typename... Us>
struct has_type_no_cv<T, type_pack_<Us...> >
    : or_<is_same_no_cv<T, Us>...> {};

// ----------------------------------------------------------------------------

// is_subset_no_cv: true if all elements of A appear in B

template <typename A, typename B>
struct is_subset_no_cv;

template <typename... Us>
struct is_subset_no_cv<type_pack_<>, type_pack_<Us...> > : std::true_type {};

template <typename T, typename... Ts, typename... Us>
struct is_subset_no_cv<type_pack_<T, Ts...>, type_pack_<Us...> >
    : and_<has_type_no_cv<T, type_pack_<Us...> >,
        is_subset_no_cv<type_pack_<Ts...>, type_pack_<Us...> > >
{};

//-----------------------------------------------------------------------------

#endif /* INCLUDED_TYPECHECKINGTOOLS_H */
