import sys

# https://www.geeksforgeeks.org/morse-code-translator-python/
# Dictionary representing the morse code chart (from geeksforgeeks)
MORSE_CODE_DICT = {"A": ".-", "B": "-...",
                        "C": "-.-.", "D": "-..", "E": ".",
                        "F": "..-.", "G": "--.", "H": "....",
                        "I": "..", "J": ".---", "K": "-.-",
                        "L": ".-..", "M": "--", "N": "-.",
                        "O": "---", "P": ".--.", "Q": "--.-",
                        "R": ".-.", "S": "...", "T": "-",
                        "U": "..-", "V": "...-", "W": ".--",
                        "X": "-..-", "Y": "-.--", "Z": "--..",
                        "1": ".----", "2": "..---", "3": "...--",
                        "4": "....-", "5": ".....", "6": "-....",
                        "7": "--...", "8": "---..", "9": "----.",
                        "0": "-----", ", ": "--..--", ".": ".-.-.-",
                        "?": "..--..", "/": "-..-.", "-": "-....-",
                        "(": "-.--.", ")": "-.--.-"}


def print_morse_code(message, separator="/"):
    """Function to convert and print
        a string according to morse code
    """
    morse = ""
    err_msg = "AssertionError: the arguments are bad"
    print_space_before = False
    for x, letter in enumerate(message):
        if letter == " ":
            morse += separator
            print_space_before = False
        else:
            assert letter in MORSE_CODE_DICT, err_msg
            if print_space_before:
                morse += " "
            morse += MORSE_CODE_DICT[letter]
            print_space_before = True
    print(morse)


def main():
    """Main function, computation starts here

The program should receive exactly 1 argument

The program will translate the string to morse
code and output the result

Errors
------
AssertionError
    More or less than 2 arguments passed
    """

    err_msg = "AssertionError: received wrong number of arguments"
    try:
        assert len(sys.argv) == 2, err_msg
        print_morse_code(sys.argv[1].upper())
    except AssertionError as msg:
        print(msg)


if __name__ == "__main__":
    main()
