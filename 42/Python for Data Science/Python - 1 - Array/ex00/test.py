import sys
from give_bmi import give_bmi, apply_limit


def is_num(i):
    """Evaluate if a parameter is an integer

    Returns True when the parameter is an integer
    else returns False"""
    try:
        int(i)
        return True
    except ValueError:
        return False


def main():
    """Take two list and a limit
        Uses the two list to generate a bmi list of indexes
        Uses the limit to evaluate which bmi is above it
    """
    try:
        error_msg = "AssertError: Bad arguments \"<lst1>\" \"<lst2>\" <limit>"
        assert len(sys.argv) == 4, error_msg
        assert is_num(sys.argv[3]), "AssertError: Limit should be an int"
        bmi = give_bmi(sys.argv[1].split(), sys.argv[2].split())
        print(f"BMI's {bmi} |{type(bmi)}|")
        print(apply_limit(bmi, int(sys.argv[3])))
    except AssertionError as msg:
        print(msg)


if __name__ == "__main__":
    main()
