import sys
from ft_filter import ft_filter


def is_num(n: int) -> bool:
    """Evaluate if a parameter is an integer

Returns True when the parameter is an integer
else returns False"""

    try:
        int(n)
        return True
    except ValueError:
        return False


def main():
    """Main function, computation starts here

The program should receive exactly 2 arguments
The first argument should be a string
The second argument should be a number

The program will split the words contained in the string
Then it will filter the ones which are greater than number passed

Errors
------
AssertionError
    More or less than 2 arguments passed
    Second argument not being a number
    """

    err_msg = "AssertionError: received wrong number of arguments"
    try:
        assert len(sys.argv) == 3, err_msg
        assert is_num(sys.argv[2]), "AssertionError: the arguments are bad"
        n = int(sys.argv[2])
        lst = sys.argv[1].split()
        print([_ for _ in ft_filter(lambda _: len(_) > n, lst)])
    except AssertionError as msg:
        print(msg)


if __name__ == "__main__":
    main()


# Tests
# a = filterstring("Hello the World", 3)
# b = filterstring("Hello the World", 99)
# print(a)
# print(b)
