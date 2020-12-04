#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "sudoku.h"
#include "array_tools.h"

// DIM is the dimension of the Sudoku board
#define DIM 9

// DIMBOX is the dimension of a box
static const int DIMBOX = 3;
// EMPTY is the code for an empty cell
static const int EMPTY = 0;
// PRINT holds the characters for printing out the Sudoku board
static const char PRINT[] = { '_', '1', '2', '3', '4', '5', '6', '7', '8', '9' };
// SUCCESS indicates the successful execution of a function
static const int SUCCESS = 0;
// CONSTRAINT_VIOLATED indicates that a row-, column, or box-constraint is
//   violated
static const int CONSTRAINT_VIOLATED = -1;

struct sudoku {
  // puzzle contains the start state of the puzzle
  int puzzle[DIM * DIM];
  // solution contains is equal to puzzle at the beginning and then stores
  //   all numbers filled in by the player.
  int solution[DIM * DIM];
};

// see sudoku.h for documentation
struct sudoku *sudoku_read(void) {
  struct sudoku *su = malloc(sizeof(struct sudoku));
  assert(su);
  int *pPuz = su->puzzle, *pSol = su->solution;
  for (int cnt = 0; cnt < DIM * DIM; ++cnt, ++pPuz, ++pSol) {
    char c = '\0';
    assert(scanf(" %c", &c) == 1);
    *pPuz = *pSol = (c == PRINT[EMPTY]) ? 0 : c - '0';
  }
  return su;
}

// see sudoku.h for documentation
void sudoku_destroy(struct sudoku *su) {
  assert(su);
  free(su);
}

// solution_print_separator() prints out a separator between boxes.
// produces output
static void solution_print_separator(void) {
  for (int box = 0; box < DIM / DIMBOX; ++box) {
    printf("+");
    for (int i = 0; i < 3 * DIMBOX; ++i) {
      printf("-");
    }
  }
  printf("+\n");
}

// see sudoku.h for documentation
void solution_print(const struct sudoku *su) {
  assert(su);

  for (int r = 0; r < DIM; ++r) {
    if (r % DIMBOX == 0) {
      solution_print_separator();
    }
    for (int c = 0; c < DIM; ++c) {
      if (c % DIMBOX == 0) {
        printf("|");
      }
      printf(" %c ", PRINT[su->solution[r * DIM + c]]);
    }
    printf("|\n");
  }
  solution_print_separator();
  printf("\n");
}

// see sudoku.h for documentation
void solution_reset(struct sudoku *su) {
  assert(su);
  for (int row = 0; row < DIM; ++row) {
    for (int col = 0; col < DIM; ++col) {
      su->solution[row * DIM + col] = su->puzzle[row * DIM + col];
    }
  }  
}

// see sudoku.h for documentation
bool cell_erase(struct sudoku *su, int row, int col) {
  assert(su);
  assert(0 <= row && row <= DIM - 1);
  assert(0 <= col && col <= DIM - 1);

  if (su->puzzle[row * DIM + col] != EMPTY) {
    return false;
  } else {
    su->solution[row * DIM + col] = EMPTY;
    return true;
  }
}

// row_pass(su, row, col, num) returns true if the num can 
// be filled in the spot with parameters row and col 
// without violating the row constraints.
// requires: su must be valid.
bool row_pass(const struct sudoku *su, int row,
              int col, int num) {
  assert(su);
  assert(0 <= row && row <= DIM - 1);
  assert(0 <= col && col <= DIM - 1);
  assert(1 <= num && num <= 9);
  for (int i = 0; i <= DIM - 1; i += 1) {
    if (i == col) {
      continue;
    } else if (num == su->solution[row * DIM + i]) {
      return false;
    } else {
      continue;
    }
  }
  return true;
}

// col_pass(su, row, col, num) returns true if the num can 
// be filled in the spot with parameters row and col 
// without violating the col constraints.
// requires: su must be valid.
bool col_pass(const struct sudoku *su, int row,
              int col, int num) {
  assert(su);
  assert(0 <= row && row <= DIM - 1);
  assert(0 <= col && col <= DIM - 1);
  assert(1 <= num && num <= 9);
  for (int i = 0; i <= DIM - 1; i += 1) {
    if (i == row) {
      continue;
    } else if (num == su->solution[i * DIM + col]) {
      return false;
    } else {
      continue;
    }
  }
  return true;
}

// box_pass(su, row, col, num) returns true if the num can 
// be filled in the box with parameters row and col 
// without violating the box constraints.
// requires: su must be valid.
bool box_pass(const struct sudoku *su, int row,
              int col, int num) {
  assert(su);
  assert(0 <= row && row <= DIM - 1);
  assert(0 <= col && col <= DIM - 1);
  assert(1 <= num && num <= 9);
  int inner_row = 0;
  int inner_col = 0;
  inner_row = row % DIMBOX;
  inner_col = col % DIMBOX;
  if ((inner_row == 0) && (inner_col == 0)) {
    if ((su->solution[row * DIM + col + 1] == num) ||
        (su->solution[row * DIM + col + 2] == num) ||
        (su->solution[(row + 1) * DIM + col] == num) ||
        (su->solution[(row + 1) * DIM + col + 1] == num) ||
        (su->solution[(row + 1) * DIM + col + 2] == num) ||
        (su->solution[(row + 2) * DIM + col] == num) ||
        (su->solution[(row + 2) * DIM + col + 1] == num) ||
        (su->solution[(row + 2) * DIM + col + 2] == num)) {
      return false;
    } else {
      return true;
    }
  } else if ((inner_row == 0) && (inner_col == 1)) {
    if ((su->solution[row * DIM + col + 1] == num) ||
        (su->solution[row * DIM + col - 1] == num) ||
        (su->solution[(row + 1) * DIM + col] == num) ||
        (su->solution[(row + 1) * DIM + col + 1] == num) ||
        (su->solution[(row + 1) * DIM + col - 1] == num) ||
        (su->solution[(row + 2) * DIM + col] == num) ||
        (su->solution[(row + 2) * DIM + col + 1] == num) ||
        (su->solution[(row + 2) * DIM + col - 1] == num)) {
      return false;
    } else {
      return true;
    }
  } else if ((inner_row == 0) && (inner_col == 2)) {
    if ((su->solution[row * DIM + col - 1] == num) ||
        (su->solution[row * DIM + col - 2] == num) ||
        (su->solution[(row + 1) * DIM + col] == num) ||
        (su->solution[(row + 1) * DIM + col - 1] == num) ||
        (su->solution[(row + 1) * DIM + col - 2] == num) ||
        (su->solution[(row + 2) * DIM + col] == num) ||
        (su->solution[(row + 2) * DIM + col - 1] == num) ||
        (su->solution[(row + 2) * DIM + col - 2] == num)) {
      return false;
    } else {
      return true;
    } // above is for first row boxes.
  } else if ((inner_row == 1) && (inner_col == 0)) {
    if ((su->solution[row * DIM + col + 1] == num) ||
        (su->solution[row * DIM + col + 2] == num) ||
        (su->solution[(row - 1) * DIM + col] == num) ||
        (su->solution[(row - 1) * DIM + col + 1] == num) ||
        (su->solution[(row - 1) * DIM + col + 2] == num) ||
        (su->solution[(row + 1) * DIM + col] == num) ||
        (su->solution[(row + 1) * DIM + col + 1] == num) ||
        (su->solution[(row + 1) * DIM + col + 2] == num)) {
      return false;
    } else {
      return true;
    }
  } else if ((inner_row == 1) && (inner_col == 1)) {
    if ((su->solution[row * DIM + col + 1] == num) ||
        (su->solution[row * DIM + col - 1] == num) ||
        (su->solution[(row + 1) * DIM + col] == num) ||
        (su->solution[(row + 1) * DIM + col + 1] == num) ||
        (su->solution[(row + 1) * DIM + col - 1] == num) ||
        (su->solution[(row - 1) * DIM + col] == num) ||
        (su->solution[(row - 1) * DIM + col + 1] == num) ||
        (su->solution[(row - 1) * DIM + col - 1] == num)) {
      return false;
    } else {
      return true;
    }
  } else if ((inner_row == 1) && (inner_col == 2)) {
    if ((su->solution[row * DIM + col - 1] == num) ||
        (su->solution[row * DIM + col - 2] == num) ||
        (su->solution[(row + 1) * DIM + col] == num) ||
        (su->solution[(row + 1) * DIM + col - 1] == num) ||
        (su->solution[(row + 1) * DIM + col - 2] == num) ||
        (su->solution[(row - 1) * DIM + col] == num) ||
        (su->solution[(row - 1) * DIM + col - 1] == num) ||
        (su->solution[(row - 1) * DIM + col - 2] == num)) {
      return false;
    } else {
      return true;
    }// above is for second row of box
  }  else if ((inner_row == 2) && (inner_col == 0)) {
    if ((su->solution[row * DIM + col + 1] == num) ||
        (su->solution[row * DIM + col + 2] == num) ||
        (su->solution[(row - 1) * DIM + col] == num) ||
        (su->solution[(row - 1) * DIM + col + 1] == num) ||
        (su->solution[(row - 1) * DIM + col + 2] == num) ||
        (su->solution[(row - 2) * DIM + col] == num) ||
        (su->solution[(row - 2) * DIM + col + 1] == num) ||
        (su->solution[(row - 2) * DIM + col + 2] == num)) {
      return false;
    } else {
      return true;
    }
  } else if ((inner_row == 2) && (inner_col == 1)) {
    if ((su->solution[row * DIM + col + 1] == num) ||
        (su->solution[row * DIM + col - 1] == num) ||
        (su->solution[(row - 2) * DIM + col] == num) ||
        (su->solution[(row - 2) * DIM + col + 1] == num) ||
        (su->solution[(row - 2) * DIM + col - 1] == num) ||
        (su->solution[(row - 1) * DIM + col] == num) ||
        (su->solution[(row - 1) * DIM + col + 1] == num) ||
        (su->solution[(row - 1) * DIM + col - 1] == num)) {
      return false;
    } else {
      return true;
    }
  } else {
    if ((su->solution[row * DIM + col - 1] == num) ||
        (su->solution[row * DIM + col - 2] == num) ||
        (su->solution[(row - 2) * DIM + col] == num) ||
        (su->solution[(row - 2) * DIM + col - 1] == num) ||
        (su->solution[(row - 2) * DIM + col - 2] == num) ||
        (su->solution[(row - 1) * DIM + col] == num) ||
        (su->solution[(row - 1) * DIM + col - 1] == num) ||
        (su->solution[(row - 1) * DIM + col - 2] == num)) {
      return false;
    } else {
      return true;
    }
  }
}

// pass(su, row, col, num) returns true if the num can 
// be filled in the spot with parameters row and col 
// without violating the row, col, and box constraints.
// requires: su must be valid.
bool pass(const struct sudoku *su, int row,
          int col, int num) {
  if ((row_pass(su, row, col, num)) &&
      (col_pass(su, row, col, num)) && 
      (box_pass(su, row, col, num))) {
    return true;
  } else {
    return false;
  }
}


// see sudoku.h for documentation
bool cell_fill(struct sudoku *su, int row,
               int col, int num) {
  assert(su);
  assert(0 <= row && row <= DIM - 1);
  assert(0 <= col && col <= DIM - 1);
  assert(1 <= num && num <= 9);
  if (su->solution[row * DIM + col] != EMPTY) {
    return false;
  } else if (pass(su, row, col, num)) {
    su->solution[row * DIM + col] = num;
    return true;
  } else {
    return false;
  }
}

// one_to_nine(a, b, c, d, e, f, g, h, i) returns true
// the a to i cover integer from 1 to 9 (both includsive), 
// order does not matter. return false otherwise.
bool one_to_nine (int a, int b, int c, int d, int e,
                  int f, int g, int h, int i) {
  for (int j = 1; j <= 9; j += 1) {
    if ((a == j) || (b == j) || (c == j) || (d == j) ||
        (e == j) || (f == j) || (g == j) || (h == j) ||
        (i == j)) {
      continue;
    } else {
      return false;
    }
  }
  return true;
}

// row_solved(su) returns true if all rows are 
// solved, false otherwise
// requires: su must be valid.
bool row_solved(const struct sudoku *su) {
  assert(su);
  for (int row = 0; row < DIM; row += 1) {
    if (one_to_nine(su->solution[row * DIM + 0],
                    su->solution[row * DIM + 1],
                    su->solution[row * DIM + 2],
                    su->solution[row * DIM + 3],
                    su->solution[row * DIM + 4],
                    su->solution[row * DIM + 5],
                    su->solution[row * DIM + 6],
                    su->solution[row * DIM + 7],
                    su->solution[row * DIM + 8])) {
      continue;
    } else {
      return false;
    }
  }
  return true;
}

// col_solved(su) returns true if all columns are 
// solved, false otherwise
// requires: su must be valid.
bool col_solved(const struct sudoku *su) {
  assert(su);
  for (int col = 0; col < DIM; col += 1) {
    if (one_to_nine(su->solution[0 * DIM + col],
                    su->solution[1 * DIM + col],
                    su->solution[2 * DIM + col],
                    su->solution[3 * DIM + col],
                    su->solution[4 * DIM + col],
                    su->solution[5 * DIM + col],
                    su->solution[6 * DIM + col],
                    su->solution[7 * DIM + col],
                    su->solution[8 * DIM + col])) {
      continue;
    } else {
      return false;
    }
  }
  return true;
}

// box1_solved(su) returns true if all top boxes are 
// solved, false otherwise
// requires: su must be valid.
bool box1_solved(const struct sudoku *su) {
  assert(su);
  for (int box = 0; box < 3; box += 1) {
    if (one_to_nine(su->solution[0 * DIM + 3 * box],
                    su->solution[1 * DIM + 3 * box],
                    su->solution[2 * DIM + 3 * box],
                    su->solution[(0 * DIM + 1) + 3 * box],
                    su->solution[(0 * DIM + 2) + 3 * box],
                    su->solution[(1 * DIM + 1) + 3 * box],
                    su->solution[(1 * DIM + 2) + 3 * box],
                    su->solution[(2 * DIM + 1) + 3 * box],
                    su->solution[(2 * DIM + 2) + 3 * box])) {
      continue;
    } else {
      return false;
    }
  }
  return true;
}

// box2_solved(su) returns true if all middle boxes are 
// solved, false otherwise
// requires: su must be valid.
bool box2_solved(const struct sudoku *su) {
  assert(su);
  for (int box = 0; box < 3; box += 1) {
    if (one_to_nine(su->solution[3 * DIM + 3 * box],
                    su->solution[4 * DIM + 3 * box],
                    su->solution[5 * DIM + 3 * box],
                    su->solution[(3 * DIM + 1) + 3 * box],
                    su->solution[(4 * DIM + 2) + 3 * box],
                    su->solution[(5 * DIM + 1) + 3 * box],
                    su->solution[(3 * DIM + 2) + 3 * box],
                    su->solution[(4 * DIM + 1) + 3 * box],
                    su->solution[(5 * DIM + 2) + 3 * box])) {
      continue;
    } else {
      return false;
    }
  }
  return true;
}

// box3_solved(su) returns true if all bottom boxes are 
// solved, false otherwise
// requires: su must be valid.
bool box3_solved(const struct sudoku *su) {
  assert(su);
  for (int box = 0; box < 3; box += 1) {
    if (one_to_nine(su->solution[6 * DIM + 3 * box],
                    su->solution[7 * DIM + 3 * box],
                    su->solution[8 * DIM + 3 * box],
                    su->solution[(6 * DIM + 1) + 3 * box],
                    su->solution[(7 * DIM + 2) + 3 * box],
                    su->solution[(8 * DIM + 1) + 3 * box],
                    su->solution[(6 * DIM + 2) + 3 * box],
                    su->solution[(7 * DIM + 1) + 3 * box],
                    su->solution[(8 * DIM + 2) + 3 * box])) {
      continue;
    } else {
      return false;
    }
  }
  return true;
}

// box_solved(su) returns true if all boxes are solved,
// false otherwise
// requires: su must be valid.
bool box_solved(const struct sudoku *su) {
  if (box1_solved(su) && box2_solved(su) 
      && box3_solved(su)) {
    return true;
  } else {
    return false;
  }
}


// see sudoku.h for documentation
bool puzzle_solved(const struct sudoku *su) {
  assert(su);
  if (row_solved(su) && col_solved(su) 
      && box_solved(su)) {
    return true;
  } else {
    return false;
  }
}

// see sudoku.h for documentation
int cell_choices(const struct sudoku *su, int row, int col, int choices[]) {
  assert(su);
  assert(choices);
  assert(0 <= row && row <= DIM - 1);
  assert(0 <= col && col <= DIM - 1);
  int index = 0;
  int result = 0;
  for (int i = 1; i <= 9; i += 1) {
    if ((su->solution[row * DIM + col] == EMPTY) && 
        (pass(su, row, col, i))) {
      choices[index] = i;
      index += 1;
      result += 1;
    } else {
      continue;
    }
  }
  return result;
}

// see sudoku.h for documentation
bool cell_hint(const struct sudoku *su, int *row, int *col) {
  assert(su);
  assert(row);
  assert(col);
  int choices[DIM] = {0};
  for (int i = 0; i <= DIM - 1; i += 1) {
    for (int j = 0; j <= DIM - 1; j += 1) {
      if ((su->solution[i * DIM + j] == EMPTY) && 
          (cell_choices(su, i, j, choices) == 1)) {
        *row = i;
        *col = j;
        return true;
      } else {
        continue;
      }
    }
  }
  return false;
}

// see sudoku.h for documentation
bool sudoku_solve(struct sudoku *su) {
  if (puzzle_solved(su)) {
    return true;
  } else {
    int empty = 0;
    int choices[DIM] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    int choices_temp[DIM] = {0};
    int choices_num = 9;
    int choices_num_temp = 0;
    int row = 0;
    int col = 0; 
    for (int i = 0; i <= DIM - 1; i += 1) {
      for (int j = 0; j <= DIM - 1; j += 1) {
        if (su->solution[i * DIM + j] == EMPTY) {
          empty += 1;
          choices_num_temp = cell_choices (su, i,
                                           j, choices_temp);
          if (choices_num_temp == 0) {
            return false;
          } else if (choices_num_temp < choices_num) {
            choices_num = choices_num_temp;
            row = i;
            col = j;
            for (int i = 0; i < choices_num; i += 1) {
              choices[i] = choices_temp[i];
            }
          } else {
            continue;
          }
        } else {
          continue;
        }
      }
    }
    if (empty == 0) {
      return false;
    } else {
      for (int i = 0; i < choices_num; i += 1) {
        cell_fill(su, row, col, choices[i]);
        if (sudoku_solve(su)) {
          return true;
        } else {
          cell_erase(su, row, col);
          continue;
        }
      }
      return false;
    }
  }
}
