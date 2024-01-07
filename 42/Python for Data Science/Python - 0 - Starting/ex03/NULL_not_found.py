import math

def NULL_not_found(object: any) -> int:
    nulls_dict = {
    None: "Nothing",
    bool: [False, "Fake"],
    int: [0, "Zero"],
    "False": "Fake",
    "": "Empty"
    }
    if object.__hash__ and object in nulls_dict:
        print(f"{ nulls_dict[object] :7s} : { object } { type(object) }")
    elif type(object) == float and math.isnan(object):
        print(f"Cheese  : { object } { type(object) }")
    elif type(object) in nulls_dict and nulls_dict[type(object)][0] == object:
        print(f"{ nulls_dict[type(object)][1] :7s} : { object } { type(object) }")
    else:
        print("Type not Found")
        return 1
    return 0