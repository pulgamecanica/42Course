from S1E7 import Baratheon, Lannister


Robert = Baratheon("Robert")
print(Robert)
print(Robert.__dict__)
print(Robert.__str__)
print(Robert.__repr__)
print(Robert.is_alive)
Robert.die()
print(Robert.is_alive)
print(Robert.__doc__)
print("---")
Cersei = Lannister("Cersei")
print(Cersei.__dict__)
print(Cersei.__str__)
print(Cersei.is_alive)
print("---")
Jaine = Lannister.create_lannister("Jaine", True)
print(f"""\
Name : {Jaine.first_name, type(Jaine).__name__}, Alive : {Jaine.is_alive}""")
