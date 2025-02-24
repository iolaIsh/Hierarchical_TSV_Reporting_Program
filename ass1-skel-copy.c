/* Program to generate hierarchical reports from TSV structured data

   Skeleton program written by Alistair Moffat, ammoffat@unimelb.edu.au,
   August 2024, with the intention that it be modified by students
   to add functionality, as required by the assignment specification.
   All included code is (c) Copyright University of Melbourne, 2024

   Student Authorship Declaration:

   (1) I certify that except for the code provided in the initial skeleton
   file, the  program contained in this submission is completely my own
   individual work, except where explicitly noted by further comments that
   provide details otherwise.  I understand that work that has been developed
   by another student, or by me in collaboration with other students, or by
   non-students as a result of request, solicitation, or payment, may not be
   submitted for assessment in this subject.  I understand that submitting for
   assessment work developed by or in collaboration with other students or
   non-students constitutes Academic Misconduct, and may be penalized by mark
   deductions, or by other penalties determined via the University of
   Melbourne Academic Honesty Policy, as described at
   https://academicintegrity.unimelb.edu.au.

   (2) I also certify that I have not provided a copy of this work in either
   softcopy or hardcopy or any other form to any other student, and nor will I
   do so until after the marks are released. I understand that providing my
   work to other students, regardless of my intention or any undertakings made
   to me by that other student, is also Academic Misconduct.

   (3) I further understand that providing a copy of the assignment
   specification to any form of code authoring or assignment tutoring service,
   or drawing the attention of others to such services and code that may have
   been made available via such a service, may be regarded as Student General
   Misconduct (interfering with the teaching activities of the University
   and/or inciting others to commit Academic Misconduct).  I understand that
   an allegation of Student General Misconduct may arise regardless of whether
   or not I personally make use of such solutions or sought benefit from such
   actions.

   Signed by: Ishnaa Sandeep Goenka 1498485 
   Dated:     09/08/2024

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>
#include <assert.h>

#define CHAR_CR '\r'	     /* CR character, in PC-format text files */
#define CHAR_NL '\n'         /* newline character, ends each input line */
#define CHAR_TB '\t'         /* tab character, ends each TSV field */

#define STATUS_EOL 1         /* return value for end of line field */
#define STATUS_EOF 2         /* return value for end of file field */
#define STATUS_NML 3         /* return value if neither of previous two */

#define MAXFIELDLEN 50       /* maximum field length */
#define MAXCOLUMNS 30
#define MAXROWS 1000

/* one tsv field, stored within a fixed-length character string */
typedef char field_t[MAXFIELDLEN+1];


/**************************************************************/

int  mygetchar(void);
int  getfield(field_t dest);
int  compare_rows(field_t row1[], field_t row2[], 
                  int sort_columns[], int num_sort);
void insertion_sort(field_t data[][MAXCOLUMNS], int num_rows,
                  int sort_columns[], int num_sort);
void print_select_rows(field_t data[][MAXCOLUMNS], int num_rows,
                  int sort_columns[], int num_sort);
void print_header_3(int sort_columns[], int num_sort,
                  field_t data[][MAXCOLUMNS], int num_rows);
void count_occurrences(field_t data[][MAXCOLUMNS], int num_rows,
                  int sort_columns[], int num_sort);

/**************************************************************/

/* processing and testing
   cd ~/Desktop
   gcc ass1-skel-copy.c -o ass1-skel-copy
   ./ass1-skel-copy 4 2 < test0.tsv
   */

/* main program provides traffic control
*/
int
main(int argc, char *argv[]) {

   /* STAGE 1: */
   /* Declaring 2D array to store data */
   field_t data[MAXROWS][MAXCOLUMNS];
   int row = 0, col = 0;
   int max_columns = 0;
   int status;

   /* Read data file by using getfield() */
   while ((status = getfield(data[row][col])) != STATUS_EOF) {
      col++; /* Move to next column */

      /* Update max columns if needed */
      if (col > max_columns) {
         max_columns = col;
         }
      
      /* If end of row, move to next row */
      if (status == STATUS_EOL) {
         if (col > 0) { 
            row++;
         }
         col = 0;
      }

      /* Stop if max rows is reached */
      if (row >= MAXROWS) {
            break;
      } 
   }

   /* handle EOF at the end of the file */
   if (col == 0 && status == STATUS_EOF && row > 0) {
      row--;
   }

   /* print output */
   printf("Stage 1\n");
   int target_row = row;

   printf("input tsv data has %d rows and %d columns\n", row, max_columns);

   /* Print fields of the last row */
   if (target_row == row) {
      printf("row %d is:\n", target_row);
      printf("   1: %-10s %s\n", "Year", data[target_row][0]);
      printf("   2: %-10s %s\n", "Event", data[target_row][1]);
      printf("   3: %-10s %s\n", "Gender", data[target_row][2]);
      printf("   4: %-10s %s\n", "Country", data[target_row][3]);
      printf("   5: %-10s %s\n", "Medal", data[target_row][4]);
   }
   

   /* algorithms are fun */

   printf("\n");
   printf("\n");
   printf("\n");
   
   /* STAGE 2: */

   /* Checking if there are more than 1 arguments for sorting in stage 2 */
   if (argc > 1) {
      int sort_columns[10];
      int num_sort = 0;
      for (int i = 1; i < argc; i++) {
         sort_columns[num_sort++] = atoi(argv[i]); /* atoi is to convert string to integer */
      }

      /* Insertion sort for stage 2 */
      insertion_sort(data, row, sort_columns, num_sort);

      print_select_rows(data, row, sort_columns, num_sort);

      /* STAGE 3: */
      print_header_3(sort_columns, num_sort, data, row);
      count_occurrences(data, row, sort_columns, num_sort);
   }

	/* all done, time to go home */
   printf("\n");
	printf("ta daa!\n");
	return 0;
}

/**************************************************************/

/* read characters and build a string, stopping when a tab or newline
   as encountered, with the return value indicating what that
   terminating character was
*/
int
getfield(field_t dest) {

	int ch, nchs=0;
	int status=STATUS_NML;

	dest[0] = '\0';
	while ((ch=mygetchar())
		&& (ch != CHAR_TB) && (ch != CHAR_NL) && (ch != EOF)) {

		if (nchs < MAXFIELDLEN) {
		   /* ok to save this character */
			dest[nchs++] = ch;
			dest[nchs] = '\0';
		} else {
			/* silently discard extra characters if present */
		}
	}

	/* return status is defined by what character stopped the loop */
	if (ch == EOF) {
		status = STATUS_EOF;
	} else if (ch == CHAR_NL) {
		status = STATUS_EOL;
	} else if (ch == CHAR_TB) {
		status = STATUS_NML;
	}
	return status;
}

/**************************************************************/

/* read a single character, bypassing any CR characters encountered,
   so as to work correctly with either PC-type or Unix-type input
 */
int
mygetchar(void) {
	int c;
	while ((c=getchar()) == CHAR_CR) {
		/* empty loop body */
	}
	return c;
}

/**************************************************************/


/**
 * For stage 2, compare_rows function compares two rows of data based on the specified sort columns.
 *
 * This function compares two rows from a table, using the specified columns
 * for sorting. The comparison is lexicographical (string-based).
 *
 * row1 is the first row to be compared, represented as an array of strings.
 * row2 is the second row to be compared, represented as an array of strings.
 * sort_columns is an array of column indices used for sorting, indexed from 1.
 * num_sort are  the number of columns to use for comparison.
 * returns an integer value: 
 *         - negative if row1 < row2 
 *         - zero if row1 == row2 
 *         - positive if row1 > row2.
 */
int 
compare_rows(field_t row1[], field_t row2[], int sort_columns[], int num_sort) {
   for (int i = 0; i < num_sort; i++) {
      int col = sort_columns[i] - 1;
      int cmp = strcmp(row1[col], row2[col]);

      if (cmp != 0) {
         return cmp;
      }    
   }
   return 0;
}

/**
 * For Stage 2: insertion_sort function performs an insertion sort on rows of data 
 using specified sort columns.
 *
 * This function sorts the rows of a 2D array of data in place using the
 * insertion sort algorithm. It sorts the data based on the columns specified
 * in the `sort_columns` array.
 *
 * data is the 2D array of strings representing the table of data.
 * num_rows is the number of rows in the data array.
 * sort_columns is an Array of column indices used for sorting, indexed from 1.
 * num_sort is the number of columns to use for sorting.
 * returns void as this function does not return a value.
 */
void 
insertion_sort(field_t data[][MAXCOLUMNS], int num_rows, int sort_columns[], int num_sort) {

   for (int i = 2; i <= num_rows; i++) { /* starts from third row, as 1st row is header and 2nd row is already "sorted" */
      field_t temp[MAXCOLUMNS]; /* temp storage array for row */

      for (int k = 0; k < MAXCOLUMNS; k++) {
         strcpy(temp[k], data[i][k]); /* loop and strcpy to store row (each field) */
      }

      int j = i - 1;
      while (j >= 1 && compare_rows(data[j], temp, sort_columns, num_sort) > 0) {
         for (int k = 0; k < MAXCOLUMNS; k++) {
            strcpy(data[j + 1][k], data[j][k]);
         }
         j--;
      }
      for (int k = 0; k < MAXCOLUMNS; k++) {
         strcpy(data[j + 1][k], temp[k]);
      }

   }
}

/* Stage 2: printing sorted rows */ 
/**
 *For stage 2, print_select_rows prints selected rows from the sorted data based on sorting columns.
 *
 * This function prints the rows of data that have been sorted using the specified
 * sorting columns. It displays the first, middle, and last row of the sorted data.
 *
 * data is the 2D array of strings representing the sorted table of data.
 * num_rows is the number of rows in the sorted data array.
 * sort_columns is an array of column indices used for sorting, indexed from 1.
 * num_sort is the number of columns used for sorting.
 * returns void as this function does not return a value.
 */
void 
print_select_rows(field_t data[][MAXCOLUMNS], int num_rows, int sort_columns[], int num_sort){
   int middle_row = (num_rows + 1) / 2;
   printf("\nStage 2\n");

   /* Print sorting by section */
   printf("sorting by ");
   const char* field_names[] = { "Year", "Event", "Gender", "Country", "Medal" };


   for (int i = 0; i < num_sort; i++) {
      int col_index = sort_columns[i] - 1;
      printf("\"%s\"", field_names[col_index]);

      if (i < (num_sort - 1)) {
         printf(",\n   then by ");
      }
   }

   printf("\n");

   /* Testing loop (DELETE LATER)
   for (int i = 0; i < 30; i++) {
      for (int k = 0; k < 6; k++) {
         printf("this is a testing thing: %s\n }}}}} ", data[i][k]);
      }
   } 
   */

   /* Printing first row */
   printf("row 1 is:\n");
   for (int col = 0; col < 5; col++) {
      printf("   %d: %-10s %s\n", col + 1, field_names[col], data[1][col]);
   } 

   /* Printing middle row */
   printf("row %d is:\n", middle_row);
   for (int col = 0; col < 5; col++) {
      printf("   %d: %-10s %s\n", col + 1, field_names[col], data[middle_row][col]);
   }

   /* Printing last row */
   printf("row %d is:\n", num_rows);
   for (int col = 0; col < 5; col++) {
      printf("   %d: %-10s %s\n", col + 1, field_names[col], data[num_rows][col]);
   }

}

/**
 * For stage 3: print_header_3 prints the header for stage 3 based on user-specified columns.
 *
 * This function prints a dynamic header in stage 3 of the program, showing
 * the selected columns in the correct order, followed by the word "Count".
 * It adapts the layout based on the maximum field length.
 *
 * sort_columns is an array of column indices used for sorting, indexed from 1.
 * num_sort is the number of columns used for sorting.
 * data is the 2D array of strings representing the table of data.
 * num_rows are the number of rows in the data array.
 * returns void as this function does not return a value.
 */
void 
print_header_3(int sort_columns[], int num_sort, field_t data[][MAXCOLUMNS], int num_rows) {
   printf("\nStage 3\n");
    
   const char* field_names[] = { "Year", "Event", "Gender", "Country", "Medal" };

   int max_length = 0;

   for (int i = 0; i < num_sort; i++) {
      int col_index = sort_columns[i] - 1;

      int field_len = strlen(field_names[col_index]);
      if (field_len > max_length) {
         max_length = field_len;
      }

      for (int j = 0; j < num_rows; j++) {
         int data_len = strlen(data[j][col_index]);  
         if (data_len > max_length) {
            max_length = data_len;
         }
      }
   }

   int total_width = max_length + 4 + 5;

   for (int i = 0; i < total_width; i++) {
      printf("-");
   }
   printf("\n");

   int col_index = sort_columns[0] - 1;
   printf("%-*s", max_length, field_names[col_index]);

   for (int i = 1; i < num_sort; i++) {
      printf("\n    ");
      col_index = sort_columns[i] - 1;
      

      if (i == num_sort - 1) {
         printf("%-*s", max_length, field_names[col_index]);
         int remaining_width = total_width - (max_length * (num_sort)) - 4 * (num_sort - 1);
         printf("%*s", remaining_width, "Count");
         /* printf("%-*s  Count", max_length, field_names[col_index]); */
      } else {
         printf("%-*s", max_length, field_names[col_index]);
      }
   }

   printf("\n");

   for (int i = 0; i < (total_width); i++) {
      printf("-");
   }
   printf("\n");
}

/* Stage 3: counting occurences */
/**
 * For stage 3: count_occurrences  prints the occurrences of 
 distinct values in the sorted data.
 *
 * This function counts how many times distinct values appear 
 in the sorted data
 * and prints the results, grouped by the specified columns. 
 It prints the count
 * alongside each unique group.
 *
 * data is the 2D array of strings representing the sorted table 
 of data.
 * num_rows is the number of rows in the sorted data array.
 * sort_columns is an array of column indices used for sorting, 
 indexed from 1.
 * num_sort are the number of columns used for sorting and counting.
 * returns void as this function does not return a value.
 */
void 
count_occurrences(field_t data[][MAXCOLUMNS], int num_rows, 
                  int sort_columns[], int num_sort) {
int count = 1;
   int max_length = 0;
   field_t prev_values[MAXCOLUMNS];

   for (int i = 0; i < MAXCOLUMNS; i++) {
      prev_values[i][0] = '\0';
   }

   for (int i = 0; i < num_sort; i++) {
      int col = sort_columns[i] - 1;

      int header_len = strlen(data[0][col]);
      if (header_len > max_length) {
         max_length = header_len;
      }

      for (int j = 1; j <= num_rows; j++) {
         int data_len = strlen(data[j][col]);
         if (data_len > max_length) {
            max_length = data_len;
         }
      }
    
   for (int i = 1; i <= num_rows; i++) {
      int col_index;

      col_index = sort_columns[0] - 1;
      if (strcmp(prev_values[0], data[i][col_index]) != 0) {
         printf("%-*s\n", max_length, data[i][col_index]);
         strcpy(prev_values[0], data[i][col_index]);
      }

      col_index = sort_columns[1] - 1;
      printf("    %-*s", max_length, data[i][col_index]);

      while (i + 1 <= num_rows && compare_rows(data[i], data[i + 1], sort_columns, num_sort) == 0) {
         count++;
         i++;
      }

      printf("%*d\n", 5, count);

      count = 1;
   }
}
}
