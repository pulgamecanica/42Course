import time
import math
import os


def time_format(s: float):
    """Convert the seconds to a time (hh:mm:ss)

        Details
        hours:minutes:seconds

        Params:
            s: seconds to be formated
    """
    if s is None:
        return "?"
    mins, secs = divmod(s, 60)
    hours, mins = divmod(mins, 60)
    if hours == 0:
        return f"{int(mins):02d}:{int(secs):02d}"
    return f"{int(hours):02d}:{int(mins):02d}:{int(secs):02d}"


def ft_tqdm(lst: range) -> None:
    """Mimic the original tqdm function

        Details:
            "?" symbol for unknown time formats or iterations
            s/it & it/s show s/it when seconds per iteraton is higher than 1

    spi: seconds per iteration
    ips: iterations per second
    """
    w = os.get_terminal_size()[0]
    total = len(lst)
    percentage_w = 4
    bar_margin_w = 2
    bar_margin = "|"
    bar_fill = "█"
    timestamp = time.time()
    i = 0
    while (i <= total):
        percentage = i / total
        items_w = len(str(i)) + len(" / ") + len(str(total))
        # Time info
        time_info_elapsed = time.time() - timestamp
        spi = None if i == 0 else (float)(time_info_elapsed / i)
        ips = None if time_info_elapsed == 0 else float(i / time_info_elapsed)
        time_info_left = None if spi is None else (total - i) / ips
        time_info_iterations = "?"
        if ips and spi:
            time_info_iterations = f"{spi:.2f}" if spi > 1 else f"{ips:.2f}"
        time_info_iterations_suffix = "s/it" if spi and spi > 1 else "it/s"
        time_info = (
            f"[{time_format(time_info_elapsed)}"
            f"<{time_format(time_info_left)}, "
            f"{time_info_iterations}{time_info_iterations_suffix}]"
        )
        time_info_w = len(time_info)
        # Prepare Bar
        bar_w = w - percentage_w - bar_margin_w - items_w - time_info_w
        bar_filled = bar_fill * (int)(percentage * bar_w)
        bar = bar_filled + (" " * (bar_w - len(bar_filled)))
        print("\r"
              f"{math.ceil(percentage*100):-3d}%"
              f"{bar_margin}{bar}{bar_margin}"
              f" {i}/{total} "
              f"{time_info}", end="", flush=True)
        if i != total:
            yield i
        i += 1
