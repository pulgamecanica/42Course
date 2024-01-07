ft_list = ["Hello", "tataw!"]
ft_tuple = ("Hello", "toto!")
ft_set = {"Hello", "tutu!"}
ft_dict = {"Hello" : "titi!"}

# My code goes here

# https://docs.python.org/3.11/library/stdtypes.html#list
ft_list[1] = "World!"

# https://docs.python.org/3.11/library/stdtypes.html#tuple
ft_tuple = ("Hello", "France!")

# https://docs.python.org/3.11/library/stdtypes.html#set
ft_set.remove("tutu!")
ft_set.add("Paris!")

# https://docs.python.org/3.11/library/stdtypes.html#dict
ft_dict["Hello"] = "42Paris!"

print(ft_list)
print(ft_tuple)
print(ft_set)
print(ft_dict)