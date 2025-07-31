#ifndef REMOVE_CONST_HPP
#define REMOVE_CONST_HPP

namespace ft {

template <typename T>
struct remove_const {
  typedef T type;
};

template <typename T>
struct remove_const<const T> {
  typedef T type;
};

} // namespace ft

#endif // REMOVE_CONST_HPP


/*
https://isocpp.org/wiki/faq/const-correctness#constptrptr-conversion
# 📌 Why `T**` Causes Issues in `deque_random_access_iterator` for `const T`

### ✅ Vector Iterators Use `T*`

In `vector<T>`, elements are stored contiguously in a flat array. The iterator typically holds a `T*`:

```cpp
T* _ptr;
```

This works fine for both `T` and `const T` because:

* `int*` can convert to `const int*` (safe, allowed)
* So `vector<int>::iterator` can be implicitly converted to `vector<int>::const_iterator`

### ❌ Deque Iterators Use `T**`

In `deque<T>`, elements are stored in fixed-size blocks:

```cpp
T** _map;  // a map of blocks of T*
```

This breaks down when converting between non-const and const iterators:

* `int**` **does not convert** to `const int**`

### ⚠️ Why `int** → const int**` is invalid:

Because it would allow **breaking const-correctness** via aliasing:

```cpp
const int a = 42;
int* ptr = const_cast<int*>(&a); // forbidden but possible if `int**` is allowed
```

If C++ allowed `int** → const int**`, you could sneak in a mutable `int*` and write to a `const int`, violating const safety.

### 🧠 Why This Happens in Deque but Not in Vector

* **Vector** only needs `T*` (one level of indirection) → `int* → const int*` is allowed.
* **Deque** uses `T**` (two levels of indirection) → `int** → const int**` is **not** allowed.

### ✅ Solution: Strip the const

To safely allow `deque<T>::iterator → deque<T>::const_iterator`, we use:

```cpp
typename remove_const<T>::type** _map;
```

This forces `_map` to always be `non-const T**`, while the actual access through it still respects `T`'s constness.

---

## ✅ Summary Table

| Context    | Iterator Type     | Conversion Valid | Explanation                                        |
| ---------- | ----------------- | ---------------- | -------------------------------------------------- |
| Vector     | `T*`              | ✅ Yes            | `T* → const T*` is valid                           |
| Deque      | `T**`             | ❌ No             | `T** → const T**` is unsafe                        |
| Workaround | `remove_const<T>` | ✅ Yes            | Always store non-const pointers in map, cast later |

*/