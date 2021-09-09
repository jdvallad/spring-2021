// Joshua Valladares
// jdvallad
// Spring 2021 UCSC
//
// CSE 13S
// Header file containing names of philosophers
#include "philos.h"
// Standard libraries used in program
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
// Amount of money each player starts with
#define BUY_IN      3
#define MAX_PLAYERS 14
#define MIN_PLAYERS 1
// Determines the number of faces the die being rolled has
#define NUM_FACES 6
// Controls what the pot is initially set to
#define INITIAL_POT 0

faces roll_die(int);
int players_in(int size, int arr[]);
int get_winner(int size, int arr[]);
// This program takes in a seed and number of players from user.
// Each player rolls dice, and depending on the result, gives a dollar to the player on the left,
// right, or into the pot, or does nothing. Then next player in the circle is focused. This
// continues until one player with money remains. This player is the winner.
int main(void) {
    int seed = 1, num_players, current_player = 0, pot = INITIAL_POT,
        dollar_count[MAX_PLAYERS] = { BUY_IN, BUY_IN, BUY_IN, BUY_IN, BUY_IN, BUY_IN, BUY_IN,
            BUY_IN, BUY_IN, BUY_IN, BUY_IN, BUY_IN, BUY_IN, BUY_IN };

    // Gets seed from user, returns if seed is out of range (must be positive)
    printf("Random seed: ");
    scanf("%d", &seed);
    if (seed <= 0) {
        printf("Pseudorandom seed must be non-negative (%d).\n", seed);
        return 1;
    }
    srandom((unsigned int) seed);

    // Gets number of players from user, returns if number is not between MIN_PLAYERS and
    // MAX_PLAYERS, inclusive
    printf("How many players? ");
    scanf("%d", &num_players);
    if (num_players < MIN_PLAYERS || num_players > MAX_PLAYERS) {
        printf("Number of players must be from %d to %d.\n", MIN_PLAYERS, MAX_PLAYERS);
        return 1;
    }
    // Handles edge case if number of players is 1, returns
    if (num_players == 1) {
        printf("%s wins the $%d pot with $%d left in the bank!\n", philosophers[0], INITIAL_POT,
            BUY_IN);
        return 0;
    }
    // Main game loop, repeats until main returns
    for (int round = 0;;) {
        int money = dollar_count[current_player]; //money of current player at round start
        if (money > 0) { //player has money
            printf((round++ == 0 ? "" : "\n")); //scanf prints a nwline, so first nwline is skipped
            printf("%s rolls...", philosophers[current_player]);
        }
        for (int i = 0; i < (money < BUY_IN ? money : BUY_IN); i++) { //rolls at most BUY_IN dice
            faces die_result = roll_die(NUM_FACES);
            switch (die_result) { //appropriate action is taken depending on die roll
            case LEFT:
                dollar_count[current_player]--;
                dollar_count[left(current_player, num_players)]++;
                printf(" gives $1 to %s", philosophers[left(current_player, num_players)]);
                break;
            case RIGHT:
                dollar_count[current_player]--;
                dollar_count[right(current_player, num_players)]++;
                printf(" gives $1 to %s", philosophers[right(current_player, num_players)]);
                break;
            case CENTER:
                dollar_count[current_player]--;
                pot++;
                printf(" puts $1 in the pot");
                break;
            case PASS: printf(" gets a pass"); break;
            default: break; //This should never be reached
            }
            if (players_in(num_players, dollar_count) == 1) { //exit condition checked here
                int winner = get_winner(num_players, dollar_count); //index of winner
                printf("\n%s wins the $%d pot with $%d left in the bank!\n", philosophers[winner],
                    pot, dollar_count[winner]);
                return 0; //game ends successfully
            }
        }
        current_player = right(current_player, num_players); //end condition not satisfied,
            //next player to the right focused
    }
    return 1; //this should not be reachable.
}
//
// rolls a die with sides amount of sides.
// Returns corresponding face from die[].
//
// sides: The number of sides for the die to be rolled.
//
faces roll_die(int sides) {
    return die[random() % sides];
}
//
// finds the number of players with money.
// Returns the number of players with money.
//
// size: length of array of players.
// arr: array of players to check.
//
int players_in(int size, int arr[]) {
    int count = 0;
    for (int i = 0; i < size; i++) {
        if (arr[i] > 0) {
            count++;
        }
    }
    return count;
}
//
// Finds the first player in arr with money.
// If only player with money, winner is returned.
// Returns the index of the first player with money.
//
// size: length of the array of players.
// arr: array of players to check.
//
int get_winner(int size, int arr[]) {
    for (int i = 0; i < size; i++) {
        if (arr[i] > 0) {
            return i;
        }
    }
    return 0;
}
