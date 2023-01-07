-- Keep a log of any SQL queries you execute as you solve the mystery.

-- CRIME SCENE REPORTS--
SELECT * FROM crime_scene_reports WHERE day = 28 AND year = 2021 AND month = 7 AND street = "Humphrey Street";
/*
    - ID of crime report: 295
    - Three witnesses present
    - Mention of bakery
    - Time: 10:15am
*/


-- INTERVIEWS --
SELECT * FROM interviews WHERE day = 28 AND year = 2021 AND month = 7;
/*
    - ID: 161, 162, 163
    - Ruth:Sometime within ten minutes of the theft, I saw the thief get into a car in the bakery parking lot and drive away.
      If you have security footage from the bakery parking lot, you might want to look for cars that left the parking lot in that time frame.

    - Eugene: I don't know the thief's name, but it was someone I recognized. Earlier this morning, before I arrived at Emma's bakery,
      I was walking by the ATM on Leggett Street and saw the thief there withdrawing some money.

    - Raymond: As the thief was leaving the bakery, they called someone who talked to them for less than a minute.
      In the call, I heard the thief say that they were planning to take the earliest flight out of Fiftyville tomorrow.
      The thief then asked the person on the other end of the phone to purchase the flight ticket.
*/


-- BAKERY LOGS--
-- Info: Within 10 min of theft, thief gets into parking and drives away
SELECT * FROM bakery_security_logs WHERE day = 28 AND year = 2021 AND month = 7 AND hour = 10 AND minute > 15 AND minute <= (15 + 10);
-- Possible license plates:
SELECT license_plate FROM bakery_security_logs WHERE day = 28 AND year = 2021 AND month = 7 AND hour = 10 AND minute > 15 AND minute <= (15 + 10);


-- ATM TRANSACTIONS --
-- Info: before 10:15am, Leggett Street, Transaction: Withdraw
SELECT * FROM atm_transactions WHERE day = 28 AND year = 2021 AND month = 7 AND atm_location = "Leggett Street" AND transaction_type = "withdraw";
-- Possible bank account numbers:
SELECT account_number FROM atm_transactions WHERE day = 28 AND year = 2021 AND month = 7 AND atm_location = "Leggett Street" AND transaction_type = "withdraw";

-- BANK ACCOUNTS --
SELECT * FROM bank_accounts
WHERE account_number IN (SELECT account_number FROM atm_transactions WHERE day = 28 AND year = 2021 AND month = 7 AND atm_location = "Leggett Street" AND transaction_type = "withdraw");
-- Possible people IDs:
SELECT person_id FROM bank_accounts WHERE account_number IN (SELECT account_number FROM atm_transactions WHERE day = 28 AND year = 2021 AND month = 7 AND atm_location = "Leggett Street" AND transaction_type = "withdraw");

-- PHONE CALLS --
-- Info: Time: <1min, after 10:15am, earliest flight out of Fiftyville tomorrow, ask other person on the other end to purchase ticket
SELECT * FROM phone_calls WHERE day = 28 AND year = 2021 AND month = 7 AND duration <= 60;
-- Possible caller phone numbers:
SELECT caller FROM phone_calls WHERE day = 28 AND year = 2021 AND month = 7 AND duration <= 60;
-- Possible receiver phone numbers:
SELECT receiver FROM phone_calls WHERE day = 28 AND year = 2021 AND month = 7 AND duration <= 60;

-- FLIGHTS --
-- Info: Day after 28th, earliest flight possible, destination: Fiftyville
SELECT * FROM flights WHERE day = 29 AND year = 2021 AND month = 7 ORDER BY hour, minute;
/*
  - Origin airport id: 8 (Fiftyvile)
  - Destination airport ID: 4
  - Flight ID: 36
  - Time: 8:20am
*/
-- Destination airport ids:
SELECT destination_airport_id FROM flights WHERE day = 29 AND year = 2021 AND month = 7 ORDER BY hour, minute;

-- AIRPORTS --
SELECT * FROM airports WHERE id = 4;
/*
  - Destionation city: New York City
  - Name: LaGuardia Airport
  - Abbreviation: LGA
*/

-- PASSENGERS --
-- Info: flight id 36
SELECT * FROM passengers WHERE flight_id = 36;
-- Possible passport numbers:
SELECT passport_number FROM passengers WHERE flight_id = 36;


-- PEOPLE --
-- Info:
-- License plate:
SELECT license_plate FROM bakery_security_logs WHERE day = 28 AND year = 2021 AND month = 7 AND hour = 10 AND minute > 15 AND minute <= (15 + 10);
-- Possible people IDs:
SELECT person_id FROM bank_accounts WHERE account_number IN (SELECT account_number FROM atm_transactions WHERE day = 28 AND year = 2021 AND month = 7 AND atm_location = "Leggett Street" AND transaction_type = "withdraw");
-- Possible phone numbers:
SELECT caller FROM phone_calls WHERE day = 28 AND year = 2021 AND month = 7 AND duration <= 60;
-- Possible passport numbers:
SELECT passport_number FROM passengers WHERE flight_id = 36;

-- People (thief):
SELECT * FROM people
WHERE license_plate IN (SELECT license_plate FROM bakery_security_logs WHERE day = 28 AND year = 2021 AND month = 7 AND hour = 10 AND minute > 15 AND minute <= (15 + 10))
AND id IN (SELECT person_id FROM bank_accounts WHERE account_number IN (SELECT account_number FROM atm_transactions WHERE day = 28 AND year = 2021 AND month = 7 AND atm_location = "Leggett Street" AND transaction_type = "withdraw"))
AND phone_number IN (SELECT caller FROM phone_calls WHERE day = 28 AND year = 2021 AND month = 7 AND duration <= 60)
AND passport_number IN (SELECT passport_number FROM passengers WHERE flight_id = 36);
/*
  - Name: Bruce
  - Phone number: (367) 555-5533
*/


-- FIND ACCOMPLICE PHONE NUMBER --
SELECT * FROM phone_calls WHERE day = 28 AND year = 2021 AND month = 7 AND duration <= 60 AND caller = "(367) 555-5533";
-- Accomplice's phone number: (375) 555-8161
SELECT * FROM people WHERE phone_number = "(375) 555-8161";
-- Accomplice: Robin
