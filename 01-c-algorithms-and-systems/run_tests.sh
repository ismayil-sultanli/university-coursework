#!/usr/bin/env bash
# Builds every C program in this folder and runs the automated checks.
# Usage: ./run_tests.sh
set -u
cd "$(dirname "$0")"
BUILD="$(mktemp -d)"; trap 'rm -rf "$BUILD"' EXIT
fail=0

echo "== Building all C programs =="
while IFS= read -r src; do
  out="$BUILD/$(basename "${src%.c}")"
  if gcc -std=c11 -O2 -o "$out" "$src" -lm 2>/dev/null; then echo "  ok    $src"
  else echo "  FAIL  $src"; fail=1; fi
done < <(find . -name '*.c' | sort)

echo "== Call-center simulation: expected output tests =="
T=data-structures/call-center-queue-simulation/tests
for in in "$T"/input*.txt; do
  n=${in##*/input}; n=${n%.txt}; exp="$T/expected$n.txt"
  if diff -q <("$BUILD/call_center_sim" "$in") "$exp" >/dev/null; then echo "  pass  input$n"
  else echo "  FAIL  input$n"; fail=1; fi
done

echo "== AVL student grades: load, search, statistics =="
out=$(cd data-structures/avl-student-grades && printf '1\n1002\n5\n0\n' | "$BUILD/avl_student_grades" students.txt)
if echo "$out" | grep -q "Alice" && echo "$out" | grep -q "Average grade *: 7.82"; then echo "  pass"
else echo "  FAIL"; fail=1; fi

echo "== Numerical methods: Newton-Raphson on x^3-2*x-5 =="
if printf '3\nx^3-2*x-5\n2\n0.00001\n50\n0\n' | "$BUILD/numerical_methods" | grep -q "2.0945"; then echo "  pass"
else echo "  FAIL"; fail=1; fi

[ $fail -eq 0 ] && echo "ALL CHECKS PASSED" || { echo "SOME CHECKS FAILED"; exit 1; }
