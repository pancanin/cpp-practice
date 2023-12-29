#include <iostream>
#include <memory>

// Used only for testing
#include <assert.h>
#include <sstream>

typedef unsigned int MatrixValue;
typedef unsigned int BitSet;

// Note: Many functions accept 'n' as the size of the sudoku board.
// This is the number of rows and columns the square sudoku board has.
// So a 3x3 sudoku has 9 rows and 9 columns.
// This code does not focus on testing the input data too strictly.

size_t readN(std::istream& in);
MatrixValue** reserveMatrix(size_t n);
void releaseMatrix(MatrixValue** matrix, size_t n);

/// <summary>
/// Parses values from input stream and assigns them to the matrix.
/// Fills first row first, then second, etc. So the input values have to be passed in in the same order (row-major).
/// Does not do validation whether the input data is in the range 1...n (inclusive)
/// </summary>
/// <param name="in">input stream. Could be standard in, file or network.</param>
/// <param name="board">The already allocated matrix of integer values</param>
/// <param name="n">The number of rows/cols as the matrix is square.</param>
void fillMatrix(std::istream& in, MatrixValue** matrix, size_t n);
void testFillMatrix2x2();

size_t countOneBits(unsigned int bits);
void testCountOneBits();

bool checkBoardRowOrColMajor(MatrixValue** board, size_t n, bool rowMajor);
bool isValidSudokuSolution(MatrixValue** board, size_t n);

void testIsValidSudokuTrue();
void testIsValidSudokuTrue2();
void testIsValidSudokuInvalidRow();
void testIsValidSudokuInvalidCol();
void testIsValidSudokuInvalidCol2();

// Ideas
// 1. Think about the data types for the matrix values.
// 2. Add checks for the input of N?
// 3. Add checks if 'new' operator fails.
int main()
{
  testCountOneBits();
  testFillMatrix2x2();
  testIsValidSudokuTrue();
  testIsValidSudokuTrue2();
  testIsValidSudokuInvalidRow();
  testIsValidSudokuInvalidCol();
  
  size_t n = readN(std::cin);
  n *= n; // a 2x2 sudoku has 2^2 rows and cols
  MatrixValue** m = reserveMatrix(n);

  if (!m) {
    std::cout << "Failed to allocate matrix\n";
    return -1;
  }

  fillMatrix(std::cin, m, n);

  std::cout << (isValidSudokuSolution(m, n) ? "Sudoku is valid." : "Sudoku is invalid") << "\n";

  releaseMatrix(m, n);
  return 0;
}

size_t readN(std::istream& in) {
  unsigned int n = 0;
  in >> n;
  return n;
}

MatrixValue** reserveMatrix(size_t n) {
  MatrixValue** matrix = new (std::nothrow) MatrixValue * [n];

  if (!matrix) {
    return nullptr;
  }

  for (size_t i = 0; i < n; i++) {
    matrix[i] = new (std::nothrow) MatrixValue[n];
    if (!matrix[i]) {
      return nullptr;
    }
  }

  return matrix;
}

void releaseMatrix(MatrixValue** matrix, size_t n) {
  for (size_t i = 0; i < n; i++) {
    delete[] matrix[i];
  }
  delete[] matrix;
}

/// <summary>
/// Parses values from input stream and assigns them to the matrix.
/// Fills first row first, then second, etc. So the input values have to be passed in in the same order (row-major).
/// Does not do validation whether the input data is in the range 1...n (inclusive)
/// </summary>
/// <param name="in">input stream. Could be standard in, file or network.</param>
/// <param name="board">The already allocated matrix of integer values</param>
/// <param name="n">The number of rows/cols as the matrix is square.</param>
void fillMatrix(std::istream& in, MatrixValue** matrix, size_t n) {
  MatrixValue value = 0;
  for (size_t i = 0; i < n; i++) {
    for (size_t j = 0; j < n; j++) {
      in >> value;
      matrix[i][j] = value;
    }
  }
}

void testFillMatrix2x2();

size_t countOneBits(BitSet bits) {
  const size_t bitsCount = sizeof(BitSet) * 8;
  size_t ones = 0;
  for (size_t i = 0; i < bitsCount; i++) {
    BitSet currentBit = bits & (1u << i);
    if (currentBit != 0) {
      ones++;
    }
  }

  return ones;
}

void testCountOneBits();

bool checkBoardRowOrColMajor(MatrixValue** board, size_t n, bool rowMajor) {
  for (size_t i = 0; i < n; i++) {
    BitSet bits = 0; // used to record which numbers are on the row/col in separate slots.
    for (size_t j = 0; j < n; j++) {
      MatrixValue val = rowMajor ? board[i][j] : board[j][i];
      if (val > n || val == 0) { return false; } // Checks the values are in sudoku range

      bits |= (1u << val); // Each number will reserve an unique bit-slot in 'bits'
    }

    // The unique numbers count has to match the number of rows/cols.
    if (countOneBits(bits) != n) {
      return false;
    }
  }

  return true;
}

bool isValidSudokuSolution(MatrixValue** board, size_t n) {
  return checkBoardRowOrColMajor(board, n, true) && checkBoardRowOrColMajor(board, n, false);
}

void testFillMatrix2x2() {
  const size_t n = 4;
  std::stringstream ss("1 2 3 4 3 4 1 2 2 1 4 3 4 3 2 1");
  MatrixValue** m = reserveMatrix(n);
  fillMatrix(ss, m, n);

  assert(m[0][0] == 1);
  assert(m[0][1] == 2);
  assert(m[0][2] == 3);
  assert(m[0][3] == 4);
  assert(m[1][0] == 3);
  assert(m[1][1] == 4);
  assert(m[1][2] == 1);
  assert(m[1][3] == 2);
  assert(m[2][0] == 2);
  assert(m[2][1] == 1);
  assert(m[2][2] == 4);
  assert(m[2][3] == 3);
  assert(m[3][0] == 4);
  assert(m[3][1] == 3);
  assert(m[3][2] == 2);
  assert(m[3][3] == 1);
}

void testCountOneBits() {
  assert(countOneBits(1022) == 9);
  assert(countOneBits(51) == 4);
  assert(countOneBits(1) == 1);
  assert(countOneBits(21) == 3);
}

void testIsValidSudokuTrue() {
  const size_t n = 4;
  std::stringstream ss("1 2 3 4 3 4 1 2 2 1 4 3 4 3 2 1");
  MatrixValue** m = reserveMatrix(n);
  fillMatrix(ss, m, n);

  assert(isValidSudokuSolution(m, n));
}

void testIsValidSudokuInvalidRow() {
  const size_t n = 4;
  //                          *
  std::stringstream ss("1 2 3 1 3 4 1 2 2 1 4 3 4 3 2 1");
  MatrixValue** m = reserveMatrix(n);
  fillMatrix(ss, m, n);

  assert(!isValidSudokuSolution(m, n));
}

void testIsValidSudokuInvalidCol() {
  const size_t n = 4;
  std::stringstream ss("1 2 3 4 1 2 3 4 1 2 3 4 1 2 3 4");
  MatrixValue** m = reserveMatrix(n);
  fillMatrix(ss, m, n);

  assert(!isValidSudokuSolution(m, n));
}

void testIsValidSudokuInvalidCol2() {
  const size_t n = 4;
  std::stringstream ss("1 2 3 4 4 3 2 1 2 1 4 3 4 3 2 1");
  MatrixValue** m = reserveMatrix(n);
  fillMatrix(ss, m, n);

  assert(!isValidSudokuSolution(m, n));
}

void testIsValidSudokuTrue2() {
  const size_t n = 4;
  std::stringstream ss("1 4 3 2 2 3 1 4 3 2 4 1 4 1 2 3");
  MatrixValue** m = reserveMatrix(n);
  fillMatrix(ss, m, n);

  assert(isValidSudokuSolution(m, n));
}