from load_csv import load
import matplotlib.pyplot as plt


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
    if type(text) is str and text[-1].upper() in d:
        num, magnitude = text[:-1], text[-1].upper()
        return float(num) * 10 ** d[magnitude]
    else:
        return float(text)


def plot_income_against_life_expectancy(countries, year):
    """plot_income_against_life_expectancy

    Details:
    Iterates the list of countries and for the year given
    plots the income vs the life expectancey (x axis & y axis)
    """
    incm = load(
                "./income_per_person_gdppercapita_ppp_inflation_adjusted.csv")
    expect = load("./life_expectancy_years.csv")
    if incm.empty or expect.empty:
        print("Warning, files were not found")
        return
    fig, ax = plt.subplots()
    ax.set_xscale('log')
    if len(incm.get(str(year), [])) == 0:
        print("Warning, year not founded in the data set")
        return
    for country in countries:
        income = incm[incm["country"] == country][str(year)].values
        expectancy = expect[expect["country"] == country][str(year)].values
        values_normalized = [text_to_num(income[0]),
                             text_to_num(expectancy[0])]
        ax.scatter(values_normalized[0], values_normalized[1], c="royalblue")


def main():
    """Show country data

    Details:
    Plot the list of countries population between two dates
    """
    year = 1900
    ds = load("./income_per_person_gdppercapita_ppp_inflation_adjusted.csv")
    if ds.empty:
        return
    countries = ds["country"].values
    plot_income_against_life_expectancy(countries, year)
    plt.title(f"Year {year}")
    plt.xlabel('Gross Domestic Product')
    plt.ylabel('Life Expectancy')
    plt.show()


if __name__ == "__main__":
    main()
