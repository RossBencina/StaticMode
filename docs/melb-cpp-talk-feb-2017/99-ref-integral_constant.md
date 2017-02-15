`std::integral_constant` allows you to represent values at the type level. For example:

```c++
using fortytwo = std::integral_constant<int, 42>;
```

cppreference.com gives the following "possible implementation" of `std::integral_constant`:

```c++
template<class T, T v>
struct integral_constant {
    static constexpr T value = v;
    typedef T value_type;
    typedef integral_constant type; // using injected-class-name
    constexpr operator value_type() const noexcept { return value; }
    constexpr value_type operator()() const noexcept { return value; } //since c++14
};
```

Source: [http://en.cppreference.com/w/cpp/types/integral_constant](http://en.cppreference.com/w/cpp/types/integral_constant)

