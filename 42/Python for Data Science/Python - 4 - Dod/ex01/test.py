from in_out import outer, square, pow


my_counter = outer(3, square)
print(my_counter())  # Output: 9
print(my_counter())  # Output: 81
print(my_counter())  # Output: 6561
print("---")
another_counter = outer(1.5, pow)
print(another_counter())  # Output: 1.8371173070873836
print(another_counter())  # Output: 3.056683336818703
print(another_counter())  # Output: 30.42684786675409
