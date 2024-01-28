def ft_filter(function, iterable):
    """filter(function or None, iterable) --> filter object

Return an iterator yielding those items of iterable for which function(item)
is true. If function is None, return the items that are true."""

    if function:
        return (_ for _ in iterable if function(_))
    return (_ for _ in iterable if _)


# def main():
#     print("Testing ft_filter")
#     list1 = [True, 0, 2, False, 42, 42.42, None, None, 1]
#     list2 = {2: "London", 3: "Lisbon", False: "Berlin", 6: False}

#     # None function
#     list1filtered = list(filter(None, list1))
#     list1ft_filtered = list(ft_filter(None, list1))
#     list2filtered = list(filter(None, list2))
#     list2ft_filtered = list(ft_filter(None, list2))
#     assert list1ft_filtered == list1filtered
#     print("Passed test 1")
#     assert list2ft_filtered == list2filtered
#     print("Passed test 2")

#     # Lambdas
#     list3 = [4, 5, 6, 7, 8, 9, 10]

#     list1filtered = list(filter(lambda _: _ and _ >= 42, list1))
#     list1ft_filtered = list(ft_filter(lambda _: _ and _ >= 42, list1))
#     list3filtered = list(filter(lambda _: _ % 2, list3))
#     list3ft_filtered = list(ft_filter(lambda _: _ % 2, list3))
#     assert list1ft_filtered == list1filtered
#     print("Passed test 3")
#     assert list3ft_filtered == list3filtered
#     print("Passed test 4")


# if __name__ == "__main__":
#     main()
