# Cross-Database Query Problem

External merge-sort join operation on two CSV files with limited memory. Performs SQL-like JOIN matching specified columns, handling datasets larger than available RAM by sorting to disk and merging sorted results.

## Problem Statement

Joining large database exports exceeds available memory. This program solves it using external sorting: partition files into memory-sized chunks, sort each, multi-way merge partitions, then join on matching columns. Enables gigabyte-scale joins with megabyte-level RAM.

**Real-world scenario:** Combine employee records + email records from two database servers, matching on employee ID + name + email. Original files don't fit in memory; program sorts both files on those columns, then outputs matching rows.

## How It Works

### Three-Phase Algorithm

```
Input Files
    ↓
Phase 1: External Sort Both Files
├─ Read M lines into memory
├─ Sort by specified columns (L1 for file1, L2 for file2)
├─ Write sorted chunks to P temporary partition files each
    ↓
Phase 2: Multi-way Merge Partitions
├─ Merge P file1 partitions → sorted0.txt
└─ Merge P file2 partitions → sorted1.txt
    ↓
Phase 3: Join Sorted Files
├─ Two-pointer scan (O(n+m) time)
├─ Compare rows on L1 vs L2 column values
├─ Output matching rows (join columns + remaining fields)
    ↓
Output File
```

### Phase 1: Partition & In-Memory Sort

- Read up to M lines from input file into RAM
- Sort using qsort_r by columns specified in L1/L2
- Write sorted chunk to temporary file (f0.txt, f1.txt, etc.)
- Repeat until entire file processed
- Result: P partitions per file, each locally sorted

### Phase 2: Multi-Way Merge

- Merge P file1 partitions using validation matrix tracking remaining lines
- Iteratively find smallest line across all P partitions, write to sorted0.txt
- Repeat for P file2 partitions → sorted1.txt
- Result: Two fully sorted files

### Phase 3: Join Operation

- Open sorted0.txt and sorted1.txt
- Read one line from each
- Compare values at L1 columns (file1) vs L2 columns (file2)
  - If match: output joined row (matched columns + remaining fields from both)
  - If L1 row < L2 row: advance file1
  - If L1 row > L2 row: advance file2
- Repeat until either file ends
- Result: Single output file with matched rows

## Building

```bash
make all      # Clean and compile (produces mergejoin executable)
make clean    # Remove object files and intermediate outputs
```

## Usage

```bash
./mergejoin P M L1 L2 file1 file2 output
```

**Parameters:**
- `P`: Number of partitions (integer, e.g., 3)
- `M`: Lines per in-memory sort buffer (integer, e.g., 5)
- `L1`: Comma-separated column indices for file1 (e.g., "0,1,4")
- `L2`: Comma-separated column indices for file2 (e.g., "4,0,1")
- `file1`: First input CSV file
- `file2`: Second input CSV file
- `output`: Output file path

### Example: Baseline Test

```bash
./mergejoin 3 5 0,1,4 4,0,1 inputs/file1.txt inputs/file2.txt outputs/result.txt
```

Joins file1 on columns 0,1,4 with file2 on columns 4,0,1 (in that order).

### Example: Large Dataset Test

```bash
./mergejoin 3 5 1,3 4,8 inputs/pythonfile1.txt inputs/pythonfile2.txt outputs/pythonresult.txt
```

Handles ~12MB files with person records matching on name+date vs person_id+email.

## Input Format

CSV files with no header row. Fields separated by commas, records separated by newlines.

```
field0,field1,field2,...,fieldN
field0,field1,field2,...,fieldN
```

**Example (file1.txt):**
```
0,V,RW,K,6
Y,8A,6,UM,S
VS,WH,W3,6,0
```

**Example (file2.txt):**
```
JD,OB,W,XK,H6,KG
UE,CV,49,KL,FH,NQ
```

**Constraints:**
- Column indices are zero-based
- Maximum field length: 61 characters
- All columns must exist in input files

## Output Format

CSV with matched rows. Columns appear in order: matched columns (from L1) + unmatched file1 columns + unmatched file2 columns.

**Example output** (joining file1[0,1,4] with file2[4,0,1]):
```
H6,JD,OB,O,RV,W,XK,KG
VS,WH,0,W3,6,6M,JI,NZ
```

Each line is a complete match: the matched values first, then remaining fields from each file.

## Running Tests

```bash
make run      # Small dataset (10 rows each)
make run0     # Large dataset (~12MB, ~1M rows)
make run2     # Secondary test set (different partitions/buffer)
make run3     # Portuguese language dataset test
```

## Memory Checking

Verify no memory leaks using Valgrind:

```bash
make val      # Main test with Valgrind
make val0     # Large dataset with Valgrind
make val2     # Secondary test with Valgrind
make val3     # Portuguese test with Valgrind
make full     # Main test with full leak detection
make full2    # Secondary test with full leak detection
```

## Architecture

### Source Files

| File | Purpose |
|------|---------|
| `src/main.c` | Entry point; orchestrates three phases |
| `src/sort.c` | External sort, qsort_r comparator, multi-way merge via validation matrix |
| `src/files.c` | File I/O (open, close, rename); join logic with two-pointer merge |
| `src/list.c` | List structure for column indices; string-to-int parsing |
| `src/strings.c` | Matrix utilities; CSV parsing (field extraction via strtok) |

### Key Data Structures

**List** (`list.c`): Wraps int array of column indices from parsed command-line args. Passed to qsort_r as comparator context.

**Matrix** (`strings.c`): 3D char array (memo) for buffering rows during sort and merge. SLOTS=61 max field length. Dimensions: M lines × K fields.

**Validation Matrix** (`sort.c`): Tracks remaining blocks during multi-way merge. Rows = merge iterations, columns = P partitions. Each cell = lines still to read from that partition.

### Join Algorithm (mergeFiles in files.c)

Two-pointer technique:
1. Read one line from each sorted file into matrix structures
2. Compare values at L1 columns (file1) vs L2 columns (file2) using strcmp
3. Based on comparison result:
   - Equal: write joined row (mark matched columns with "-", output unmatched), advance both
   - File1 < File2: advance file1
   - File1 > File2: advance file2
4. Repeat until either file ends

## Temporary Files

During execution, creates `f0.txt` through `f(2P-1).txt` (2P files, one per partition). Automatically deleted at program exit.

Also creates intermediate sorted files:
- `sorted0.txt`: Fully sorted file1
- `sorted1.txt`: Fully sorted file2

These are also cleaned up after join completes.

## Common Issues

**Argument count error:** Requires exactly 7 arguments. Verify all provided.

**Column index out of bounds:** Indices are zero-based. Check input files have required columns.

**File not found:** Verify input file paths. Use absolute paths if needed.

**Memory errors with large files:** Increase M (lines per buffer). If many temp files created, indicates poor partitioning.

---

## Code Quality Analysis (Refactoring Opportunities)

This section documents areas for potential improvement. This code is typical for academic/prototype projects; production use would address these.

### Critical Issues

**Memory leak in mergeFiles early returns** (`src/files.c:95-102`)

When input files are empty, early returns don't free allocated matrices:
```c
// Line 95-102: Early returns leak matrix1, matrix2, line
if (feof(input1)) {
    return;  // matrix1 not freed
}
// ... later ...
if (feof(input2)) {
    return;  // matrix2 not freed
}
```

**Impact:** Minor (only on empty input), but present on all valid inputs.

**Fix:** Move early EOF checks before matrix allocation, or use goto cleanup pattern.

---

**Missing malloc/realloc error handling**

malloc/realloc calls never checked for NULL. If allocation fails, program crashes silently.

Example (`src/sort.c:80`):
```c
char **firstString = malloc(sizeof(char*) * K);  // No NULL check
```

Example (`src/sort.c:159`):
```c
firstString[k] = realloc(firstString[k], length + 1);  // No NULL check
```

**Impact:** Program fails ungracefully on OOM. Should log error and exit cleanly.

**Fix:** Add `if (!ptr) { perror("malloc"); exit(1); }` after allocations.

---

**Buffer size bug in list.c:30**

Wrong sizeof used in malloc:
```c
int *intList = malloc(sizeof(char *) * length);  // sizeof(char*) = 8 bytes
```

Should be:
```c
int *intList = malloc(sizeof(int) * length);  // sizeof(int) = 4 bytes
```

**Impact:** Allocates double needed memory on 64-bit systems. Works but wastes space.

**Fix:** Change to `sizeof(int)`.

---

### High Priority Issues

**Magic numbers throughout codebase**

- `SLOTS=61`: Max field length hardcoded in strings.h:7
- Buffer size 16: Used for filenames in multiple places (files.c:38,40,224)
- Buffer size 13: Fixed output filename lengths (files.c:40)

**Examples:**
```c
// files.c:38 - No comment explaining 16
char *oldname = malloc(sizeof(char)*16);
// sort.c:274 - sizeof magic number
qsort_r(matrix, M, sizeof(matrix[0]), comparatorFromList, list);
```

**Impact:** Hard to maintain. If field limit needs changing, requires hunting down SLOTS usage.

**Fix:** Define constants: `#define MAX_FIELD_LEN 61`, `#define TEMP_FILENAME_LEN 16`.

---

**Code duplication - CSV writing pattern (4 occurrences)**

Same loop pattern for fprintf with comma separation used 4x:

(`src/sort.c:171-177`)
```c
for (int w = 0; w < K; w++) {
    if (w == K - 1) {
        fprintf(pfiles[chosenFile + pCopy], "%s\n", firstString[w]);
    } else {
        fprintf(pfiles[chosenFile + pCopy], "%s,", firstString[w]);
    }
}
```

Also in `src/files.c:145-164` (unmatched field writing).

**Impact:** Maintenance burden. Bug fix needs 4 locations.

**Fix:** Extract `void writeCSVLine(FILE *f, char **fields, int count)` function.

---

**Code duplication - Comparison logic (3 occurrences)**

Column-by-column comparison logic repeated:

(`src/sort.c:17-23`)
```c
for (int i = 0; i < n; i++) {
    result = strcmp(g1[array[i]], g2[array[i]]);
    if (result != 0) return result;
}
```

Also in `src/files.c:110-115` and `src/sort.c:148-152`.

**Fix:** Extract `int compareByColumns(char **row1, char **row2, int *cols, int colCount)`.

---

**Code duplication - realloc pattern (5+ occurrences)**

Length checking + realloc pattern repeated:

(`src/sort.c:157-161`)
```c
unsigned length = strlen(auxMatrix[b][k]);
if (length + 1 > SLOTS) {
    firstString[k] = realloc(firstString[k], length + 1);
}
strcpy(firstString[k], auxMatrix[b][k]);
```

Occurs in sort.c multiple places, files.c.

**Fix:** Extract `void safeCopy(char **dest, char *src, int *destSize)`.

---

**Complex function - blockSorting (126 lines)**

`src/sort.c:69-195` does multiple tasks:
1. Allocate validation matrix + auxMatrix
2. Read initial lines (lines 92-101)
3. Find minimum line across partitions (lines 104-182)
4. Write winner and recurse

**Impact:** Hard to understand, debug, test. Deep nesting (5+ levels).

**Fix:** Split into:
- `readInitialLines()`: Lines 92-101
- `findMinimumLine()`: Lines 104-182
- `writeAndContinue()`: Line 166-177

---

### Medium Priority Issues

**Naming inconsistencies**

- K/K1/K2: Number of fields. Used interchangeably (sort.c uses K, files.c uses K1/K2).
- Portuguese/English mix: Comments in Portuguese, variable names English.
- P: Number of partitions, but also used as file offset context (confusing in blockSorting).

**Impact:** Harder for contributors to understand intent.

**Fix:** Use consistent names: `fieldsPerLine`, `partitionCount`. Translate comments.

---

**Poor separation of concerns**

Sorting logic mixed with file operations:
- `src/sort.c:225-311`: externalSorting() handles file I/O + sorting
- `src/files.c`: mergeFiles() in wrong file (logic not about file operations)

**Impact:** Harder to reuse, test, or swap implementations.

**Fix:** Move all file operations to files.c, keep sort.c pure sorting logic.

---

**Algorithmic inefficiencies**

String copies in blockSorting loop (lines 156-162) copy entire rows repeatedly without need.

Multi-way merge uses validation matrix which is complex; simpler heap-based approach exists.

**Impact:** Slower on large datasets. Not critical for prototype.

---

### Low Priority Issues

**Comment quality**

Mix of Portuguese and English. Many complex sections lack explanation. Example (`src/sort.c:85-89`):
```c
// O entendimento dessa função é um pouco complicado visto que foi feito de forma iterativa,
// portanto possui muitos laços de repetição.
```

**Fix:** Translate to English, add algorithm pseudocode for complex sections.

---

**Portability issue - qsort_r**

qsort_r signature differs between BSD (macOS) and GNU (Linux):
- BSD: `void qsort_r(void *base, size_t nmemb, size_t size, void *thunk, int (*compar)(void *, const void *, const void *))`
- GNU: `void qsort_r(void *base, size_t nmemb, size_t size, int (*compar)(const void *, const void *, void *), void *arg)`

Current code assumes BSD (macOS).

**Impact:** Code won't compile on Linux without modification.

**Fix:** Add configure check or use portable alternative (e.g., sort then pass list as global, or use C++ std::sort).

---

**Unused validation logic**

The validation matrix in Phase 2 (blockSorting) serves the same purpose as tracking EOF in simpler code. It's correct but more complex than necessary for the two-file case.

**Fix:** Not critical; works as-is. Could simplify if code becomes maintenance burden.

---

## Testing Strategy

Run test suite to verify correctness and memory safety:

```bash
# Build
make all

# Functionality tests
make run      # Should output matching rows
make run0     # Test with large dataset
make run2     # Test with different parameters
make run3     # Test with Portuguese dataset

# Memory checks (requires Valgrind)
make val      # Quick memory check
make full     # Detailed leak detection
```

Compare output files to verify join correctness.
