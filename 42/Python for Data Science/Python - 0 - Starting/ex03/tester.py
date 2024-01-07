from NULL_not_found import NULL_not_found

Nothing = None
Garlic = float("NaN")
Zero = 0
Empty = ""
Fake = False

NULL_not_found(Nothing)
NULL_not_found(Garlic)
NULL_not_found(Zero)
NULL_not_found(Empty)
NULL_not_found(Fake)
print(NULL_not_found("Brian"))
print(NULL_not_found(float("NaN")))
print(NULL_not_found(True))
print(NULL_not_found(1))
print(NULL_not_found({}))
print(NULL_not_found([]))