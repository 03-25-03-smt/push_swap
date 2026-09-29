*This project has been created as part of the 42 curriculum by vladyslb, sekryzhe.*

# push_swap

## Description

`push_swap` sorts a list of integers using two stacks (`a` and `b`) and a
fixed set of 11 operations (`sa sb ss pa pb ra rb rr rra rrb rrr`). The
program does not print the sorted numbers: it prints the **list of
operations** that sorts stack `a` (smallest on top), and the goal is to make
that list as short as possible.

The binary embeds four strategies, selectable at runtime:

| Flag          | Strategy                                   | Class (in operations) |
|---------------|--------------------------------------------|-----------------------|
| `--simple`    | selection sort (min extraction)            | O(n²)                 |
| `--medium`    | √n-chunk sort + windowed cheapest insertion| O(n√n)                |
| `--complex`   | 3-way quicksort on chunks                  | O(n log n)            |
| `--adaptive`  | chooses a method from the measured disorder (default) | depends on regime |

`--bench` prints, on stderr, the disorder, the strategy and its class, the
total number of operations and the count of each operation.

The bonus program `checker` reads operations on stdin, executes them and
prints `OK` if `a` is sorted and `b` is empty, `KO` otherwise.

## Instructions

```sh
make            # builds push_swap
make bonus      # builds checker
make clean / make fclean / make re
```

```sh
./push_swap 2 1 3 6 5 8
./push_swap --complex 4 67 3 87 23 | ./checker_linux 4 67 3 87 23
ARG=$(shuf -i 0-9999 -n 500 | tr '\n' ' '); ./push_swap $ARG | wc -l
./push_swap --bench $ARG 2> bench.txt | ./checker $ARG
./push_swap --bench --medium $ARG > /dev/null     # only the metrics
```

Arguments may be separate (`3 2 1`) or grouped in one string (`"3 2 1"`).
Flags may appear anywhere; giving more than one strategy selector is an
error. `Error\n` is printed on stderr for non-integers, values outside the
`int` range, duplicates, empty arguments or unknown flags. Without numbers,
nothing is printed.

Benchmark example:

```
[bench] disorder: 49.81%
[bench] strategy: Adaptive / O(n√n)
[bench] total_ops: 4181
[bench] sa: 0 sb: 0 ss: 0 pa: 497 pb: 497
[bench] ra: 1411 rb: 279 rr: 348 rra: 574 rrb: 395 rrr: 180
```

Test script (random inputs, each result verified with `checker_linux`):

```sh
python3 tests/bench.py 100 300            # n=100, 300 runs, default mode
python3 tests/bench.py 500 50 --complex
python3 tests/bench.py 100 20 --disorder 0.1   # nearly sorted inputs
```

## Algorithms

### Common building blocks

* Ranks. Values are first replaced by their rank (0 = smallest). Only
  the order matters, and ranks make chunk bounds trivial. Sorting a copy
  with merge sort also detects duplicates.
* Disorder. Exactly the subject's formula: number of pairs `i < j` with
  `a[i] > a[j]`, divided by `n(n-1)/2`, measured before any move. Regimes
  are compared with integers (`5·mis < tot` means `d < 0.2`), so there is
  no floating-point error at the thresholds.
* Stacks are circular buffers: every rotation is O(1).
* Cheapest insertion** (`greedy_insert`). Stack `a` is kept sorted up to a
  rotation. For a candidate `x` in `b`, the target in `a` (the smallest
  element bigger than `x`) is found by binary search. Moving it costs
  `max(ra, rb)` if both stacks rotate the same way (shared `rr`/`rrr`) and
  `ra + rb` otherwise. The 4 direction combinations are tried and the
  cheapest candidate is inserted with `pa`.
* Peephole optimizer.** Between two pushes, moves on `a` and moves on `b`
  are independent. The optimizer splits each segment into an a-list and a
  b-list, cancels inverse neighbours (`ra rra`, `sa sa`), then merges the two
  lists optimally (longest common subsequence) into `rr`, `rrr` and `ss`.
  Adjacent `pb pa` / `pa pb` are removed. It never changes the final state.

### Simple: selection sort, O(n²)

Bring the minimum of `a` to the top by the shortest rotation, `pb`, and repeat
until 3 elements remain (or `a` is already sorted), sort those 3 with at most 2
operations, then `pa` everything back.
Each extraction costs at most `k/2` rotations for a stack of size `k`, so the
total is at most `n²/4 + 2n`, which is **O(n²)**. Space: the two stacks, O(n).

### Medium: √n-chunk sort, O(n√n)

With chunk size `s = 5·⌊√n⌋ + 2`:

1. Ranks are grouped in chunks `[0, s)`, `[s, 2s)`, ... Chunks are pushed to
   `b` in order: `ra` until the top of `a` belongs to the current chunk, then
   `pb`. The lower half of each chunk is sent to the bottom of `b` (`rb`), so
   the biggest ranks of `b` always stay close to one end of `b`.
2. The 3 remaining elements of `a` are sorted.
3. Cheapest insertion back into `a`, restricted to the `s` biggest ranks
   still in `b` (they sit close to the ends of `b`, and their place in `a` is
   among the last `s` inserted elements).
4. Rotate `a` so that the minimum is on top.

Upper bound: step 1 performs at most `2n` pushes/`rb`, plus at most one full
sweep of `a` per chunk. That is `n/s` chunks times `n` rotations, so
`O(n²/s)`. Each insertion in step 3 costs `O(s)`, so `O(n·s)` in total. With
`s ∝ √n`, the total is **O(n√n)**. The factor 5 was tuned on random inputs.
It changes the constant, not the class. Space: O(n).

### Complex: 3-way quicksort on chunks, O(n log n)

A *chunk* is a set of consecutive ranks stored contiguously at one of four
places: top of `a`, bottom of `a`, top of `b`, bottom of `b`. Splitting a
chunk sends each of its elements, in one pass, to one of the three *other*
places: the top third (the biggest ranks) toward `a`, the middle third and
the bottom third to the other positions. Each move costs 1 to 3 operations
(`move_elem`). The three parts are then sorted recursively, biggest first,
so they stack up sorted on top of `a`. Chunks of 1 to 3 elements are
finished with small hand-written sequences.

Upper bound: at each recursion level, every element is moved once with at most 3
operations, and chunk sizes are divided by 3. That gives `⌈log₃ n⌉` levels, so
at most `3n·log₃ n + O(n)` operations: **O(n log n)**. Space: O(n) for the stacks,
plus O(log n) recursion depth.

### Adaptive (default)

| Regime                | Disorder d        | Technique                       | Bound      |
|-----------------------|-------------------|---------------------------------|------------|
| already rotated       | any               | rotate only                     | O(n)       |
| tiny input (n ≤ 5)    | any               | selection sort (≤ 12 ops)       | O(1)       |
| low                   | d < 0.2           | LIS + cheapest insertion        | O(n²)      |
| medium                | 0.2 ≤ d < 0.5     | √n-chunk sort (as `--medium`)   | O(n√n)     |
| high                  | d ≥ 0.5           | 3-way quicksort (as `--complex`)| O(n log n) |

Low disorder: LIS + insertion. The longest increasing subsequence of `a`
(read cyclically from the minimum, found by patience sorting) is already in
order, so it stays in `a`. Only the other `k` elements are pushed to `b`
(nearest first) and put back with cheapest insertion. Each push costs at most
`n/2` rotations and each insertion at most `n` rotations, so the worst case is
O(n²). Since `k ≤ n`, the practical cost is `O(n + k·n)`, and `k` is small
when the input is nearly sorted.

Why these techniques per regime. With low disorder most of the input is
already ordered. A method that keeps the ordered part in place, like LIS, only
pays for the few misplaced elements. Its O(n²) worst case is allowed there and
never reached in practice. At medium disorder there is no long ordered run
worth keeping, so the chunk sort gives an order-insensitive O(n√n) with a
small constant. With high disorder (random or reversed input) the quicksort
gives the best asymptotic guarantee. The thresholds 0.2 and 0.5 are the ones
required by the subject. Random inputs have a disorder of about 0.5 ± 0.03,
so they fall on either side of 0.5. Both methods were tuned to stay well
below 700 / 5500 operations for 100 / 500 numbers.

### Measured results (random inputs, verified with `checker_linux`)

| n    | default (adaptive) avg / max | `--complex` avg | `--medium` avg | `--simple` avg |
|------|------------------------------|-----------------|----------------|----------------|
| 100  | 588 / 625  (300 runs)        | 607             | 564            | 1465           |
| 500  | 4110 / 4264 (60 runs)        | 4121            | 4106           | 32937          |

Nearly sorted input (n = 100, d ≈ 0.1): about 270 operations. Rotated sorted
input: only rotations.

## Code layout

| File                      | Content                                         |
|---------------------------|-------------------------------------------------|
| `stack.c`, `stack_rot.c`  | circular-buffer stack, sortedness checks        |
| `ps.c`, `output.c`        | operations on both stacks, recording, printing  |
| `parse.c`, `ranks.c`      | argument parsing, errors, ranks, disorder       |
| `strategy.c`              | strategy selection, adaptive regimes            |
| `strat_simple.c`          | selection sort, 3-element sort                  |
| `lis.c`, `strat_lis.c`    | low-disorder method                             |
| `strat_medium.c`          | chunk sort                                      |
| `quick*.c`                | 3-way quicksort                                 |
| `moves.c`, `moves_cost.c` | rotations, cheapest insertion                   |
| `optimize*.c`             | peephole optimizer                              |
| `bench.c`                 | `--bench` output                                |
| `bonus/`                  | `checker`                                       |

## Contributions

vladyslb: _to fill in (parsing, stacks, quicksort, tests)_
sekryzhe: _to fill in (disorder, medium/adaptive, checker, README)

## Resources

* D. Knuth, *The Art of Computer Programming, Vol. 3: Sorting and Searching*
* Cormen et al., *Introduction to Algorithms*: quicksort, LIS, LCS
  (dynamic programming)
* [Big-O notation (Wikipedia)](https://en.wikipedia.org/wiki/Big_O_notation)
* [Inversion (discrete mathematics)](https://en.wikipedia.org/wiki/Inversion_(discrete_mathematics)):
  the disorder metric
* [Longest increasing subsequence](https://en.wikipedia.org/wiki/Longest_increasing_subsequence):
  patience sorting
* [Push_swap: the least amount of moves with two stacks](https://medium.com/@jamierobertdawson/push-swap-the-least-amount-of-moves-with-two-stacks-d1e76a71789a)
* [Push swap tutorial (3-way chunk quicksort)](https://medium.com/@ulysse.gks/push-swap-in-less-than-4200-operations-c292f034f6c0)

### Use of AI

An AI assistant was used to:
* read and summarize the subject, and list the requirements (4 strategies,
  disorder, `--bench`, error cases);
* propose and discuss algorithm designs, and draft code for them, which we
  then reviewed, tested and adjusted;
* draft this README.
