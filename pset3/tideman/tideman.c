#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
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
}
pair;

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
int get_index(string name);
int compare(const void *pair_1, const void *pair_2);
bool cycle_helper(int index, bool visited[]);
bool get_output_helper(int index);
int get_output();

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

        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

int get_index(string name)
{
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(name, candidates[i]) == 0)
        {
            return i;
        }
    }
    return -1;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    // TODO
    int candidate_index = get_index(name);
    if (candidate_index != -1)
    {
        ranks[rank] = candidate_index;
        return true;
    }
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    // TODO
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            preferences[ranks[i]][ranks[j]]++;
        }
    }
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
            if (preferences[i][j] > preferences[j][i])
            {
                pair temp;
                temp.winner = i;
                temp.loser = j;
                pairs[pair_count++] = temp;
            }
        }
    }
    return;
}

int compare(const void *pair_1, const void *pair_2)
{
    pair p1 = *((pair *) pair_1);
    pair p2 = *((pair *) pair_2);
    int first = preferences[p1.winner][p1.loser] - preferences[p1.loser][p1.winner];
    int second = preferences[p2.winner][p2.loser] - preferences[p2.loser][p2.winner];
    return second - first;
}
// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    // TODO
    qsort(pairs, pair_count, sizeof(pair), compare);
    return;
}

// check cycle for recursion help
bool cycle_helper(int index, bool visited[])
{
    if (visited[index])
    {
        return true;
    }
    visited[index] = true;
    for (int i = 0; i < candidate_count; i++)
    {
        if (locked[index][i] && cycle_helper(i, visited))
        {
            return true;
        }
    }
    return false;
}
// call the recursion func to check the cycle
bool has_cycle(int index)
{
    bool visited[candidate_count];
    for (int i = 0; i < candidate_count; i++)
    {
        visited[i] = false;
    }
    return cycle_helper(index, visited);
}
// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    // TODO
    for (int i = 0; i < pair_count; i++)
    {
        locked[pairs[i].winner][pairs[i].loser] = true;
        if (has_cycle(pairs[i].winner))
        {
            locked[pairs[i].winner][pairs[i].loser] = false;
        }
    }
    return;
}
// check if is the output index
bool get_output_helper(int index)
{
    for (int i = 0; i < candidate_count; i++)
    {
        if (locked[i][index])
        {
            return false;
        }
    }
    return true;
}
// get the final result index
int get_output()
{
    for (int i = 0; i < candidate_count; i++)
    {
        if (get_output_helper(i))
        {
            return i;
        }
    }
    return -1;
}
// Print the winner of the election
void print_winner(void)
{
    // TODO
    int index = get_output();
    if (index != -1)
    {
        printf("%s\n", candidates[index]);
    }
    return;
}

