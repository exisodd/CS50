import csv
import sys


def main():

    # TODO: Check for command-line usage
    if len(sys.argv) != 3:
        sys.exit("Usage: python dna.py DATABASEFILE SEQUENCEFILE")

    database_name = sys.argv[1]
    sq_name = sys.argv[2]
    # TODO: Read database file into a variable
    database = []
    with open(database_name, 'r') as file:
        reader = csv.DictReader(file)
        for line in reader:
            name = line["name"]
            AGATC = int(line["AGATC"])
            AATG = int(line["AATG"])
            TATC = int(line["TATC"])
            if database_name == "databases/small.csv":
                database.append({"name": name, "AGATC": AGATC,
                                "AATG": AATG, "TATC": TATC})
            else:
                TTTTTTCT = int(line["TTTTTTCT"])
                TCTAG = int(line["TCTAG"])
                GATA = int(line["GATA"])
                GAAA = int(line["GAAA"])
                TCTG = int(line["TCTG"])
                database.append({"name": name, "AGATC": AGATC, "AATG": AATG, "TATC": TATC,
                                "TTTTTTCT": TTTTTTCT, "TCTAG": TCTAG, "GATA": GATA, "GAAA": GAAA, "TCTG": TCTG})

    # TODO: Read DNA sequence file into a variable
    DNA_FILE = open(sq_name, 'r')
    DNA_SEQUENCE = DNA_FILE.readline()
    DNA_FILE.close()

    # TODO: Find longest match of each STR in DNA sequence
    strs = ["AGATC", "AATG", "TATC"] if database_name == "databases/small.csv" else [
        "AGATC", "AATG", "TATC", "TTTTTTCT", "TCTAG", "GATA", "GAAA", "TCTG"]
    dna = {}

    """
    LMAO I wrote this function not knowing there already was a function longest_match :(
    """

    # Loop over every STR type
    for str in strs:
        # Create list to store consecutive sequences of STRs
        max_len = []
        # Get length of STR
        str_len = len(str)

        # Loop over every character in dna
        for i in range(len(DNA_SEQUENCE)):
            # If character is beginning of STR sequence
            if DNA_SEQUENCE[i:i+str_len] == str:
                # Create tracker to count number of times the sequence appears
                num = 1
                for j in range(i + str_len, len(DNA_SEQUENCE), str_len):
                    # Loop over following STRs of same length
                    # print(line[j:j+str_len])
                    if DNA_SEQUENCE[j:j+str_len] == str:
                        # Increment counter
                        num += 1
                    else:
                        # If STR does not match, that means the end of sequence has been found
                        # Append counter to max_len list
                        max_len.append(num)
                        break
                # Add maximum length of maximum to max list
                # Take highest value of max(longest sequence) and add to dna dict
                dna[f"{str}"] = max(max_len)

    # TODO: Check database for matching profiles
    # Loop every person in database
    for person in database:

        for key in person.keys():
            # Does not match, so skip to next person
            # Skip name key
            if key == "name":
                continue
            # If DNA sequence does not have that key, that means next person
            if key not in dna.keys():
                break
            # If does not match
            elif person[key] != dna[key]:
                break
        else:
            # Match has been found, loop executed successfully
            print(person["name"])
            return

    print("No match")
    return


def longest_match(sequence, subsequence):
    """Returns length of longest run of subsequence in sequence."""

    # Initialize variables
    longest_run = 0
    subsequence_length = len(subsequence)
    sequence_length = len(sequence)

    # Check each character in sequence for most consecutive runs of subsequence
    for i in range(sequence_length):

        # Initialize count of consecutive runs
        count = 0

        # Check for a subsequence match in a "substring" (a subset of characters) within sequence
        # If a match, move substring to next potential match in sequence
        # Continue moving substring and checking for matches until out of consecutive matches
        while True:

            # Adjust substring start and end
            start = i + count * subsequence_length
            end = start + subsequence_length

            # If there is a match in the substring
            if sequence[start:end] == subsequence:
                count += 1

            # If there is no match in the substring
            else:
                break

        # Update most consecutive matches found
        longest_run = max(longest_run, count)

    # After checking for runs at each character in seqeuence, return longest run found
    return longest_run


main()
