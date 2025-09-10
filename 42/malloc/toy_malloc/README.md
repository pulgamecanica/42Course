This is a toy malloc

A minimal implementation which WORKS!

It's just not advanced, but it works and it respects the intrinsics of a memory allocator!

From this toy app I can learn how to prevent defragmentation and why we need to call less times mmap, and keeping bins to make our mem allocation super duper fast!

It would be possible to deliver the project like this if the subject didn't mention the following:

> With performance in mind, you must limit the number of calls to mmap(), but also
> to munmap(). You have to “pre-allocate” some memory zones to store your “small”
> and “medium” malloc.