# 2024_uni
Year 1, 2024 university project code

## Hierarchical TSV Reporting Program

This C program reads **TSV (Tab-Separated Values)** data from standard input and generates hierarchical reports according to the user’s selection of sort columns. It was originally distributed as a skeleton by the University of Melbourne (see header comments) but has been extended with additional functionality for sorting and reporting.

---

### Table of Contents
1. [Overview](#overview)
2. [Files](#files)
3. [Compilation](#compilation)
4. [Running the Program](#running-the-program)
5. [Code Structure](#code-structure)
6. [Detailed Functionality](#detailed-functionality)
    - [Stage 1](#stage-1-reading-and-storing-data)
    - [Stage 2](#stage-2-sorting)
    - [Stage-3](#stage-3-aggregated-counts)
7. [Example Usage](#example-usage)
8. [Acknowledgments](#acknowledgments)

---

### Overview

- **Purpose**:  
  The program reads rows of TSV-formatted data (for example, Olympic data with columns like Year, Event, Gender, Country, Medal). Then it:
  1. Stores each row/field in a 2D array.
  2. Sorts the data by user-specified columns.
  3. Prints selected rows before ultimately generating counts of unique combinations of field values.

- **Input**:  
  TSV data from `stdin`, up to **1000 rows** and **30 columns** (the code primarily uses the first 5 columns in the output example).

- **Output**:  
  Multiple “stages” of information:
  1. Basic stats (number of rows/columns) and a sample row (Stage 1).
  2. Sorted rows, printing the first, middle, and last row of the sorted data (Stage 2).
  3. Counted occurrences of data combinations (Stage 3).

---

### Files

- **`ass1-skel-copy.c`**  
  This single file contains all the necessary code:
  - Reading and storing data
  - Sorting (insertion sort)
  - Generating and printing the reports.

*(The filename might differ based on your setup or assignment submission requirements.)*

---

### Compilation

To compile the code on a Unix-like system with the GCC compiler:

```bash
gcc ass1-skel-copy.c -o ass1-skel-copy
```

This will produce an executable file named `ass1-skel-copy`.

---

### Running the Program

After compilation, run the program by specifying **one or more column indices** as command-line arguments, then pipe in a TSV file from standard input. For example:

```bash
./ass1-skel-copy 4 2 < test0.tsv
```

Here:
- `4` and `2` are sort columns (meaning the data is first sorted on column 4, and ties are broken by column 2).
- `test0.tsv` is a TSV file being redirected as the program’s input.

**Note**:  
The program expects at least one sort column if you wish to produce Stage 2 and Stage 3 outputs.

---

### Code Structure

Below is a high-level view of the key functions and their roles:

1. **`main()`**  
   - Orchestrates reading the data, storing it in a 2D array, and determining how many rows and columns were read.
   - Performs Stage 1 output (print basic info and the last row).
   - Parses command-line arguments for sorting columns.
   - Calls the sorting routine and prints partial results (Stage 2).
   - Calls the functions that compute and display aggregated counts (Stage 3).

2. **`getfield(field_t dest)`**  
   - Reads a single field from standard input (up to a tab `\t` or newline `\n`).  
   - Returns whether it hit end-of-line (EOL), end-of-file (EOF), or a normal field delimiter.

3. **`mygetchar()`**  
   - A utility function that reads a single character from stdin, ignoring carriage returns (`\r`) so the program can handle data from different operating systems.

4. **`compare_rows(field_t row1[], field_t row2[], int sort_columns[], int num_sort)`**  
   - Compares two rows lexicographically based on the specified sort columns.

5. **`insertion_sort(field_t data[][MAXCOLUMNS], int num_rows, int sort_columns[], int num_sort)`**  
   - Sorts the 2D array `data` in place using insertion sort, applying `compare_rows` on the specified columns.

6. **`print_select_rows(field_t data[][MAXCOLUMNS], int num_rows, int sort_columns[], int num_sort)`**  
   - After sorting, prints the first row, a middle row, and the last row of the data to demonstrate the sorted order.

7. **`print_header_3(int sort_columns[], int num_sort, field_t data[][MAXCOLUMNS], int num_rows)`**  
   - Prepares and prints a heading for Stage 3, showing which columns are being grouped and leaving space for a “Count” column.

8. **`count_occurrences(field_t data[][MAXCOLUMNS], int num_rows, int sort_columns[], int num_sort)`**  
   - Traverses the sorted data, counting how often each distinct combination of field values appears. Prints these combinations along with their counts.

---

### Detailed Functionality

#### Stage 1: Reading and Storing Data
- The program first reads TSV fields by calling `getfield(...)` in a loop, storing each field into the `data` array.
- Each new line triggers incrementing the row counter, and the maximum column count is tracked.
- At the end of input, the total row and column counts are printed, plus a sample output of the last row read.

#### Stage 2: Sorting
- The user specifies **one or more** column indices as command-line arguments (e.g., 2, 4, etc.).
- The code uses those columns in `compare_rows()` inside an **insertion sort** (`insertion_sort()`).
- After sorting, the program prints:
  1. The first row
  2. The middle row (index = `(num_rows + 1)/2`)
  3. The last row

#### Stage 3: Aggregated Counts
- The code groups rows by the specified columns. Within each group, it counts how many consecutive rows (when sorted on the same columns) share the same combination of field values.
- A header row is printed that includes each of the sort column names followed by “Count.”
- The program prints each combination of columns plus the total count of rows in that combination.

---

### Example Usage

Assume `test0.tsv` contains:

```
Year    Event       Gender  Country Medal
2000    100m        M       AUS     Gold
2000    100m        F       USA     Silver
2004    200m        M       AUS     Bronze
...
```
*(Each field in an actual TSV file is separated by tabs.)*

```bash
./ass1-skel-copy 1 4 < test0.tsv
```
1. **Stage 1** will print the total rows/columns and sample row details.
2. **Stage 2** will sort by the specified columns (`Year` then `Country`) and print the first, middle, and last row from the sorted data.
3. **Stage 3** will produce grouped counts based on that sorting.

Output will look something like:
```
Stage 1
input tsv data has 3 rows and 5 columns
row 3 is:
   1: Year       2004
   2: Event      200m
   3: Gender     M
   4: Country    AUS
   5: Medal      Bronze

Stage 2
sorting by "Year",
   then by "Country"
row 1 is:
   ...
row 2 is:
   ...
row 3 is:
   ...

Stage 3
-----------
Year       (other columns)   Count
-----------
2000       ...
           ...
2004       ...
           ...
```
*(Exact formatting will vary based on your dataset and column choices.)*

---

### Acknowledgments

- **Skeleton Code** provided by Alistair Moffat, University of Melbourne, August 2024.  
- This modified code is submitted by *Ishnaa Sandeep Goenka (1498485)* as part of a programming assignment.  
- See the header disclaimer for statements regarding ownership, collaboration, and academic integrity.
