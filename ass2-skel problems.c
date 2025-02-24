/* Program to perform 1D cellular automaton (CA) computations and to use 1D CA
   to solve the density classification problem.

  Skeleton program written by Artem Polyvyanyy, http://polyvyanyy.com/,
  September 2024, with the intention that it be modified by students
  to add functionality, as required by the assignment specification.
  All included code is (c) Copyright University of Melbourne, 2024.

  Authorship Declaration:

  (1) I certify that except for the code provided in the initial skeleton file,
  the program contained in this submission is completely my own individual
  work, except where explicitly noted by further comments that provide details
  otherwise. I understand that work that has been developed by another student,
  or by me in collaboration with other students, or by non-students as a result
  of request, solicitation, or payment, may not be submitted for assessment in
  this subject. I understand that submitting for assessment work developed by
  or in collaboration with other students or non-students constitutes Academic
  Misconduct, and may be penalized by mark deductions, or by other penalties
  determined via the University of Melbourne Academic Honesty Policy, as
  described at https://academicintegrity.unimelb.edu.au.

  (2) I also certify that I have not provided a copy of this work in either
  softcopy or hardcopy or any other form to any other student, and nor will I
  do so until after the marks are released. I understand that providing my work
  to other students, regardless of my intention or any undertakings made to me
  by that other student, is also Academic Misconduct.

  (3) I further understand that providing a copy of the assignment specification
  to any form of code authoring or assignment tutoring service, or drawing the
  attention of others to such services and code that may have been made
  available via such a service, may be regarded as Student General Misconduct
  (interfering with the teaching activities of the University and/or inciting
  others to commit Academic Misconduct). I understand that an allegation of
  Student General Misconduct may arise regardless of whether or not I personally
  make use of such solutions or sought benefit from such actions.

  Signed by: Ishnaa Sandeep Goenka 1498485
  Dated:     23/09/2024
*/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <string.h>

/* #DEFINE'S -----------------------------------------------------------------*/
#define SDELIM "==STAGE %d============================\n"   // stage delimiter
#define MDELIM "-------------------------------------\n"    // delimiter of -'s
#define THEEND "==THE END============================\n"    // end message

#define CRTRNC '\r'     // carriage return character
#define NBRHDS 8        // number of possible neighbourhoods

/* Helper functions stage 0 */
int read_int_input();
char* read_initial_state(int size);
void visualize_rule(int rule_number, const char* neighbourhoods[], char rule_array[], char x_or_y);
void print_state(int time_step, char* state);

/* Helper functions stage 1 */
void read_cell_position_and_start(int *cell_position, int *start_time);
void simulate_automaton(int time_steps, char *initial_state, int size, const char* neighbourhoods[], char rule_array[], char **history);
int get_rule_for_neighbourhood(int rule_number, int neighbourhood);

/* Helper functions stage 2*/
typedef char (*rule_function)(char left, char current, char right);
void apply_rule(char *initial_state, int size, char **history, int steps, rule_function rule);
char rule_184(char left, char current, char right);
char rule_232(char left, char current, char right);
void count_on_off_cells(char **history, int total_steps, int size, int *count_on, int *count_off, int cell_position, int start_time_step);
void print_classification_results(int on_count, int off_count);
void stage_2(char *final_stage_1, int size);



/* TYPE DEFINITIONS ----------------------------------------------------------*/
typedef char cells_t;            // base type to store states of cells
typedef struct state state_t;           // a cellular automaton state
typedef unsigned char rule_t[NBRHDS];   // an elementary CA update rule function

struct state {                   // a state in a CA is defined by
    cells_t*        clls;        // ... an array of cells and
    state_t*        next;        // ... a link to the next state
};

typedef struct {                 // a run of a CA consists of
    state_t*        init;        // ... the initial state and
    state_t*        curr;        // ... the current state,
} run_t;                         // implemented as a linked list of states


typedef struct {                 // an elementary CA is defined by
    unsigned char   code;        // ... a code of the update rule,
    unsigned int    size;        // ... a number of cells,
    unsigned int    elts;        // ... number of elements to store cells,
    unsigned int    time;        // ... the current time step,
    rule_t          rule;        // ... an update rule function, and
    run_t*          run;         // ... a run of state steps
} CA_t;

/* USEFUL FUNCTIONS ----------------------------------------------------------*/
int             mygetchar(void);                // getchar() that skips
                                                //    carriage returns

/* WHERE IT ALL HAPPENS ------------------------------------------------------*/
int main(int argc, char *argv[]) {
    // Message from Artem: The proposed in this skeleton file #define's,
    // typedef's, and struct's are the subsets of those from my sample solution
    // to this assignment. You can decide to use them in your program, or if
    // you find them confusing, you can remove them and implement your solution
    // from scratch. I will share my sample solution with you at the end of
    // the subject.
    // gcc -o ass2-skel ass2-skel.c
    // ./ass2-skel < test0.txt

    /* Stage 0 */
    int size = read_int_input();
    int rule_number = read_int_input();
    char* initial_state = read_initial_state(size);

    printf(SDELIM, 0);
    printf("SIZE: %d\n", size);
    printf("RULE: %d\n", rule_number);
    const char* neighbourhoods[8];
    char rule_array[8];
    visualize_rule(rule_number, neighbourhoods, rule_array, 'x');

    print_state(0, initial_state);


    /* Stage 1 */
    printf(SDELIM, 1);
    int time_step_total = read_int_input();
    int cell_position, start_time;
    read_cell_position_and_start(&cell_position, &start_time);

    // Allocate space for storing states
    char **history = (char **)malloc((time_step_total + 1) * sizeof(char *));
    simulate_automaton(time_step_total, initial_state, size, neighbourhoods, rule_array, history);

    for (int t = 0; t <= time_step_total; t++) {
        print_state(t, history[t]);  // Print the state at each time step
    }

    int count_on = 0;
    int count_off = 0;

    for (int t = start_time; t <= time_step_total; t++) {
        if (history[t][cell_position] == '*') {
            count_on++;
        } else {
            count_off++;
        }
    }

    printf(MDELIM);
    printf("#ON=%d #OFF=%d CELL#%d START@%d\n", count_on, count_off, cell_position, start_time);

    

    /* Stage 2 */
    printf(SDELIM, 2);
    char *final_stage_1 = history[10];
    stage_2(final_stage_1, size);


    printf(THEEND);

    /* Clean up of malloc */
    for (int t = 0; t <= time_step_total; t++) {
        assert(history[t] != NULL);  // Ensure the pointer is not NULL before freeing
        free(history[t]);            // Free the allocated memory
        history[t] = NULL; 
    }

    free(history);
    history = NULL;
    free(initial_state);
    initial_state = NULL;

    return EXIT_SUCCESS;        // algorithms are fun!!!
}

/* USEFUL FUNCTIONS ----------------------------------------------------------*/

// An improved version of getchar(); skips carriage return characters.
// NB: Adapted version of the mygetchar() function by Alistair Moffat
int mygetchar() {
    int c;
    while ((c=getchar())==CRTRNC);          // skip carriage return characters
    return c;
}

/* reading input line */
int read_int_input() {
    int input;
    scanf("%d", &input);
    return input;
}

/* reading initial state */
char* read_initial_state(int size) {

    // getting space to pointer initial_state
    char* initial_state = (char* )malloc((size + 1) * sizeof(char));
    assert(initial_state != NULL); // making sure of success

    // storing state in array as initial_state points to start of an array
    scanf("%s", initial_state);
    return initial_state; // returning the pointer
}

/* Converting rule int to binary number and printing out rule code */
void visualize_rule(int rule_number, const char* neighbourhoods[], char rule_array[], char x_or_y) {
    
    neighbourhoods[0] = "000";
    neighbourhoods[1] = "001";
    neighbourhoods[2] = "010";
    neighbourhoods[3] = "011";
    neighbourhoods[4] = "100";
    neighbourhoods[5] = "101";
    neighbourhoods[6] = "110";
    neighbourhoods[7] = "111";

    if (x_or_y == 'x') {
        printf(MDELIM);

    /* print neighbourhoods */
        for (int i = 0; i <= 7; i++) {
            printf(" %s", neighbourhoods[i]);
        }
        printf("\n");
    }
    
    for (int i = 0; i < 8; i++) {
        int bit = (rule_number >> i) & 1;
        if (bit == 1) {
            rule_array[i] = '*';
        } else if (bit == 0) {
            rule_array[i] = '.';
        }

        if (x_or_y == 'x') {
            printf("  %d ", bit);
        }
    }

    if (x_or_y == 'x') {
    printf("\n");
    printf(MDELIM);
    }

}

/* printing state */
void print_state(int time_step, char* state) {
    printf("%4d: %s\n", time_step, state);  // Print time_step and state
}

/* reading line 5 of input */
void read_cell_position_and_start(int *cell_position, int *start_time) {
    scanf("%d,%d", cell_position, start_time);
}

/* stage 1 helper function */
void simulate_automaton(int time_steps, char *initial_state, int size, const char* neighbourhoods[], char rule_array[], char **history) {
    char *current_state = (char *)malloc((size + 1) * sizeof(char));
    char *next_state = (char *)malloc((size + 1) * sizeof(char));
    strcpy(current_state, initial_state);

    for (int t = 0; t <= time_steps; t++) {

        // Allocate space for the state at each time step
        // history is a 2d array to store states
        history[t] = (char *)malloc((size + 1) * sizeof(char));
        strcpy(history[t], current_state);

        for (int i = 0; i < size; i++) {
            
            int left;
            if (i == 0) {
                left = current_state[size - 1];
            } else {
                left = current_state[i - 1];
            }

            int right;
            if (i == size - 1) {
                right = current_state[0];
            } else {
                right = current_state[i + 1];
            }

            int current = current_state[i];

            char neighbourhood[4];

            if (left == '*') {
                neighbourhood[0] = '1';
            } else if (left == '.') {
                neighbourhood[0] = '0';
            }
            
            if (current == '*') {
                neighbourhood[1] = '1';
            } else if (current == '.') {
                neighbourhood[1] = '0';
            }
            
            if (right == '*') {
                neighbourhood[2] = '1';
            } else if (right == '.') {
                neighbourhood[2] = '0';
            }

            neighbourhood[3] = '\0';
            
            for (int j = 0; j < 8; j++) {
                if (strcmp(neighbourhood, neighbourhoods[j]) == 0) {
                    next_state[i] = rule_array[j];
                    break;
                }
            }
        }
            next_state[size] = '\0';

            strcpy(current_state, next_state);

    }
    free(current_state);
    free(next_state);
}

/* Stage 1 helper function */
int get_rule_for_neighbourhood(int rule_number, int neighbourhood) {
    // The neighbourhood is a 3-bit number (from 0 to 7).
    // Shift the rule number right by 'neighbourhood' bits, then AND with 1 to get the result.
    return (rule_number >> neighbourhood) & 1;
}

/* stage 2 helper function */
void count_on_off_cells(char **history, int total_steps, int size, int *count_on, int *count_off, int cell_position, int start_time_step) {
    
    *count_on = 0;
    *count_off = 0;

    for (int t = start_time_step; t < total_steps + 1; t++) {
        char state = history[t][cell_position];

        if (state == '*') {
            (*count_on)++;
        } else {
            (*count_off)++;
        }
    }

    printf("#ON=%d #OFF=%d CELL#%d START@%d\n", *count_on, *count_off, cell_position, start_time_step);
}

void print_classification_results(int on_count, int off_count) {
    if (on_count > off_count) {
        printf("AT T=10: #ON/#CELLS > 1/2\n");
    } else if (on_count < off_count) {
        printf("AT T=10: #ON/#CELLS < 1/2\n");
    } else {
        printf("AT T=10: #ON/#CELLS = 1/2\n");
    }
}

void stage_2(char *final_stage_1, int size) {
    int n = (size - 2) / 2;
    int m = (size - 1) / 2;
    char **history_184 = (char **)malloc((n + 1) * sizeof(char *));
    char **history_232 = (char **)malloc((m + 1) * sizeof(char *));
    char rule_array_184[8], rule_array_232[8];

    const char* neighbourhoods[8];
    visualize_rule(184, neighbourhoods, rule_array_184, 'y');
    visualize_rule(232, neighbourhoods, rule_array_232, 'y');

    printf("RULE: 184; STEPS: %d.\n", n);
    printf(MDELIM);

    simulate_automaton(n, final_stage_1, size, neighbourhoods, rule_array_184, history_184);

    for (int t = 0; t <= n; t++) {
        printf("%4d: %s\n", t + 10, history_184[t]);
    }

    printf(MDELIM);
    printf("RULE: 232; STEPS: %d.\n", m);
    printf(MDELIM);

    simulate_automaton(m, history_184[n], size, neighbourhoods, rule_array_232, history_232);

    for (int t = 0; t <= m; t++) {
        printf("%4d: %s\n", t + 24, history_232[t]);
    }
    
    int count_on, count_off;
    printf(MDELIM);
    int cell_position, start_time_step;
    scanf("%d,%d", &cell_position, &start_time_step);
    count_on_off_cells(history_232, m + 1, size, &count_on, &count_off, cell_position, start_time_step);

    printf(MDELIM);

    printf("  10: %s\n", final_stage_1);
    print_classification_results(count_on, count_off);

    for (int t = 0; t < n + 1; t++) {
        free(history_184[t]);
    }
    for (int t = 0; t < m + 1; t++) {
        free(history_232[t]);
    }
    free(history_184);
    free(history_232);

}

/* THE END -------------------------------------------------------------------*/