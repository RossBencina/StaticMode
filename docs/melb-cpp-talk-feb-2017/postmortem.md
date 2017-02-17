This file contains notes about things I might do differently
if I give this talk again.

### "bitmask flags"

There is a strong analogy between runtime "bitmask flags" and
the compile time techniques presented in this talk. During
the talk I made reference to "the bitmask flag aproach", but
I never gave a full code example. In a future talk it would 
be good to show code giving the runtime bitmask flags 
version in its entirety:

```
// something like...

enum LineStyle { dotted=0x01, dashed=0x02, solid=0, LineStyle_MASK=0x03 };
enum EndStyle { arrows=0x04, circles=0x08, no_ends=0, EndStyle_MASK=0x0c };

void drawLine(int flags)
{
	assert((flags & ~(LineStyle_MASK|EndStyle_MASK)) == 0); // only expected flags
	LineStyle lineStyle = flags & LineStyle_MASK;
	EndStyle endStyle = flags & EndStyle_MASK;

	switch(lineStyle) {
		case dotted:
			etc.
		case dashed:
			etc.
		case solid:
			etc.
		default:
			assert(false); // unexpected line style	
	}

	etc...
}
```

### Tag Dispatch

See [20d-one-mode.h](20d-one-mode.h)

The precise definition of "tag dispatch" is controversial 
(see first link below). When preparing the talk I took 
the tentative view that what I'm doing here is not tag dispatch. 
Partly because of my experience in trying to get help -- 
mentioning "tag dispatch" generally leads to people thinking 
I'm trying to do something else.

During the talk, a comment from the audience drew 
me back into uncertainty on the definition of tag dispatch.

In future I think it would be better to include this aside
when file `20d-one-mode.h` is shown:

> Function overloading based on empty type parameters is
> part of a family of techniques known as "tag dispatch".
> For this talk you don't need to know anything about tag dispatch,
> you just need to understand how code in this file works.

Here is further information on tag dispatch from the top
two results of "definition of tag dispatch" on google:

[Tutorial on Tag Dispatching](https://crazycpp.wordpress.com/2014/12/15/tutorial-on-tag-dispatching/) 
-- this makes the point that tag dispatch is about annotating
types with "tags" (empty structs) that indicate which concepts
the tagged type implements. Quoting the article:

> We tag a type with something that can be accessed during 
> compile time that tells us what concept a type is implementing. 

[Generic Programming Techniques (boost.org)](http://www.boost.org/community/generic_programming.html)
-- here, the example uses a type traits template to extract
a tag associated with a given iterator.

I interpret the above two examples to illustrate that tag dispatch
involves *concepts* and attaching "tags" to other types --
either by putting the tags inside the other types, or using
traits. That said, there is a certainly overlap with 
the methods I discuss in this talk.


### Non-type Template Parameters

See [22-refine-one-mode-template.cpp](22-refine-one-mode-template.cpp)

At this point I should have checked that everyone understood 
what non-type template parameters are and how they work. I could have 
gone through parsing `Mode<T, X>` to make sure that they understood
that X is an enumeration value of type T. A simpler initial example such
as `Mode<int, 42>` might have helped at that point.
