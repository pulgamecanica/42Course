import sys


# Comments & Documentation
# https://realpython.com/documenting-python-code/#commenting-vs-documenting-code


def analyse(data: str = None):
    """Analyse the string content

The following elements are analysed:
    - upper letters
    - lower letters
    - punctuation marks
    - spaces
    - digits

Note
----
The punctuation marks to be concidered are the following:
    !,';".-?

Parameters
----------
data: str
    The string to be analysed"""

    marks = ("!", ",", "\'", ";", "\"", ".", "-", "?")
    if not data:
        data = input("Enter the data: ")
    print(f"The text contains {len(data)} characters:")
    print(f"{ sum([1 if _.isupper() else 0 for _ in data]) } upper letters")
    print(f"{ sum([1 if _.islower() else 0 for _ in data]) } lower letters")
    print(f"{ sum([1 if _ in marks else 0 for _ in data]) } punctuation marks")
    print(f"{ sum([1 if _.isspace() else 0 for _ in data]) } spaces")
    print(f"{ sum([1 if _.isdigit() else 0 for _ in data]) } digits")


def main():
    """Main function, computation starts here

The program will only handle one string.
If no argument is provided the user will be
input to enter one string.

Errors
------
AssertionError
    If more than one argument is passed
    """

    err_msg = "AssertionError: received wrong number of arguments"
    try:
        assert len(sys.argv) <= 2, err_msg
        analyse(sys.argv[1] if len(sys.argv) == 2 else None)
    except AssertionError as msg:
        print(msg)


if __name__ == "__main__":
    main()
