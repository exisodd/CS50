from sys import exit

while True:
    number = input("Number: ")

    # Try converting height to int
    try:
        int(number)
    except:
        # If exception, go back to start of while loop
        continue
    break

length = len(number)
total = 0
for i in range(len(number) - 1, 0, -2):
    # Loop over every other digit startingfrom the first
    num = int(number[i - 1])
    num *= 2
    # Add individual digits
    if num >= 10:
        n1 = int(num % 10)
        n2 = int(num % 100 / 10)
        total += n1
        total += n2
    else:
        total += num

# Add sum to the numbers that weren't multiplied by 2
for i in range(len(number) - 1, -1v, -2):
    total += int(number[i])

# Validate Luhn's algorithm
if total % 10 != 0:
    print("INVALID")
    exit()

# Validate American Express
if length == 15:
    if number[0] == '3':
        if number[1] == '4' or number[1] == '7':
            print("AMEX")
            exit()

# Validate MasterCard
if length == 16:
    if number[0] == '5':
        if int(number[1]) >= 1 and int(number[1]) <= 5:
            print("MASTERCARD")
            exit()

# Validate Visa
if length == 13 or length == 16:
    if number[0] == '4':
        print("VISA")
        exit()

# If card matches none of the Credit number patterns, that means it is invalid
print("INVALID")
