/*
    Extra: static checks, runtime values

    This was something I planned to implement if I had time (I didn't)

    Even if you don't want compile time dispatch, you could still use
    the techniques in this talk to get some compile-time type checking.

    For example, it could be used to check for combining incompatible modes.

    The idea would be to allow both Mode and ModeSet to convert
    to an underlying value type (e.g. size_t). Mode operator size_t() would just return
    the underlying enum value. ModeSet operator size_t() would return all of the
    enum values, or-ed together.
*/

