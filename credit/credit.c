#include <cs50.h>
#include <stdio.h>
#include <math.h>

int main(void)
{
    long credit = get_long("Number: ");
    int length = floor(log10(credit)) + 1;
    long num = 0;
    long modulo = 0;
    long divider = 0;
    long sum = 0;
    int first = 0;
    int second = 0;

    if (length >= 13 && length <= 16)
    {
        for (int i = 2; i <= length; i += 2)
        {
            modulo = pow(10, i);
            divider = pow(10, i - 1);
            num = credit % modulo / divider;
            num *= 2;
            if (num >= 10)
            {
                second = num % 10;
                first = (num % 100) / 10;
                sum += first;
                sum += second;
            }
            else
            {
                sum += num;
            }
        }

        for (int i = 1; i <= length; i += 2)
        {
            modulo = pow(10, i);
            divider = pow(10, i - 1);
            num = credit % modulo / divider;
            sum += num;
        }

        if (sum % 10 == 0)
        {

            // Debit is valid: Determine which type of debit card it is

            // AMERICAN EXPRESS - Length 15 - starts with 34 or 37
            if (length == 15)
            {
                long div = pow(10, 13);
                long starting_number = credit / div;
                if (starting_number == 34 || starting_number == 37)
                {
                    printf("AMEX\n");
                }
                else
                {
                    printf("INVALID\n");
                }
            }
            // VISA - Length 13 - starts with 4
            else if (length == 13)
            {
                long div = pow(10, 12);
                long starting_number = credit / div;
                if (starting_number == 4)
                {
                    printf("VISA\n");
                }
            }

            // VISA - Length 16 - starts with 4
            // MASTERCARD - Length 16 - starts with 51, 52, 53, 54, or 55
            else if (length == 16)
            {
                long starting_number = credit / pow(10, 15);
                if (starting_number == 4)
                {
                    printf("VISA\n");
                }
                else
                {
                    starting_number = credit / pow(10, 14);
                    if (starting_number >= 51 && starting_number <= 55)
                    {
                        printf("MASTERCARD\n");
                    }
                    else
                    {
                        printf("INVALID\n");
                    }
                }
            }
        }
        else
        {
            printf("INVALID\n");
        }
    }
    else
    {
        printf("INVALID\n");
    }
}