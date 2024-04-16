from time import sleep
from tqdm import tqdm
from Loading import ft_tqdm


def main():
    """Testing
    """
    for elem in ft_tqdm(range(120)):
        sleep(0.09)
    print()

    for elem in tqdm(range(120)):
        sleep(0.09)
    print()


if __name__ == "__main__":
    main()
