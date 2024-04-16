import pandas
from load_csv import load
import matplotlib.pyplot as plt
import matplotlib.axes as axes
from matplotlib import animation


def update(frame: int, country_data: pandas.core.frame.DataFrame,
           ax: axes._axes.Axes):
    """update

    Details:
    Update the frame, from the current year
    """
    ax.clear()  # Clear the current plot
    year = 1800 + frame
    ax.plot(range(1800, year), country_data[:frame])
    ax.set_title(f'Life Expectancy in {"France"} ({year})')
    ax.set_xlabel('Year')
    ax.set_ylabel('Life Expectancy')
    ax.set_xlim(1800, 2100)
    ax.set_ylim(0, 100)


def get_country_data(country: str):
    """Show country data

    Details:
    Plot the graph of the country life expectancy along the time
    """
    ds = load("./life_expectancy_years.csv")
    return ds[ds['country'] == country].values.flatten()[1:]


def main():
    """Show country data

    Details:
    Plot the graph of the country life expectancy along the time
    """
    ani = None
    animate = True
    france_life_expectancy = get_country_data("France")
    if animate:
        fig, ax = plt.subplots()
        ani = (animation.FuncAnimation(
               fig, update, fargs=(france_life_expectancy, ax),
               frames=len(france_life_expectancy), interval=10, repeat=False))
    else:
        plt.plot(range(1800, 2101), france_life_expectancy)
        plt.title('Life Expectancy in France')
        plt.xlabel('Year')
        plt.ylabel('Life Expectancy')
        plt.xlim(1800, 2100)
        plt.ylim(0, 100)
    print(type(ani))
    plt.show()


if __name__ == '__main__':
    main()
