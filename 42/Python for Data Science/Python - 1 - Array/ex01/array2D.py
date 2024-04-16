def slice_me(family: list, start: int, end: int) -> list:
    """slice_me

    Details:
    Slice an array at the start and end and return it.
    Print the original size. Print the new size."""
    col_size = len(family[0])
    rect_martix = len([_ for _ in family if len(_) != col_size]) == 0
    if not rect_martix:
        print("Warning, your matrix is not rectangular, columns vary")
    print(f"My shape is : ({len(family)}, {len(family[0])})")
    new_arr = family[start:end]
    print(f"My new shape is : ({len(new_arr)}, {len(new_arr[0])})")
    return new_arr
