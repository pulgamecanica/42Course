import random
import string
from dataclasses import dataclass, field


def generate_id() -> str:
    return "".join(random.choices(string.ascii_lowercase, k=15))


@dataclass()
class Student:
    """Class for keeping track of an item in inventory."""
    name: str
    surname: str
    active: bool = True
    login: str = field(init=False, default="Eagle")
    id: str = field(init=False, default=generate_id())
