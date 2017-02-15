# My Motivation

I was trying to solve an API design problem. I had something like this:

```c++
class X {
    void send(Y *recipient, Z *message); // default, could be either of the below, or something else
    void send_enqueue(Y *recipient, Z *message);
    void send_forceDirectCall(Y *recipient, Z *message);
};
```
Key points:
* the *enqueue* and *direct call* involve independent implementations.
* it's kind-of ugly

After living with it for a whlie, it was time to consider a refactoring.
The usual suspects are: (1) use separate functions as above, or
(2) have a single function with a flag to select the dispatch method:

```c++
enum DispatchMethod { default_dispatch, enqueue, direct_call };

// caller syntax:
send(recipient, message); // default_dispatch
send(recipient, message, enqueue);
send(recipient, message, direct_call);
```

> Could I get the flags syntax, while retaining compile-time function selection?

I knew that the usage would always be like the first set of functions
i.e. the choice of *enqueue* or *direct call* is intrinsic to the algorithm.
But the second syntax made more sense, especially
when I discovered that I needed a second class of orthogonal modes:

```c++
enum DispatchMethod { default_dispatch, enqueue, direct_call };
enum MessagePhase { initiate, before_complete, complete, after_complete, passthrough };

// caller syntax
send(recipient, message); // defaults to enqueue|passthrough
send(recipient, message, enqueue|initiate);
send(recipient, message, enqueue|passthrough);
send(recipient, message, enqueue); // defaults to passthrough
```
---

Note that there is an analogy here with `std::atomic` memory ordering constraints,
where the barriers used are typically intrinsic to the correctness
of the algorithm (i.e. fixed at compile time), but the barrier selection is
made by passing an argument. e.g.

```c++
a.load(std::memory_order_acquire).
```