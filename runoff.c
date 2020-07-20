#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max voters and candidates
#define MAX_VOTERS 100
#define MAX_CANDIDATES 9

// preferences[i][j] is jth preference for voter i
int preferences[MAX_VOTERS][MAX_CANDIDATES];

// Candidates have name, vote count, eliminated status
typedef struct
{
    string name;
    int votes;
    bool eliminated;
}
candidate;

// Array of candidates
candidate candidates[MAX_CANDIDATES];

// Numbers of voters and candidates
int voter_count;
int candidate_count;

// Function prototypes
bool vote(int voter, int rank, string name);
void tabulate(void);
bool print_winner(void);
int find_min(void);
bool is_tie(int min);
void eliminate(int min);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: runoff [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX_CANDIDATES)
    {
        printf("Maximum number of candidates is %i\n", MAX_CANDIDATES);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i].name = argv[i + 1];
        candidates[i].votes = 0;
        candidates[i].eliminated = false;
    }

    voter_count = get_int("Number of voters: ");
    if (voter_count > MAX_VOTERS)
    {
        printf("Maximum number of voters is %i\n", MAX_VOTERS);
        return 3;
    }

    // Keep querying for votes
    for (int i = 0; i < voter_count; i++)
    {

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            // Record vote, unless it's invalid
            if (!vote(i, j, name))
            {
                printf("Invalid vote.\n");
                return 4;
            }
        }

        printf("\n");
    }

    // Keep holding runoffs until winner exists
    while (true)
    {
        // Calculate votes given remaining candidates
        tabulate();

        // Check if election has been won
        bool won = print_winner();
        if (won)
        {
            break;
        }

        // Eliminate last-place candidates
        int min = find_min();
        bool tie = is_tie(min);

        // If tie, everyone wins
        if (tie)
        {
            for (int i = 0; i < candidate_count; i++)
            {
                if (!candidates[i].eliminated)
                {
                    printf("%s\n", candidates[i].name);
                }
            }
            break;
        }

        // Eliminate anyone with minimum number of votes
        eliminate(min);

        // Reset vote counts back to zero
        for (int i = 0; i < candidate_count; i++)
        {
            candidates[i].votes = 0;
        }
    }
    return 0;
}

// Record preference if vote is valid
bool vote(int voter, int rank, string name)
{
    for (int k = 0; k < candidate_count; k++) // Iterate over the candidates array to match vote to candidate. Update preferences[i][j] to that candidate
    {
        if (strcmp(candidates[k].name, name) == 0)
        {
            preferences[voter][rank] = k;
            return true;
        }
    }
    return false;
}

// Tabulate votes for non-eliminated candidates
void tabulate(void)
{
    for (int i = 0; i < voter_count; i++)
    {
        int j = 0;
        while (candidates[preferences[i][j]].eliminated == true) // checking if their favourite is still in the race, if not moving down list of preferences
        {
            j++;
        }
        candidates[preferences[i][j]].votes++; // adding vote to the highest ranked choice sitll in the race
    }
    return;
}

// Print the winner of the election, if there is one
bool print_winner(void)
{
    int majority = (voter_count / 2);
    for (int i = 0; i < candidate_count; i++)
    {
        if (candidates[i].votes > majority) //checking if the candidate has more votes than half of the total voters
        {
            printf("%s\n", candidates[i].name);
            return true;
        }
    }
    return false;
}

// Return the minimum number of votes any remaining candidate has
int find_min(void)
{
    int min = candidates[0].votes;
    for (int i = 1; i < candidate_count; i++) //iterating over all candidates
    {
        if (candidates[i].eliminated == false && min > candidates[i].votes) // checking if their votes is lower than min and they are not yet eliminated
        {
            min = candidates[i].votes;
        }
    }
    return min;
    return 0;
}

// Return true if the election is tied between all candidates, false otherwise
bool is_tie(int min)
{
    int tie_counter = 0;
    int remaining_candidates = 0;
    for (int i = 0; i < candidate_count; i++)
    {
        if (candidates[i].eliminated == false) // checking and storing how many are left in the race
        {
            remaining_candidates++;
        }

        if (candidates[i].eliminated == false && candidates[i].votes == min) //checking how many are left in the race and on the min votes
        {
            tie_counter++;
        }
    }
    if (tie_counter == remaining_candidates) //comparing those in the race vs those in the race with min votes
    {
        return true;
    }
    else
    {
        return false;
    }
    return false;
}

// Eliminate the candidate (or candidiates) in last place
void eliminate(int min)
{
    for (int i = 0; i < candidate_count; i++)
    {
        if (candidates[i].votes == min) // find candidate with votes equal to min votes and updated eliminated to true
        {
            candidates[i].eliminated = true;
        }
    }
    return;
}
