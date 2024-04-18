import sys
import matplotlib.pyplot as plt
from matplotlib.ticker import FuncFormatter
from load_csv import load


def text_to_num(text):
    """text_to_num

    Details:
    Convert string number to decimal.
    Accepts suffixes: [K, M, B, T]
    """
    d = {
        "K": 3,
        "M": 6,
        "B": 9,
        "T": 12
    }
    if text[-1].upper() in d:
        num, magnitude = text[:-1], text[-1].upper()
        return float(num) * 10 ** d[magnitude]
    else:
        return float(text)


def plot_countries_between(countries: list[str], start_year, end_year) -> None:
    """Get country data

    Details:
    Get a country data between two dates both dates are included
    """
    ds = load("./population_total.csv")
    if ds.empty:
        return
    ds_start_year = int(ds.columns[1])
    ds_end_year = int(ds.columns[-1])
    if start_year < ds_start_year or end_year > ds_end_year:
        print("Warning: The dates you gave are not available in the dataset")
    if start_year > end_year:
        print("Warning: Start is bigger than end")
    start_index = start_year - ds_start_year + 1
    end_index = end_year - ds_start_year + 2
    for country in countries:
        if not ds[ds["country"] == country].empty:
            country_pop = ds[ds["country"] == country].values.flatten()
            country_pop = country_pop[start_index:end_index]
            country_pop = [text_to_num(_) for _ in country_pop]
            plt.plot(
                [_ for _ in range(start_year, end_year+1)],
                country_pop, label=country)
        else:
            print(f"Warning: '{country}' info was not found")


def formatter(x, pos):
    """
    Convert the tick valie to a string with a suffix.

    '': none
    'K': thousands
    'M': millions
    'B': billions
    'T': trillions
    """
    suffixes = ['', 'K', 'M', 'B', 'T']
    order = 0
    while x >= 1000 and order < len(suffixes) - 1:
        order += 1
        x /= 1000.0
    return f"{x:.0f}{suffixes[order]}"


def main():
    """Show country data

    Details:
    Plot the list of countries population between two dates
    """
    countries = []
    if (len(sys.argv) == 1):
        print("Using default list")
        countries = ["France", "Belgium"]
    else:
        countries = sys.argv[1:]
    start_year = 1800
    end_year = 2050
    plot_countries_between(countries, start_year, end_year)
    plt.title("Population Projections")
    plt.legend()
    plt.xlabel("Year")
    plt.ylabel("Population")
    plt.gca().yaxis.set_major_formatter(FuncFormatter(formatter))
    plt.show()


if __name__ == "__main__":
    main()
