import sys


def main():
    try:
        assert len(sys.argv) <= 2, "AssertionError: received wrong number of arguments"
    except AssertionError as msg:
        print(msg)


if __name__ == "__main__":
    main()
