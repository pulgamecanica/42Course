# FT_CONTAINERS

-	vector
-	map
-	stack

***

### GUIDE

#### 1 Iterators

[https://cplusplus.com/reference/iterator/iterator/](https://cplusplus.com/reference/iterator/iterator/)

	Iterators help you access a range of elements, most common type of iterators are pointers. Some iterators can be incremented, or acceses randomly.
	To start the containers projects, you would need first to do an iterator base class, so you can access the elements of the container type.

##### Elements

- a) typedef T 									value_type;
- b) typedef ptrdiff_t							difference_type;
- c) typedef T*									pointer;
- d) typedef T&									reference;
- e) typedef std::random_access_iterator_tag	iterator_category;

