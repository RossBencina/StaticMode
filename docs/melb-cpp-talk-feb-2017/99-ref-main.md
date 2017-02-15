The following form of `main` is valid, as confirmed by the standard extracts below.

```
int main()
{

}
```

Relevant C++ standard text is here: [http://eel.is/c++draft/basic.start.main](http://eel.is/c++draft/basic.start.main)

**C++ Standard, 3.6.1.2 ([basic.start.main]):**

> [The main function] shall have a declard return type of type `int`, but otherwise its type
> is implementation-defined. An implementation shall allow both [of the following definitions of main]:
>
> ```
> int main() { / ... / }
> ```
>
> and
>
> ```
> int main(int argc, char* argv[]) { / ... / }
> ```

**C++ Standard, 3.6.1.5 ([basic.start.main]):**

> A return statement in `main` has the effect of leaving the main
> function (destroying any objects with automatic storage duration)
> and calling `std::exit` with the return value as the argument.
> If control reaches the end of the compound-statement of `main`,
> the effect is equivalent to:
>
> ```
> return 0;
> ```

See also: See [http://stackoverflow.com/questions/4207134/what-is-the-proper-declaration-of-main](http://stackoverflow.com/questions/4207134/what-is-the-proper-declaration-of-main)
