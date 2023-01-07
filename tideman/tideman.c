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
bool explore(int winner, int loser, pair original);
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
    pair locked_pairs[100];
    int explorecounter = 0;
    for (int i = 0; i < pair_count; i++)
    {
        // printf("==========================\n");
        // printf("CURRENT PAIRS EXAMIMNED: (%i, %i) (%s -> %s)\n\n\n", pairs[i].winner, pairs[i].loser, candidates[pairs[i].winner], candidates[pairs[i].loser]);
        // Lock pair automatically, unlock if creates cycle
        if (explore(pairs[i].winner, pairs[i].loser, pairs[i]))
        {
            // printf("LOCKED PAIR: (%i, %i) (%s -> %s)\n", pairs[i].winner, pairs[i].loser, candidates[pairs[i].winner], candidates[pairs[i].loser]);
            locked_pairs[i] = pairs[explorecounter];
        }
        else
        {
            // printf("PAIR NOT LOCKED: (%i, %i) (%s -> %s)\n", pairs[i].winner, pairs[i].loser, candidates[pairs[i].winner], candidates[pairs[i].loser]);
        }

        explorecounter++;

        for (int j = 0; j < pair_count; j++)
        {
            // Loop through locked_pairs and set all of those to true
            for (int row = 0; row < candidate_count; row++)
            {
                for (int col = 0; col < candidate_count; col++)
                {
                    if (row == locked_pairs[j].winner && col == locked_pairs[j].loser && (locked_pairs[j].winner != 0 || locked_pairs[j].loser != 0))
                    {
                        locked[row][col] = true;
                    }
                }
            }
        }
    }
    return;
}

// Recursive helper function
// Takes pair as input and explores path
bool explore(int winner, int loser, pair original)
{
    // Locked pairs
    // printf("current pair: (%i, %i) compared to pairs: ", winner, loser);
    locked[winner][loser] = true;

    if (loser == original.winner)
    {
        locked[winner][loser] = false;
        // printf("pair points to original(%i, %i)\n", winner, loser);
        return false;
    }

    for (int i = 0; i < candidate_count; i++)
    {
        // Explore all pairs are pointed to
        // If any of the explored pairs return false (points to original), return false
        // If all pairs return true, that means no cycles are created
        // printf("(%i, %i, [%i]) ", loser, i, locked[loser][i]);
        if (locked[loser][i] == true)
        {
            // Pairs points to this pair
            // printf("<-- MATCH\n");
            if (explore(loser, i, original) == false)
            {
                // printf("points to original eventually\n");
                locked[winner][loser] = false;
                return false;
            }
            else
            {
                // printf("all good\n");
            }
        }
    }
    // If loop executes successfully, and no pairs are pinned by (if), then return true
    return true;
}

// Print the winner of the election
void print_winner(void)
{
    // TODO
    bool isloser;
    for (int i = 0; i < candidate_count; i++)
    {
        isloser = false;
        for (int j = 0; j < candidate_count; j++)
        {
            if (locked[j][i])
            {
                isloser = true;
                break;
            }
        }

        if (isloser)
        {
            continue;
        }
        else
        {
            printf("%s\n", candidates[i]);
            return;
        }
    }
}
