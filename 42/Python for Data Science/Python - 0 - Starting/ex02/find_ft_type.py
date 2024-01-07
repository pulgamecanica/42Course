def all_thing_is_obj(object: any) -> int:
    types_dict = {
    list: "List",
    tuple: "Tuple",
    set: "Set",
    dict: "Dict"
    }
    if type(object) == str:
        print(f"{object} is in the kitchen : { type(object) }")
    elif type(object) in types_dict:
        print(f"{ types_dict[type(object)] :5s} : { type(object) }")
    else:
        print("Type not found")
    return 42
