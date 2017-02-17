// StaticMode
// Copyright (c) 2017 Ross Bencina
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#ifndef INCLUDED_STATICMODE_H
#define INCLUDED_STATICMODE_H

#include <type_traits>

namespace staticmode {

///////////////////////////////////////////////////////////////////////////////
// Utility templates used to implement type checking of mode expressions

namespace detail {

// ----------------------------------------------------------------------------

// std::remove_cv_t was added in C++14
// For C++11 compatibility we provide our own implementation called remove_cv_t_
template<typename T>
using remove_cv_t_ = typename std::remove_cv<T>::type;

// ----------------------------------------------------------------------------

// A version of std::is_same that ingnores const/volatile qualifiers

template<typename T, typename U>
using is_same_no_cv = std::is_same<remove_cv_t_<T>, remove_cv_t_<U> >;

// ----------------------------------------------------------------------------
// and_<...>, or_<...> combine a parameter pack of boolean type values
// (e.g. and_<std::true_type, std::true_type, std::false_type>).
// They are equivalent to C++17 std::conjuction and std::disjunction
// provided for compatibility with non-C++17 std libraries.

#if 0
// std::conjuction and std::disjunction are C++17 features.
// If we have std::conjuction and std::disjunction: MSVC 2015 does, clang 3.8 doesn't seem to.

template<class... B>
using and_ = std::conjunction<B...>;

template<class... B>
using or_ = std::disjunction<B...>;

#else

// Stand-alone implementations of and_<> and or_<>

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

} // end namespace detail

///////////////////////////////////////////////////////////////////////////////
// type_pack

// type_pack represents a collection of types. We only use it at the type-level,
// that's why it has no definition (value-level representation).

template<typename...>
struct type_pack;

namespace detail {

// has_type_no_cv: true if T appears in TypePack

template <typename T, typename TypePack>
struct has_type_no_cv;

template <typename T, typename... Us>
struct has_type_no_cv<T, type_pack<Us...> >
    : or_<detail::is_same_no_cv<T, Us>...> {};

//-----------------------------------------------------------------------------

// contains_duplicate_no_cv_: true if any type appears twice or more in Ts
// Ts... version

template<typename... Ts>
struct contains_duplicate_no_cv_;

template<>
struct contains_duplicate_no_cv_<> : std::false_type {};

template<typename T, typename... Ts>
struct contains_duplicate_no_cv_<T, Ts...>
    : or_<is_same_no_cv<T, Ts>..., contains_duplicate_no_cv_<Ts...> >
{};

// contains_duplicate_no_cv: true if any type appears twice or more in type_pack
// type_pack<Ts...> version

template<typename TypePack>
struct contains_duplicate_no_cv;

template<typename... Ts>
struct contains_duplicate_no_cv<type_pack<Ts...> >
    : contains_duplicate_no_cv_<Ts...>
{};

// ----------------------------------------------------------------------------

// is_subset_no_cv: true if all elements of A appear in B

template <typename A, typename B>
struct is_subset_no_cv;

template <typename... Us>
struct is_subset_no_cv<type_pack<>, type_pack<Us...> > : std::true_type {};

template <typename T, typename... Ts, typename... Us>
struct is_subset_no_cv<type_pack<T, Ts...>, type_pack<Us...> >
    : and_<has_type_no_cv<T, type_pack<Us...> >,
        is_subset_no_cv<type_pack<Ts...>, type_pack<Us...> > >
{};

//-----------------------------------------------------------------------------

} // end namespace detail

///////////////////////////////////////////////////////////////////////////////
// StaticMode library proper

template<typename T>
struct ModeType {
    using type = ModeType;

    using value_type = T; // value_type is typically the enum class type

    // value_types is used for ModeSet / mode expression type checking
    using value_types = type_pack<value_type>; // (see ModeSet::value_types)
}; // aka mode category

template<typename T, T X>
struct Mode : ModeType<T> {

    /// Mode provides the same interface as std::integral_constant
    // (plus some additional features needed for type checking mode expressions.)

    using type = Mode;
    using value_type = T; // value_type is typically the enum class type

    //  Allow using Mode instances as runtime values.
    static constexpr value_type value = X;

    // Mode instances automatically convert to the underlying enum value
    constexpr operator value_type() const noexcept { return value; }

    // Mode instances can serve as constexpr function objects that return the enum value
    constexpr value_type operator()() const noexcept { return value; }


    /// Features beyond std::integral_constant interface

    // value_types is used for ModeSet / mode expression type checking
    using value_types = type_pack<value_type>; // (see ModeSet::value_types)

    /// Prior to clang 3.9, it was required to specificy an initializer `={};`
    /// for constexpr empty structs. The user defined trivial default constructor
    /// works around that issue. It is not required for gcc or MSVC2015.
    ///
    /// The old clang behavior resulted from a more conservative interpretation
    /// of the language specification. CWG August 2011 meeting resolved:
    /// "If the implicit default constructor initializes all subobjects, no initializer should be required."
    /// Source: http://open-std.org/jtc1/sc22/wg21/docs/cwg_active.html#253
    /// See also: http://stackoverflow.com/a/28338265/2013747
    constexpr Mode() {}
};

// Type checking predicates:

namespace detail {

template<typename T>
struct is_mode_ : std::false_type {};

template<typename T, T X>
struct is_mode_<Mode<T, X> > : std::true_type {};

} // end namespace detail

template<typename T>
using is_mode = detail::is_mode_<detail::remove_cv_t_<T> >;


template<typename... Ts>
struct ModeSet {
    using type = ModeSet;

    // type checking support:

    // value_types is the list of enum class types in this mode set
    // (used for type checking, see static_asserts below).
    using value_types = type_pack<typename Ts::value_type...>;

    // Invariants:

    // A ModeSet is a set of zero or more Modes
    static_assert(detail::and_<is_mode<Ts>...>::value,
        "A ModeSet must contain only Mode<T,X> instances.");

    // A ModeSet_ contains at most one Mode_ from each mode category (enum class).
    static_assert(detail::contains_duplicate_no_cv<value_types>::value == false,
        "Duplicate mode category detected (two or more values from the same enum class). "
        "A ModeSet may contain at most one Mode from each mode category (enum class).");
};

// Type checking predicates:

namespace detail {

template<typename T>
struct is_mode_set_ : std::false_type {};

template<typename... Ts>
struct is_mode_set_<ModeSet<Ts...> > : std::true_type {};

} // end namespace detail

template<typename T>
using is_mode_set = detail::is_mode_set_<detail::remove_cv_t_<T> >;

template<typename T>
struct is_mode_expr : detail::or_<is_mode<T>, is_mode_set<T> > {};

// ModeSets can be constructed by combining Mode values using operator|:

// Mode<T,X> | Mode<U,Y> -> ModeSet< Mode<T,X>, Mode<U,Y> >
template<typename T, T X, typename U, U Y>
constexpr ModeSet<Mode<T,X>, Mode<U,Y> > operator|(Mode<T,X>, Mode<U,Y>) { return {}; }

// ModeSet<Ts...> | Mode<U,Y> -> ModeSet<Ts..., Mode<U,Y> >
template<typename... Ts, typename U, U Y>
constexpr ModeSet<Ts..., Mode<U,Y> > operator|(ModeSet<Ts...>, Mode<U,Y>) { return {}; }

// Mode<T,X> | ModeSet<Us...> -> ModeSet< Mode<T,X>, Us...>
template<typename T, T X, typename... Us>
constexpr ModeSet<Mode<T,X>, Us...> operator|(Mode<T,X>, ModeSet<Us...>) { return {}; }

// ModeSet<Ts...> | ModeSet<Us...> -> ModeSet<Ts..., Us...>
template<typename... Ts, typename... Us>
constexpr ModeSet<Ts..., Us...> operator|(ModeSet<Ts...>, ModeSet<Us...>) { return {}; }

// ............................................................................

// has_no_other_modes<EnumClasses, ModeExpr>
// test whether /ModeExpr/ only contains modes from categories in the /EnumClasses/ type_pack

template<typename AcceptedModeEnumClasses, typename ModeExpr>
using has_no_other_modes = detail::is_subset_no_cv<typename ModeExpr::value_types, AcceptedModeEnumClasses>;

// ............................................................................

// has_mode<EnumClasse, ModeExpr>
// test whether /ModeExpr/ contains a mode from category /EnumClass/ (an enum class value)

template<typename ModeEnumClass, typename ModeExpr>
using has_mode = detail::has_type_no_cv<ModeEnumClass, typename ModeExpr::value_types>;

// ............................................................................

// get_mode<KeyEnumClass, ModeExpr, Default>
//
// type parameters:
//   KeyEnumClass -- the type of the enum class used in Mode and ModeType
//   ModeExpr -- a ModeSet or a single Mode
//   Default -- the type returned if KeyEnumClass does not match any mode in ModeExpr
//
// output:
//   get_mode<K, M, D>::type will either be a type Mode<KeyEnumClass,X> (with whatever
//   X appears in the matching element of ModeExpr) or it will be of type Default.
//
// example usage:
// using lineStyle_t = get_mode_t<LineStyle, ModeExpr, /*default:*/decltype(solid)>;
//   or
// auto lineStyle = get_mode_t<LineStyle, ModeExpr, /*default:*/decltype(solid)> {};

/// General case, returns Default
/// This will match when none of the specializations below match.

template <typename KeyEnumClass, typename ModeExpr, typename Default>
struct get_mode {
    using type = detail::remove_cv_t_<Default>;
};

/// Specialization when ModeExpr is a single Mode with matching KeyEnumClass

template <typename KeyEnumClass, KeyEnumClass X, typename Default>
struct get_mode<KeyEnumClass, Mode<KeyEnumClass, X>, Default> {
    using type = Mode<KeyEnumClass, X>;
};

/// Specialization when ModeExpr is a ModeSet

// Recursing case: front element does not have KeyEnumClass
template <typename KeyEnumClass, typename EnumClass, EnumClass X, typename... Ts, typename Default>
struct get_mode<KeyEnumClass, ModeSet<Mode<EnumClass, X>, Ts...>, Default> {
    using type = typename get_mode<KeyEnumClass, ModeSet<Ts...>, Default>::type;
};

// Terminating matching case: front element has KeyEnumClass
template <typename KeyEnumClass, KeyEnumClass X, typename... Ts, typename Default>
struct get_mode<KeyEnumClass, ModeSet<Mode<KeyEnumClass, X>, Ts...>, Default> {
    using type = Mode<KeyEnumClass, X>;
};

// (Terminating empty set case is matched by the general template.)

//

/// get_mode_t alias type so we don't have to write "typename get_mode<x,y,z>::type"
template<typename KeyEnumClass, typename ModeExpr, typename Default>
using get_mode_t = typename get_mode<KeyEnumClass, detail::remove_cv_t_<ModeExpr>, Default>::type;

/*
    Implementation notes about const-ness of arguments to get_mode:

    We need to be careful with get_mode and const:

    - Mode instances declared as constexpr won't match a non-const
      Mode<> specialization in get_mode. This is the reason for
      remove_cv_t_<T> in get_mode_t. (Main reason for this is to assist
      with testing code such as get_mode_t<Key, decltype(my_flag1), decltype(my_flag1)>)

      This doesn't help if ModeExpr is a ModeSet containing const Mode<>
      (e.g. ModeSet<decltype(myflag)>). But that has only ever arisen in
      test code (in client code, ModeSet<> is constructed by operator|,
      which doesn't create const Modes). Hence, although in theory
      we could use a "deep_remove_cv" in get_mode_t to clean const Modes
      out of ModeExpr when ModeExpr is a ModeSet, we don't currently do it.

    - For consistency, we make get_mode::type non-cv, irrespective of the
      cv-ness of /Default/ or the contents of the mode set.
*/

///////////////////////////////////////////////////////////////////////////////

} // end namespace staticmode

#endif /* INCLUDED_STATICMODE_H */
