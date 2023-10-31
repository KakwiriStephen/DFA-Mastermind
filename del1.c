#include <stdio.h>
#include <stdlib.h>
//bool in deliverable two

typedef struct {
    int id;
    int isAccepting;
} State;

typedef struct {
    int from;
    int to;
    char symbol;
} Transition;

typedef struct {
    int numStates;
    int numTransitions;
    int startState;
    State* states;
    Transition* transitions;
} DFA;

DFA* readDfa(const char* filename) {
    FILE* file = fopen(filename, "r"); // Open the file in read mode
    if (file == NULL) {
        printf("Failed to open file\n");
        return NULL;
    }

    DFA* dfa = (DFA*)malloc(sizeof(DFA)); // Allocate memory for the DFA structure
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

    dfa->states = (State*)malloc(dfa->numStates * sizeof(State)); // Allocate memory for the states
    if (dfa->states == NULL) {
        printf("Memory allocation failed\n");
        fclose(file);
        free(dfa);
        return NULL;
    }

    for (int i = 0; i < dfa->numStates; i++) {
        if (fscanf(file, "%d,", &dfa->states[i].id) != 1) {
            printf("Error: Invalid file format\n");
            fclose(file);
            free(dfa->states);
            free(dfa);
            return NULL;
        }
    }

    if (fscanf(file, "%d\n", &dfa->startState) != 1) {
        printf("Error: Invalid file format\n");
        fclose(file);
        free(dfa->states);
        free(dfa);
        return NULL;
    }

    dfa->transitions = (Transition*)malloc(dfa->numTransitions * sizeof(Transition)); // Allocate memory for the transitions
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

    fclose(file); // Close the file
    return dfa;
}

void freeDfa(DFA* dfa) {
    if (dfa != NULL) {
        free(dfa->states); // Free the memory allocated for the states
        free(dfa->transitions); // Free the memory allocated for the transitions
        free(dfa); // Free the memory allocated for the DFA structure
    }
}

int main() {
    DFA* dfa = readDfa("dfa1.txt"); // Read the DFA from the file
    if (dfa == NULL) {
        printf("Failed to read DFA\n");
        return 1;
    }

    printf("Number of states: %d\n", dfa->numStates);
    printf("Number of transitions: %d\n", dfa->numTransitions);
    printf("Start state: %d\n", dfa->startState);

    printf("States: ");
    for (int i = 0; i < dfa->numStates; i++) {
        printf("%d ", dfa->states[i].id);
    }
    printf("\n");

    printf("Transitions:\n");
    for (int i = 0; i < dfa->numTransitions; i++) {
        printf("%d --%c--> %d\n", dfa->transitions[i].from, dfa->transitions[i].symbol, dfa->transitions[i].to);
    }

    freeDfa(dfa); // Free the memory allocated for the DFA

    return 0;
}
