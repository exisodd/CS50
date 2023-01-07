#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
} pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
    }

    // =========== KEEP TRACK OF CANDIDATES AND APPROPRIATE INDEXES ===================
    // printf("===CANDIDATES===\n");
    // for (int a = 0; a < candidate_count; a++)
    // {
    //     printf("%s ", candidates[a]);
    // }
    // printf("\n");
    // printf("===CORRESPONDING INDEXES===\n");
    // for (int a = 0; a < candidate_count; a++)
    // {
    //     printf("%i ", a);
    // }
    // printf("\n");
    // printf("============================\n");
    // =========== KEEP TRACK OF CANDIDATES AND APPROPRIATE INDEXES ===================

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        // test
        // for (int t = 0; t < candidate_count; t++)
        // {
        //     printf("RANKS: %i\n", ranks[t]);
        // }
        // test

        record_preferences(ranks);

        // printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    // TODO
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(name, candidates[i]) == 0)
        {
            ranks[rank] = i;
            return true;
        }
    }
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    // TODO
    for (int i = 0; i < candidate_count; i++)
    {
        // printf("Candidate %i preferred over candidates ", ranks[i]);
        for (int j = i + 1; j < candidate_count; j++)
        {
            // printf("%i, ", ranks[j]);
            preferences[ranks[i]][ranks[j]] += 1;
        }
        printf("\n");
    }

    // =============PRINT PREFERENCES=================
    // for (int i = 0; i < MAX; i++)
    // {
    //     for (int j = 0; j < MAX; j++)
    //     {
    //         printf("%i ", preferences[i][j]);
    //     }
    //     printf("\n");
    // }
    // =============PRINT PREFERENCES=================
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    // TODO
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            if (j > i)
            {

                if (preferences[i][j] > preferences[j][i])
                {
                    pair paira;
                    paira.winner = i;
                    paira.loser = j;
                    pairs[pair_count] = paira;
                    // printf("[%i, %i]: %i\n", paira.winner, paira.loser, preferences[i][j]);
                    pair_count += 1;
                }
                else if (preferences[j][i] > preferences[i][j])
                {
                    pair paira;
                    paira.winner = j;
                    paira.loser = i;
                    pairs[pair_count] = paira;
                    // printf("[%i, %i]: %i\n", paira.winner, paira.loser, preferences[j][i]);
                    pair_count += 1;
                }
            }
        }
        // printf("\n");
    }
    // ============= PRINT PAIRS ===================
    // for (int i = 0; i < pair_count; i++)
    // {
    //     printf("[%i, %i]\n", pairs[i].winner, pairs[i].loser);
    // }
    // ============= PRINT PAIRS ===================
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    // TODO
    for (int i = 0; i < pair_count; i++)
    {
        int winnera = pairs[i].winner;
        int losera = pairs[i].loser;
        int paira_strength = preferences[winnera][losera] - preferences[losera][winnera];
        for (int j = i + 1; j < pair_count; j++)
        {
            int winnerb = pairs[j].winner;
            int loserb = pairs[j].loser;
            int pairb_strength = preferences[winnerb][loserb] - preferences[loserb][winnerb];
            if (pairb_strength > paira_strength)
            {
                pair temporary_pair = pairs[i];
                pairs[i] = pairs[j];
                pairs[j] = temporary_pair;
            }
        }
    }
    // for (int i = 0; i < pair_count; i++)
    // {
    //     // printf("[%i, %i]\n", pairs[i].winner, pairs[i].loser);
    // }
    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    // TODO
    // Create temporary locked pairs array that will be looped through.
    pair locked_pairs[1000];
    pair explored_pairs[1000];

    for (int i = 0; i < pair_count; i++)
    {
        bool breakloop;
        bool cyclebreak;
        // Set current pair to pairs[i]. Current pair will be changed if match
        pair current_pair = pairs[i];

        // Add pair temporarily to locked_pairs, which could get removed later if pair creates cycle
        locked_pairs[i] = pairs[i];

        printf("======================================\n");
        printf("CURRENT PAIR EXAMINED: [%i, %i]\n", pairs[i].winner, pairs[i].loser);

        // Loop through locked_pairs to check if it creates a cycle
        while (true)
        {
            // Set tracker to keep track of number of pairs that current pair points to:
            int counter = 0;
            // Automatically set to true, if forloop executes without detecting any pairs, loop will break
            breakloop = true;
            cyclebreak = false;

            // For loop, compare current pair to each pair in locked_pairs
            printf("Current pair: [%i, %i] compared to pairs: ", current_pair.winner, current_pair.loser);

            for (int j = 0; j < pair_count; j++)
            {
                printf("[%i, %i] ", locked_pairs[j].winner, locked_pairs[j].loser);
                // If current pair points to this pair, exec
                if (current_pair.loser == locked_pairs[j].winner && (locked_pairs[j].winner != 0 || locked_pairs[j].loser != 0))
                {
                    printf("MATCH: points towards [%i, %i]\n", locked_pairs[j].winner, locked_pairs[j].loser);
                    // Set current pair to this pair, and return to the top of the loop to restart the process until no pointer to any pairs
                    current_pair = locked_pairs[j];
                    printf("CURRENT PAIR: [%i, %i]\n", current_pair.winner, current_pair.loser);
                    breakloop = true;
                    break;
                }
            }

            printf("\n");

            // Check if current pair points to examined pair
            for (int p = 0; p < pair_count; p++)
            {
                // If current pair points to examined pair, that means that pair will create cycle
                if (current_pair.loser == pairs[i].winner && (locked_pairs[p].winner != 0 || locked_pairs[p].loser != 0) && (locked_pairs[p].winner != pairs[i].winner && locked_pairs[p].winner != pairs[i].winner))
                {
                    // Don't lock pair, and remove last temporary pair from locked_pairs, so that
                    // next loop we don't go over this temporary pair
                    // Since removing element is not possible, we set temporary pair winner to -1 (not possible)
                    locked_pairs[i].winner = -1;
                    locked_pairs[i].loser = -1;
                    printf("PAIR CREATES CYCLE: (%s, %s)\n", candidates[pairs[i].winner], candidates[pairs[i].loser]);
                    cyclebreak = true;
                    break;
                }
            }

            if (cyclebreak)
            {
                break;
            }
            // NO CYCLE CREATED
            if (breakloop)
            {
                // Lock pair, and don't remove temporary pair (it stays permanent)
                locked[pairs[i].winner][pairs[i].loser] = true;
                printf("LOCKED PAIR: [%s, %s]\n", candidates[pairs[i].winner], candidates[pairs[i].loser]);
                break;
            }
        }
    }
    return;
}

// Print the winner of the election
void print_winner(void)
{
    // TODO
    bool won;
    for (int i = 0; i < candidate_count; i++)
    {
        won = true;
        for (int j = 0; j < pair_count; j++)
        {
            if (pairs[j].loser == i)
            {
                // printf("%s has lost.\n", candidates[i]);
                won = false;
                break;
            }
        }
        if (won)
        {
            printf("%s\n", candidates[i]);
            return;
        }
    }
    return;
}
