
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct {
    int from;
    char symbol;
    int to;
} Transition;

typedef struct {
    int id;
    int isAccepting;
} State;

typedef struct {
    int numStates;
    int numTransitions;
    State* states;
    Transition* transitions;
    int startState;
} DFA;

// Function to read DFA from file
DFA* readDfa(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Failed to open file\n");
        return NULL;
    }

    DFA* dfa = (DFA*)malloc(sizeof(DFA));
    if (dfa == NULL) {
        printf("Memory allocation failed\n");
        fclose(file);
        return NULL;
    }

    if (fscanf(file, "%d,%d\n", &dfa->numStates, &dfa->numTransitions) != 2) {
        printf("Error: Invalid file format\n");
        fclose(file);
        free(dfa);
        return NULL;
    }

    dfa->states = (State*)malloc(dfa->numStates * sizeof(State));
    if (dfa->states == NULL) {
        printf("Memory allocation failed\n");
        fclose(file);
        free(dfa);
        return NULL;
    }

    for (int i = 0; i < dfa->numStates; i++) {
        if (fscanf(file, "%d,%d\n", &dfa->states[i].id, &dfa->states[i].isAccepting) != 2) {
            printf("Error: Invalid file format\n");
            fclose(file);
            free(dfa->states);
            free(dfa);
            return NULL;
        }
    }

    dfa->transitions = (Transition*)malloc(dfa->numTransitions * sizeof(Transition));
    if (dfa->transitions == NULL) {
        printf("Memory allocation failed\n");
        fclose(file);
        free(dfa->states);
        free(dfa);
        return NULL;
    }

    for (int i = 0; i < dfa->numTransitions; i++) {
        if (fscanf(file, "%d,%d,%c\n", &dfa->transitions[i].from, &dfa->transitions[i].to, &dfa->transitions[i].symbol) != 3) {
            printf("Error: Invalid file format\n");
            fclose(file);
            free(dfa->states);
            free(dfa->transitions);
            free(dfa);
            return NULL;
        }
    }

    if (fscanf(file, "%d\n", &dfa->startState) != 1) {
        printf("Error: Invalid file format\n");
        fclose(file);
        free(dfa->states);
        free(dfa->transitions);
        free(dfa);
        return NULL;
    }

    fclose(file);
    return dfa;
}

// Function to simulate DFA on input string
bool simulateDfa(DFA* dfa, const char* inputString) {
    int currentState = dfa->startState;

    for (int i = 0; inputString[i] != '\0'; i++) {
        char currentSymbol = inputString[i];
        bool transitionFound = false;

        for (int j = 0; j < dfa->numTransitions; j++) {
            if (dfa->transitions[j].from == currentState && dfa->transitions[j].symbol == currentSymbol) {
                currentState = dfa->transitions[j].to;
                transitionFound = true;
                break;
            }
        }

        if (!transitionFound) {
            return false;
        }
    }

    return dfa->states[currentState].isAccepting == 1;
}

int main() {
    DFA* dfa = readDfa("dfa1.txt");
    if (dfa == NULL) {
        printf("Failed to read DFA\n");
        return 1;
    }

    bool accepted = simulateDfa(dfa, "ababab");
    if (accepted) {
        printf("True.\n");
    } else {
        printf("False\n");
    }

    free(dfa->states);
    free(dfa->transitions);
    free(dfa);

    return 0;
}
