import sys

def is_num(n):
    try:
        int(n)
        return True
    except ValueError:
        return False

if len(sys.argv) != 1:
    try:
        assert len(sys.argv) == 2, "AssertionError: more than one argument is provided"
        assert is_num(sys.argv[1]), "AssertionError: argument is not an integer"
        print("I'm", "Odd" if (int(sys.argv[1]) % 2 != 0) else "Even")
    except AssertionError as msg: 
        print(msg)