# Plan of Action

Splitting Corpus::getStringLocationFromType into three separate functions:

1. Convert a language type into its corresponding ABI register class
    a. pg20 in the AMD64 ABI doc

    b. See [X86_64ABIInfo::classify](https://github.com/llvm/llvm-project/blob/main/clang/lib/CodeGen/TargetInfo.cpp#L2790) for how LLVM does it. Note that their implementation is incomplete, but will get the "basic" types correct.

    c. The [merge](https://github.com/llvm/llvm-project/blob/main/clang/lib/CodeGen/TargetInfo.cpp#L2750) and [postMerge](https://github.com/llvm/llvm-project/blob/main/clang/lib/CodeGen/TargetInfo.cpp#L2717) steps should probably be factored out, but not accessible by the user of our class.

2. Allocate register/memory resources
    a. I haven't found code for this yet. I'll keep looking

3. Handle return values
    1. It's the same classification rules, but the allocation is different
    
# Questions from Vanessa

What does it mean to be split by significance?

> Arguments of types__float128and__m128are split into two halves. The least significant ones belong to class SSE, the most significant one toclass SSEUP.

I think this probably means we store half of it in SSE and the other half in SSEUP, so it's always in two
registers? This means I'll need to store a vector of RegisterClass for each evaluation.

I'm not sure what this is saying:

> Arguments of type__int128 offer the same operations as INTEGERs, yet they do not fit into one general purpose register but require two registers. For classification purposes__int128is treated as if it were implemented as...

```cpp
typedef struct {
   long low, high;
} __int128;
```

with  the  exception  that  arguments  of  type__int128 that  are  stored in memory must be aligned on a 16-byte boundary

I would first guess they would be treated as two longs, but I'm not sure because it mentions memory.
I'll hold off on doing this one.

Same with this one:

> Arguments ofcomplex TwhereTis one of the typesfloat or doubleare treated as if they are implemented as:
