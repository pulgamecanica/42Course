import pandas


def load(path: str) -> pandas.core.frame.DataFrame:
    """load

    Details:
    Load the file pointed by path
    Error will return empty dataset
    """
    try:
        return pandas.read_csv(path)
    except FileNotFoundError as msg:
        print(msg)
    return pandas.core.frame.DataFrame()
