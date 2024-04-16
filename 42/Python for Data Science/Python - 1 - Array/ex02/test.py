from load_image import ft_load
import sys


def main():
    if len(sys.argv) == 2:
        print(ft_load(sys.argv[1]))
    else:
        print("[Error] Wrong number of arguments")


if __name__ == '__main__':
    main()
