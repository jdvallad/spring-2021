#include <stdint.h>
//Defines the order of the dice.
typedef enum faciem { PASS, LEFT, RIGHT, CENTER } faces;
faces die[] = { LEFT, RIGHT, CENTER, PASS, PASS, PASS };

//This is a list of all names used within the game.
char *philosophers[] = { "Immanuel Kant", "Martin Heidegger", "David Hume",
    "Georg Wilhelm Friedrich Hegel", "Arthur Schopenhauer", "Ludwig Wittgenstein",
    "Karl Wilhelm Friedrich Schlegel", "Friedrich Nietzsche", "Socrates", "John Stuart Mill",
    "Plato", "Aristotle", "Thomas Hobbes", "René Descartes" };

// Code from Assignment PDF
//
// Returns the position of the player to the left .
//
// pos : The position of the current player .
// players : The number of players in the game .
//
static inline uint8_t left(uint8_t pos, uint8_t players) {
    return ((pos + players - 1) % players);
}
// Code from Assignment PDF
//
// Returns the position of the player to the right .
//
// pos : The position of the current player .
// players : The number of players in the game .
//
static inline uint8_t right(uint8_t pos, uint8_t players) {
    return ((pos + 1) % players);
}
