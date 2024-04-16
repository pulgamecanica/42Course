import time

print(f"Seconds since January 1, 1970: {time.time():,.4f} or {time.time():.2e} in scientific notation")

# https://docs.python.org/3.11/library/time.html#time.strftime
print(time.strftime("%b %d %Y"))

