Splitting Corpus::getStringLocationFromType into three separate functions:

1. Convert a language type into its corresponding ABI register class
    a. pg20 in the AMD64 ABI doc

    b. See [X86_64ABIInfo::classify](https://github.com/llvm/llvm-project/blob/main/clang/lib/CodeGen/TargetInfo.cpp#L2790) for how LLVM does it. Note that their implementation is incomplete, but will get the "basic" types correct.

    c. The [merge](https://github.com/llvm/llvm-project/blob/main/clang/lib/CodeGen/TargetInfo.cpp#L2750) and [postMerge](https://github.com/llvm/llvm-project/blob/main/clang/lib/CodeGen/TargetInfo.cpp#L2717) steps should probably be factored out, but not accessible by the user of our class.

2. Allocate register/memory resources
    a. I haven't found code for this yet. I'll keep looking

3. Handle return values
    1. It's the same classification rules, but the allocation is different
