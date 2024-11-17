# matrix [Rust Implementation]

***

The project consists on developing a Library for a Vector and Matrix with several operations

***

### Documentation 

You can visit the documentation:

- [Matrix](https://pulgamecanica.github.io/MatrixDoc/matrix/matrix/struct.Matrix.html)
- [Vector](https://pulgamecanica.github.io/MatrixDoc/matrix/vector/struct.Vector.html)
- [Scalar](https://pulgamecanica.github.io/MatrixDoc/matrix/scalar/trait.Scalar.html)
- [Complex](https://pulgamecanica.github.io/MatrixDoc/matrix/complex/struct.Complex.html)

***

### Generics

Both Matrix and Vector should support data type generics, for instance f64 or f32 as well as a complex number (for the bonus)

### Modules

The project is divided in 16 modules.

| # | Name | Example |
| - | ---- | ------- |
| 0 | Add, Sub & Scl | `v1 + v2`, `m1 - m2` & `v * 3.0` |
| 1 | Linear Combination | Providing a list of objects and an instance of the same object, compute all the combinations |
| 2 | Linear Interpolation | To compute the instance object defined between two objects (or beyond) |
| 3 | Dot Product | Compute the dot product of two vectors, uses `fma` |
| 4 | Norm | Compute the norm for the following kinds of norms: `Manhattan`, `Euclidean` & `Supremum/maximum` |
| 5 | Cosine | Compute the cosine between two vectors |
| 6 | Cross Product | Compute the cross product between two 3-dimentional vectors |
| 7 | Linear map, Matrix multiplication | Multiply a Matrix by a Vector, and by another Matrix |
| 8 | Trace | Calculate a sqare matrixe's trace |
| 9 | Transpose | Transpose a Matrix |
| 10 | Row Echelon | Get the Gauss Jordan (row echelon) of a rectangular matrix |
| 11 | Determinant | Compute the determinant of a square matrix |
| 12 | Inverse | Compute the inverse of a square matrix |
| 13 | Rank | Compute the rank of a rectangular matrix |
| 14 | Projection Matrix | Compute a projection Matrix |
| 15 | Complex numbers | Develop a Complex number to use with your library |
