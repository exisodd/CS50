while True:
    height = input("Height: ")

    # Try converting height to int
    try:
        height = int(height)
    except:
        continue
    # Check if height is within 1 and 8
    if height >= 1 and height <= 8:
            break

for i in range(height, 0, -1):
    # Print pyramid
    print(' ' * (i - 1), end='')
    print('#' * (height - i + 1), end='')
    print('  ', end='')
    print('#' * (height - i + 1))