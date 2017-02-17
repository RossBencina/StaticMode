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

#include "catch.hpp"

#include <type_traits>

#include "StaticMode.h"

using namespace staticmode;

/// Utility templates

TEST_CASE("StaticMode/detail/remove_cv_t_", "remove_cv_t_") {

    static_assert(std::is_same<detail::remove_cv_t_<const int>, int >::value == true, "");
    static_assert(std::is_same<int, detail::remove_cv_t_<const int> >::value == true, "");

    REQUIRE(true);
}

TEST_CASE("StaticMode/detail/is_same_no_cv", "is_same_no_cv") {

    static_assert(detail::is_same_no_cv<const int, int>::value == true, "");
    static_assert(detail::is_same_no_cv<int, const int>::value == true, "");

    REQUIRE(true);
}

TEST_CASE("StaticMode/detail/and_", "and_") {

    static_assert(detail::and_<>::value == true, "");

    static_assert(detail::and_<std::true_type>::value == true, "");
    static_assert(detail::and_<std::false_type>::value == false, "");

    static_assert(detail::and_<std::true_type, std::true_type>::value == true, "");
    static_assert(detail::and_<std::true_type, std::false_type>::value == false, "");
    static_assert(detail::and_<std::false_type, std::true_type>::value == false, "");
    static_assert(detail::and_<std::false_type, std::false_type>::value == false, "");

    static_assert(detail::and_<std::true_type, std::true_type, std::true_type>::value == true, "");
    static_assert(detail::and_<std::true_type, std::true_type, std::false_type>::value == false, "");
    static_assert(detail::and_<std::true_type, std::false_type, std::true_type>::value == false, "");
    static_assert(detail::and_<std::true_type, std::false_type, std::false_type>::value == false, "");
    static_assert(detail::and_<std::false_type, std::true_type, std::true_type>::value == false, "");
    static_assert(detail::and_<std::false_type, std::true_type, std::false_type>::value == false, "");
    static_assert(detail::and_<std::false_type, std::false_type, std::true_type>::value == false, "");
    static_assert(detail::and_<std::false_type, std::false_type, std::false_type>::value == false, "");

    REQUIRE(true);
}

TEST_CASE("StaticMode/detail/or_", "or_") {

    static_assert(detail::or_<>::value == false, "");

    static_assert(detail::or_<std::true_type>::value == true, "");
    static_assert(detail::or_<std::false_type>::value == false, "");

    static_assert(detail::or_<std::true_type, std::true_type>::value == true, "");
    static_assert(detail::or_<std::true_type, std::false_type>::value == true, "");
    static_assert(detail::or_<std::false_type, std::true_type>::value == true, "");
    static_assert(detail::or_<std::false_type, std::false_type>::value == false, "");

    static_assert(detail::or_<std::true_type, std::true_type, std::true_type>::value == true, "");
    static_assert(detail::or_<std::true_type, std::true_type, std::false_type>::value == true, "");
    static_assert(detail::or_<std::true_type, std::false_type, std::true_type>::value == true, "");
    static_assert(detail::or_<std::true_type, std::false_type, std::false_type>::value == true, "");
    static_assert(detail::or_<std::false_type, std::true_type, std::true_type>::value == true, "");
    static_assert(detail::or_<std::false_type, std::true_type, std::false_type>::value == true, "");
    static_assert(detail::or_<std::false_type, std::false_type, std::true_type>::value == true, "");
    static_assert(detail::or_<std::false_type, std::false_type, std::false_type>::value == false, "");

    REQUIRE(true);
}

/// type_pack templates

TEST_CASE("StaticMode/detail/has_type_no_cv", "has_type_no_cv") {
    // membership tests:
    static_assert(detail::has_type_no_cv<int, type_pack<> >::value == false, "");
    static_assert(detail::has_type_no_cv<int, type_pack<int> >::value == true, "");
    static_assert(detail::has_type_no_cv<int, type_pack<float> >::value == false, "");

    static_assert(detail::has_type_no_cv<int, type_pack<float, int> >::value == true, "");
    static_assert(detail::has_type_no_cv<int, type_pack<int, float> >::value == true, "");

    static_assert(detail::has_type_no_cv<int, type_pack<char, float, int> >::value == true, "");
    static_assert(detail::has_type_no_cv<int, type_pack<char, float, bool> >::value == false, "");

    // const doesn't affect membership test:
    static_assert(detail::has_type_no_cv<const int, type_pack<int> >::value == true, "");
    static_assert(detail::has_type_no_cv<int, type_pack<const int> >::value == true, "");

    REQUIRE(true);
}

TEST_CASE("StaticMode/detail/contains_duplicate_no_cv", "contains_duplicate_no_cv") {

    static_assert(detail::contains_duplicate_no_cv<type_pack<> >::value == false, "");
    static_assert(detail::contains_duplicate_no_cv<type_pack<int> >::value == false, "");
    static_assert(detail::contains_duplicate_no_cv<type_pack<int, float> >::value == false, "");
    static_assert(detail::contains_duplicate_no_cv<type_pack<int, int> >::value == true, "");
    static_assert(detail::contains_duplicate_no_cv<type_pack<int, int, float> >::value == true, "");
    static_assert(detail::contains_duplicate_no_cv<type_pack<int, float, int> >::value == true, "");
    static_assert(detail::contains_duplicate_no_cv<type_pack<float, int, int> >::value == true, "");

    static_assert(detail::contains_duplicate_no_cv<type_pack<int, const int> >::value == true, "");
    static_assert(detail::contains_duplicate_no_cv<type_pack<int, const int, float> >::value == true, "");
    static_assert(detail::contains_duplicate_no_cv<type_pack<int, float, const int> >::value == true, "");
    static_assert(detail::contains_duplicate_no_cv<type_pack<float, int, const int> >::value == true, "");

    static_assert(detail::contains_duplicate_no_cv<type_pack<const int, int> >::value == true, "");
    static_assert(detail::contains_duplicate_no_cv<type_pack<const int, int, float> >::value == true, "");
    static_assert(detail::contains_duplicate_no_cv<type_pack<const int, float, int> >::value == true, "");
    static_assert(detail::contains_duplicate_no_cv<type_pack<float, const int, int> >::value == true, "");

    REQUIRE(true);
}

TEST_CASE("StaticMode/detail/is_subset_no_cv", "is_subset_no_cv") {

    static_assert(detail::is_subset_no_cv<type_pack<>, type_pack<> >::value == true, "");
    static_assert(detail::is_subset_no_cv<type_pack<>, type_pack<int> >::value == true, "");
    static_assert(detail::is_subset_no_cv<type_pack<>, type_pack<int, int, int> >::value == true, "");

    static_assert(detail::is_subset_no_cv<type_pack<int>, type_pack<> >::value == false, "");
    static_assert(detail::is_subset_no_cv<type_pack<int>, type_pack<int> >::value == true, "");
    static_assert(detail::is_subset_no_cv<type_pack<int>, type_pack<int, int, int> >::value == true, "");

    static_assert(detail::is_subset_no_cv<type_pack<int, int, int>, type_pack<int, int, int> >::value == true, "");
    static_assert(detail::is_subset_no_cv<type_pack<int, float, double>, type_pack<int, float, double> >::value == true, "");
    static_assert(detail::is_subset_no_cv<type_pack<int, float, double>, type_pack<double, float, int> >::value == true, ""); // reverse order

    static_assert(detail::is_subset_no_cv<type_pack<>, type_pack<int, float, double> >::value == true, "");
    static_assert(detail::is_subset_no_cv<type_pack<int>, type_pack<int, float, double> >::value == true, "");
    static_assert(detail::is_subset_no_cv<type_pack<int, float>, type_pack<int, float, double> >::value == true, "");

    static_assert(detail::is_subset_no_cv<type_pack<int, float, double>, type_pack<> >::value == false, "");
    static_assert(detail::is_subset_no_cv<type_pack<int, float, double>, type_pack<int> >::value == false, "");
    static_assert(detail::is_subset_no_cv<type_pack<int, float, double>, type_pack<int, float> >::value == false, "");

    REQUIRE(true);
}

/// StaticMode library proper

// ModeType<>

TEST_CASE("StaticMode/mode-type", "ModeType<> fields") {

    static_assert(std::is_same<typename ModeType<int>::type, ModeType<int> >::value, "");
    static_assert(std::is_same<typename ModeType<int>::value_type, int>::value, "");
    static_assert(std::is_same<typename ModeType<int>::value_types, type_pack<int> >::value, "");

    REQUIRE(true);
}

// Mode<>

TEST_CASE("StaticMode/mode/fields", "Mode<> fields") {

    static_assert(std::is_same<typename Mode<int, 0>::type, Mode<int, 0> >::value, "");
    static_assert(std::is_same<typename Mode<int, 0>::value_type, int>::value, "");
    static_assert(Mode<int, 0>::value == 0, "");

    Mode<int, 0> m = {};
    static_assert(static_cast<int>(m) == 0, ""); // constexpr cast to value_type
    static_assert(m() == 0, ""); // constexpr operator()

    static_assert(std::is_same<typename Mode<int, 0>::value_types, type_pack<int> >::value, "");

    REQUIRE(true);
}

TEST_CASE("StaticMode/mode/is-mode", "is_mode<> true for any Mode<>, false otherwise") {

    static_assert(is_mode<Mode<int,0> >::value == true, "");

    struct S {};
    static_assert(is_mode<S>::value == false, "");

    static_assert(is_mode<ModeType<int> >::value == false, "");
    static_assert(is_mode<ModeSet<Mode<int,0> > >::value == false, "");
    static_assert(is_mode<ModeSet<Mode<int,0>, Mode<bool,true> > >::value == false, "");
    static_assert(is_mode<type_pack<> >::value == false, "");
    static_assert(is_mode<type_pack<int> >::value == false, "");
    static_assert(is_mode<type_pack<int, bool> >::value == false, "");

    REQUIRE(true);
}

// ModeSet<>

TEST_CASE("StaticMode/mode-set/type", "ModeSet<> type field") {

    // type
    static_assert(std::is_same<typename ModeSet<>::type, ModeSet<> >::value, "");
    static_assert(std::is_same<typename ModeSet<Mode<int, 0> >::type, ModeSet<Mode<int, 0> > >::value, "");
    static_assert(std::is_same<typename ModeSet<Mode<int, 0>, Mode<bool, true> >::type, ModeSet<Mode<int, 0>, Mode<bool, true> > >::value, "");

    REQUIRE(true);
}

TEST_CASE("StaticMode/mode-set/value-types",
    "ModeSet<>::value_types correctly extracts value types") {

    static_assert(detail::is_same_no_cv<ModeSet<>::value_types, type_pack<> >::value, "");
    static_assert(detail::is_same_no_cv<ModeSet<Mode<char, 1> >::value_types, type_pack<char> >::value, "");
    static_assert(detail::is_same_no_cv<ModeSet<Mode<char, 1>, Mode<int, 2> >::value_types, type_pack<char, int> >::value, "");
    static_assert(detail::is_same_no_cv<ModeSet<Mode<char, 1>, Mode<int, 2>, Mode<bool, false> >::value_types, type_pack<char, int, bool> >::value, "");

    REQUIRE(true);
}

TEST_CASE("StaticMode/mode-set/must-contain-only-modes",
    "ModeSet<> will fail to compile if you try to put anything but modes in it") {

    // legal cases:
    using modeSet0_t = ModeSet<>;
    using modeSet1_t = ModeSet<Mode<char, 1> >;
    using modeSet2_t = ModeSet<Mode<char, 1>, Mode<int, 2> >;
    using modeSet3_t = ModeSet<Mode<char, 1>, Mode<int, 2>, Mode<bool, false> >;

    // illegal cases:
    // uncommenting any of the following lines will fail to compile
    //auto modeSet1x = ModeSet<int> {};
    //auto modeSet2x = ModeSet<int, Mode<int, 2> > {};
    //auto modeSet2y = ModeSet<Mode<char, 1>, int > {};
    //auto modeSet3x = ModeSet<int, Mode<int, 2>, Mode<bool, false> > {};
    //auto modeSet3y = ModeSet<Mode<char, 1>,int, Mode<bool, false> > {};
    //auto modeSet3z = ModeSet<Mode<char, 1>, Mode<int, 2>, int > {};

    REQUIRE(true);
}

TEST_CASE("StaticMode/mode-set/is-mode-set",
    "is_mode_set<> true for any ModeSet<>, false otherwise") {

    static_assert(is_mode_set<ModeSet<> >::value == true, "");
    static_assert(is_mode_set<ModeSet<Mode<char, 1> > >::value == true, "");
    static_assert(is_mode_set<ModeSet<Mode<char, 1>, Mode<int, 2> > >::value == true, "");
    static_assert(is_mode_set<ModeSet<Mode<char, 1>, Mode<int, 2>, Mode<bool, false> > >::value == true, "");

    struct S {};
    static_assert(is_mode_set<S>::value == false, "");

    static_assert(is_mode_set<Mode<int,0> >::value == false, "");
    static_assert(is_mode_set<ModeType<int> >::value == false, "");
    static_assert(is_mode_set<type_pack<> >::value == false, "");
    static_assert(is_mode_set<type_pack<int> >::value == false, "");
    static_assert(is_mode_set<type_pack<int, bool> >::value == false, "");

    REQUIRE(true);
}

// mode expressions

TEST_CASE("StaticMode/mode-expression/is-mode-expr/basic",
    "is_mode_expr<> true for any Mode<> or ModeSet<>, false otherwise") {

    // This tests is_mode_expr<> against types returned by operator-|.
    // below we'll test it on expressions with operator-|

    static_assert(is_mode_expr<Mode<int,0> >::value == true, "");

    static_assert(is_mode_expr<ModeSet<> >::value == true, "");
    static_assert(is_mode_expr<ModeSet<Mode<char, 1> > >::value == true, "");
    static_assert(is_mode_expr<ModeSet<Mode<char, 1>, Mode<int, 2> > >::value == true, "");
    static_assert(is_mode_expr<ModeSet<Mode<char, 1>, Mode<int, 2>, Mode<bool, false> > >::value == true, "");

    struct S {};
    static_assert(is_mode_expr<S>::value == false, "");

    static_assert(is_mode_expr<ModeType<int> >::value == false, "");
    static_assert(is_mode_expr<type_pack<> >::value == false, "");
    static_assert(is_mode_expr<type_pack<int> >::value == false, "");
    static_assert(is_mode_expr<type_pack<int, bool> >::value == false, "");

    REQUIRE(true);
}

namespace {

    enum class TestEnumOne { test1, test2, test3, testX };
    constexpr Mode<TestEnumOne, TestEnumOne::test1> one_test1;
    constexpr Mode<TestEnumOne, TestEnumOne::test2> one_test2;
    constexpr Mode<TestEnumOne, TestEnumOne::test3> one_test3;
    constexpr Mode<TestEnumOne, TestEnumOne::testX> one_testX;

    enum class TestEnumTwo { test1, test2, test3, testX };
    constexpr Mode<TestEnumTwo, TestEnumTwo::test1> two_test1;
    constexpr Mode<TestEnumTwo, TestEnumTwo::test2> two_test2;
    constexpr Mode<TestEnumTwo, TestEnumTwo::test3> two_test3;
    constexpr Mode<TestEnumTwo, TestEnumTwo::testX> two_testX;

    enum class TestEnumThree { test1, test2, test3, testX };
    constexpr Mode<TestEnumThree, TestEnumThree::test1> three_test1;
    constexpr Mode<TestEnumThree, TestEnumThree::test2> three_test2;
    constexpr Mode<TestEnumThree, TestEnumThree::test3> three_test3;
    constexpr Mode<TestEnumThree, TestEnumThree::testX> three_testX;

    enum class TestEnumFour { test1, test2, test3, testX };
    constexpr Mode<TestEnumFour, TestEnumFour::test1> four_test1;
    constexpr Mode<TestEnumFour, TestEnumFour::test2> four_test2;
    constexpr Mode<TestEnumFour, TestEnumFour::test3> four_test3;
    constexpr Mode<TestEnumFour, TestEnumFour::testX> four_testX;

} // end anonymous namespace

TEST_CASE("StaticMode/mode-expression/operator-or/happy-path",
    "mode expression operator-| happy-path usage") {

    constexpr auto a = one_test1 | two_test1; // value | value

    constexpr auto ax = a | three_test1; // set | value
    constexpr auto xa = three_test1 | a; // value | set

    constexpr auto b = three_test1 | four_test1; // value | value

    constexpr auto y = a | b; // set | set

    constexpr auto c1 = one_test1 | two_test1 | three_test1;
    constexpr auto c2 = (one_test1 | two_test1) | three_test1;
    constexpr auto c3 = one_test1 | (two_test1 | three_test1);

    constexpr auto d1 = one_test1 | two_test1 | three_test1 | four_test1;
    constexpr auto d2 = (one_test1 | two_test1) | three_test1 | four_test1;
    constexpr auto d3 = one_test1 | (two_test1 | three_test1) | four_test1;
    constexpr auto d4 = one_test1 | two_test1 | (three_test1 | four_test1);

    // is_mode_expr<> true for any valid operator-| mode expression

    static_assert(is_mode_expr<decltype(one_test1)>::value == true, "");

    static_assert(is_mode_expr<decltype(a)>::value == true, "");

    static_assert(is_mode_expr<decltype(ax)>::value == true, "");
    static_assert(is_mode_expr<decltype(xa)>::value == true, "");

    static_assert(is_mode_expr<decltype(b)>::value == true, "");

    static_assert(is_mode_expr<decltype(y)>::value == true, "");

    static_assert(is_mode_expr<decltype(c1)>::value == true, "");
    static_assert(is_mode_expr<decltype(c2)>::value == true, "");
    static_assert(is_mode_expr<decltype(c3)>::value == true, "");

    static_assert(is_mode_expr<decltype(d1)>::value == true, "");
    static_assert(is_mode_expr<decltype(d2)>::value == true, "");
    static_assert(is_mode_expr<decltype(d3)>::value == true, "");
    static_assert(is_mode_expr<decltype(d4)>::value == true, "");

    REQUIRE(true);
}

TEST_CASE("StaticMode/mode-expression/operator-or/bad-usage",
    "mode expression operator-| correctly fails to compile in these bad usaeg cases") {

    enum class TestEnum { test1, test2 };
    constexpr Mode<TestEnum, TestEnum::test1> test1 = {};
    constexpr Mode<TestEnum, TestEnum::test2> test2 = {};

    // illegal cases:
    // uncommenting any of the following lines will fail to compile
    //constexpr auto shouldFail1 = test1 | test1; // correctly fails: duplicate mode
    //constexpr auto shouldFail2 = test1 | test2; // correctly fails: multiple modes from same mode category
    //constexpr auto shouldFail2 = test1 | test2 | test1; // correctly fails: duplicate mode and multiple modes from same mode category

    //using mightFail3 = decltype(test1 | test1); // doesn't fail in MSVC. does in clang. unclear whether it should fail until the type is actually used.
    //constexpr mightFail3 shouldFail3a = {}; // correctly fails: duplicate mode

    //using mightFail4 = decltype(test1 | test2); // doesn't fail in MSVC. does in clang. unclear whether it should fail until the type is actually used.
    //constexpr mightFail4 shouldFail4a = {}; // correctly fails: multiple modes from same mode category

    REQUIRE(true);
}

// has_no_other_modes

TEST_CASE("StaticMode/get_mode_t", "has_no_other_modes<>") {

    // empty allowed modes
    {
        using AllowedModes = type_pack<>;

        {
            using ModeExpr = ModeSet<>;
            static_assert(has_no_other_modes<AllowedModes, ModeExpr>::value == true, "");
        }

        {
            using ModeExpr = Mode<TestEnumOne, TestEnumOne::test1>;
            static_assert(has_no_other_modes<AllowedModes, ModeExpr>::value == false, "");
        }

        {
            using ModeExpr = decltype(one_test1 | two_test1);
            static_assert(has_no_other_modes<AllowedModes, ModeExpr>::value == false, "");
        }
    }

    // one allowed mode
    {
        using AllowedModes = type_pack<TestEnumOne>;

        {
            using ModeExpr = ModeSet<>;
            static_assert(has_no_other_modes<AllowedModes, ModeExpr>::value == true, "");
        }

        {
            using ModeExpr = Mode<TestEnumOne, TestEnumOne::test1>;
            static_assert(has_no_other_modes<AllowedModes, ModeExpr>::value == true, "");
        }

        {
            using ModeExpr = decltype(one_test1 | two_test1);
            static_assert(has_no_other_modes<AllowedModes, ModeExpr>::value == false, "");
        }
    }

    // two allowed modes
    {
        using AllowedModes = type_pack<TestEnumOne, TestEnumTwo>;

        {
            using ModeExpr = ModeSet<>;
            static_assert(has_no_other_modes<AllowedModes, ModeExpr>::value == true, "");
        }

        {
            using ModeExpr = Mode<TestEnumOne, TestEnumOne::test1>;
            static_assert(has_no_other_modes<AllowedModes, ModeExpr>::value == true, "");
        }

        {
            using ModeExpr = decltype(one_test1 | two_test1);
            static_assert(has_no_other_modes<AllowedModes, ModeExpr>::value == true, "");
        }
    }

    // three allowed modes
    {
        using AllowedModes = type_pack<TestEnumOne, TestEnumTwo, TestEnumThree>;

        {
            using ModeExpr = ModeSet<>;
            static_assert(has_no_other_modes<AllowedModes, ModeExpr>::value == true, "");
        }

        {
            using ModeExpr = Mode<TestEnumOne, TestEnumOne::test1>;
            static_assert(has_no_other_modes<AllowedModes, ModeExpr>::value == true, "");
        }

        {
            using ModeExpr = decltype(one_test1 | two_test1);
            static_assert(has_no_other_modes<AllowedModes, ModeExpr>::value == true, "");
        }
    }

    REQUIRE(true);
}

// has_mode

TEST_CASE("StaticMode/has_mode", "has_mode<>") {

    {
        using ModeExpr = ModeSet<>;
        static_assert(has_mode<TestEnumOne, ModeExpr>::value == false, "");
    }

    {
        using ModeExpr = Mode<TestEnumOne, TestEnumOne::test1>;
        static_assert(has_mode<TestEnumOne, ModeExpr>::value == true, "");
        static_assert(has_mode<TestEnumTwo, ModeExpr>::value == false, "");
    }

    {
        using ModeExpr = decltype(one_test1 | two_test1);
        static_assert(has_mode<TestEnumOne, ModeExpr>::value == true, "");
        static_assert(has_mode<TestEnumTwo, ModeExpr>::value == true, "");
        static_assert(has_mode<TestEnumThree, ModeExpr>::value == false, "");
    }

    {
        using ModeExpr = decltype(one_test1 | two_test1 | three_test1);
        static_assert(has_mode<TestEnumOne, ModeExpr>::value == true, "");
        static_assert(has_mode<TestEnumTwo, ModeExpr>::value == true, "");
        static_assert(has_mode<TestEnumThree, ModeExpr>::value == true, "");
        static_assert(has_mode<TestEnumFour, ModeExpr>::value == false, "");
    }
}

// get_mode_t

TEST_CASE("StaticMode/get_mode_t/happy-path", "get_mode_t<> test happy path usage") {

    enum class EmptyModeEnum { empty };
    using empty_mode_type = Mode<EmptyModeEnum, EmptyModeEnum::empty>;

    // get_mode_t signature:
    //template <typename KeyEnumClass, typename ModeExpr, typename Default>
    //struct get_mode_t;

    // a single Mode instance
    {
        auto singleMode = one_test1;
        // key is present, with or without specified default
        static_assert( (detail::is_same_no_cv<get_mode_t<TestEnumOne, decltype(singleMode), decltype(one_testX)>, decltype(one_test1)>::value == true), "");
        static_assert( (detail::is_same_no_cv<get_mode_t<TestEnumOne, decltype(singleMode), empty_mode_type>, decltype(one_test1)>::value == true), "" );

        // key is not present, with or without specified default
        static_assert( (detail::is_same_no_cv<get_mode_t<TestEnumTwo, decltype(singleMode), decltype(one_testX)>, decltype(one_testX)>::value == true), "");
        static_assert( (detail::is_same_no_cv<get_mode_t<TestEnumTwo, decltype(singleMode), empty_mode_type>, empty_mode_type>::value == true), "");
    }

    // an empty ModeSet
    {
        auto emptyModeSet = ModeSet<> {};
        // key is not present, with or without specified default
        static_assert( (detail::is_same_no_cv<get_mode_t<TestEnumTwo, decltype(emptyModeSet), decltype(one_testX)>, decltype(one_testX)>::value == true), "");
        static_assert( (detail::is_same_no_cv<get_mode_t<TestEnumTwo, decltype(emptyModeSet), empty_mode_type>, empty_mode_type>::value == true), "");
    }

    // a ModeSet containing one Mode instance (in practice this is not likely to be used because of the way modes are constructed)
    {
        // NB: get_mode_t can't currently handle ModeSet containing a const Mode

        //auto singletonModeSet = ModeSet<decltype(one_test1)> {}; // this will fail to compile, ModeSet<const Mode<> >

        //using one_test1_type_ = decltype(one_test1);
        //auto singletonModeSet = ModeSet<one_test1_type_> {}; // this will fail for the same reason

        auto singletonModeSet = ModeSet<detail::remove_cv_t_<decltype(one_test1)> > {}; // this is ok, ModeSet<Mode<>>

        //auto singletonModeSet = ModeSet<Mode<TestEnumOne, TestEnumOne::test1> > {}; // this is ok

        // key is present, with or without specified default
        static_assert( (detail::is_same_no_cv<get_mode_t<TestEnumOne, decltype(singletonModeSet), decltype(one_testX)>, decltype(one_test1)>::value == true), "");
        static_assert( (detail::is_same_no_cv<get_mode_t<TestEnumOne, decltype(singletonModeSet), empty_mode_type>, decltype(one_test1)>::value == true), "");

        // key is not present, with or without specified default
        static_assert( (detail::is_same_no_cv<get_mode_t<TestEnumTwo, decltype(singletonModeSet), decltype(one_testX)>, decltype(one_testX)>::value == true), "");
        static_assert( (detail::is_same_no_cv<get_mode_t<TestEnumTwo, decltype(singletonModeSet), empty_mode_type>, empty_mode_type>::value == true), "");
    }

    // a ModeSet containing two Mode instances
    {
        //NB: operator| returns a non-const ModeSet containing non-const Mode instances

        auto modeSet = one_test1 | three_test1;

        // key is present, with or without specified default
        static_assert( (detail::is_same_no_cv<get_mode_t<TestEnumOne, decltype(modeSet), decltype(one_testX)>, decltype(one_test1)>::value == true), "");
        static_assert( (detail::is_same_no_cv<get_mode_t<TestEnumOne, decltype(modeSet), empty_mode_type>, decltype(one_test1)>::value == true), "");

        // key is not present, with or without specified default
        static_assert( (detail::is_same_no_cv<get_mode_t<TestEnumTwo, decltype(modeSet), decltype(one_testX)>, decltype(one_testX)>::value == true), "");
        static_assert( (detail::is_same_no_cv<get_mode_t<TestEnumTwo, decltype(modeSet), empty_mode_type>, empty_mode_type>::value == true), "");
    }

    // a ModeSet containing three Mode instances
    {
        auto modeSet = one_test1 | three_test1 | four_test1;

        // key is present, with or without specified default
        static_assert( (detail::is_same_no_cv<get_mode_t<TestEnumOne, decltype(modeSet), decltype(one_testX)>, decltype(one_test1)>::value == true), "");
        static_assert( (detail::is_same_no_cv<get_mode_t<TestEnumOne, decltype(modeSet), empty_mode_type>, decltype(one_test1)>::value == true), "");

        // key is not present, with or without specified default
        static_assert( (detail::is_same_no_cv<get_mode_t<TestEnumTwo, decltype(modeSet), decltype(one_testX)>, decltype(one_testX)>::value == true), "");
        static_assert( (detail::is_same_no_cv<get_mode_t<TestEnumTwo, decltype(modeSet), empty_mode_type>, empty_mode_type>::value == true), "");
    }

    REQUIRE(true);
}

/// Integration tests

namespace {
    struct SingleTagMemberFn {

        int test(ModeType<TestEnumOne>) { // fallback/general case, matches tags with no specific tag type
            return 0;
        }

        int test(decltype(one_test1)={}) { // used as default when no flag is passed
            return 1;
        }

        int test(decltype(one_test2)) {
            return 2;
        }

        int test(decltype(one_test3)) {
            return 3;
        }
    };

} // end anonymous namespace

TEST_CASE("StaticMode/integration/one-tag-dispatch",
    "test overloaded member function dispatch using a single mode category") {

    SingleTagMemberFn s;

    REQUIRE( s.test() == 1 ); // default/no-flag supplied case
    REQUIRE( s.test(one_test1) == 1 );
    REQUIRE( s.test(one_test2) == 2 );
    REQUIRE( s.test(one_test3) == 3 );
    REQUIRE( s.test(one_testX) == 0 ); // fallback case

    //s.test(two_testX); // should fail to compile, this mode category not implemented
}

namespace {

    // workaround for MSVC2015 bug SO 41001482: can't use decltype(T) directly in member function template arg list
    // so instead we set up type aliases and use those in the relevant member function templates below
    // http://stackoverflow.com/questions/41001482/msvc2015-decltype-parameter-type-in-overloaded-template-function
    using two_test1_type_ = decltype(two_test1);
    using two_test2_type_ = decltype(two_test2);
    using two_test3_type_ = decltype(two_test3);
    using two_testX_type_ = decltype(two_testX);

    struct TwoNestedTagMemberFn {

        int testA(ModeType<TestEnumOne>) { // fallback/general case, matches tags with no specific tag type
            return 0;
        }

        int testA(decltype(one_test1)={}) { // used as default when no flag is passed
            return 1;
        }

        int testA(decltype(one_test2)) {
            return 2;
        }

        template<typename ModeOne_>
        std::pair<int, int> testB_(ModeOne_ modeOne, ModeType<TestEnumTwo>) { // fallback/general case, matches tags with no specific tag type
            return std::pair<int, int>(testA(modeOne), 0);
        }

        template<typename ModeOne_>
        std::pair<int, int> testB_(ModeOne_ modeOne, two_test1_type_) { // used as default when no flag is passed
            return std::pair<int, int>(testA(modeOne), 1);
        }

        template<typename ModeOne_>
        std::pair<int, int> testB_(ModeOne_ modeOne, two_test2_type_) {
            return std::pair<int, int>(testA(modeOne), 2);
        }

        template<typename ModeExpr=ModeSet<> >
        std::pair<int, int> testB(ModeExpr /*modes*/ ={})
        {
            static_assert(staticmode::is_mode_expr<ModeExpr>::value == true,
                "/modes/ argument has unexpected type. Expected a Mode or a ModeSet.");

            using AcceptedModes = staticmode::type_pack<TestEnumOne, TestEnumTwo>;
            // Verify that modes don't belong to non-accepted mode categories.
            static_assert(staticmode::has_no_other_modes<AcceptedModes, ModeExpr>::value,
                "/modes/ argument contains a mode from an unexpected category. "
                "Expected at most one TestEnumOne mode and one TestEnumTwo mode.");

            using modeOne_t = get_mode_t<TestEnumOne, ModeExpr, decltype(one_test1)>;
            using modeTwo_t = get_mode_t<TestEnumTwo, ModeExpr, decltype(two_test1)>;
            return testB_(modeOne_t{}, modeTwo_t{});
        }
    };

    static std::pair<int, int> P(int a, int b) { return std::pair<int, int>(a, b); }

} // end anonymous namespace

TEST_CASE("StaticMode/integration/get_mode_t/two-tag-dispatch", "test member function dispatch using get_mode_t with two mode categories") {

    TwoNestedTagMemberFn s;

    REQUIRE( s.testB() == P(1, 1) ); // default/no-flag supplied case
    REQUIRE( s.testB(one_test2) == P(2, 1) ); // default mode two
    REQUIRE( s.testB(one_testX) == P(0, 1) ); // default mode two, mode one fallback case
    REQUIRE( s.testB(two_test2) == P(1, 2) ); // default mode 1
    REQUIRE( s.testB(two_testX) == P(1, 0) ); // default mode 1, mode two fallback case

    // full 3x3 matrix
    REQUIRE( s.testB(one_testX | two_testX) == P(0, 0) );
    REQUIRE( s.testB(one_test1 | two_testX) == P(1, 0) );
    REQUIRE( s.testB(one_test2 | two_testX) == P(2, 0) );

    REQUIRE( s.testB(one_testX | two_test1) == P(0, 1) );
    REQUIRE( s.testB(one_test1 | two_test1) == P(1, 1) );
    REQUIRE( s.testB(one_test2 | two_test1) == P(2, 1) );

    REQUIRE( s.testB(one_testX | two_test2) == P(0, 2) );
    REQUIRE( s.testB(one_test1 | two_test2) == P(1, 2) );
    REQUIRE( s.testB(one_test2 | two_test2) == P(2, 2) );

    //s.testB(three_testX); // should fail to compile, three_testX mode category not supported by s
    //s.testB(three_testX | one_test2); // should fail to compile, three_testX mode category not supported by s
    //s.testB(one_test2 | three_testX); // should fail to compile, three_testX mode category not supported by s
    //s.testB(four_testX | three_testX); // should fail to compile, three_testX nor four_testX mode categories not supported by s
}
