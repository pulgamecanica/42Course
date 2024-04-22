from statistics import ft_statistics


ft_statistics(1, 42, 360, 11, 64,
              toto="mean", tutu="median", tata="quartile")
print("-----")
ft_statistics(5, 75, 450, 18, 597, 27474, 48575,
              hello="std", world="var")
print("-----")
ft_statistics(5, 75, 450, 18, 597, 27474, 48575,
              ejfhhe="heheh", ejdjdejn="kdekem")
print("-----")
ft_statistics(toto="mean", tutu="median", tata="quartile")
print("-----")
try:
    ft_statistics(1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
                  mean="mean", median="var", quartiles="std",
                  std_dev="quartile", variance="median")
except TypeError as e:
    print("Error:", e)
