from cs50 import get_string
import re

text = get_string("Text: ")
# Count letters
letters = len(re.findall("[a-zA-Z]", text))

# Count sentences
sentences = len(re.findall("[.?!]", text))

# Count words
words = text.count(' ') + 1

L = letters / words * 100
S = sentences / words * 100
num = round(0.0588 * L - 0.296 * S - 15.8)

if num < 1:
    print("Before Grade 1")
elif num >= 16:
    print("Grade 16+")
else:
    print(f"Grade {num}")