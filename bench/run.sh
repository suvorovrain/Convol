#!/bin/bash

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
PROJECT_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"

RESULTS_DIR="$PROJECT_ROOT/bench/results"
BIN="$PROJECT_ROOT/build/convol"
DATASET_DIR="$PROJECT_ROOT/image-examples"
OUT_DIR="$PROJECT_ROOT/image-results"

mkdir -p "$RESULTS_DIR" "$OUT_DIR"

algos=(
  "linear linear classic"
  "parallel_row parallel_row classic"
  "parallel_col parallel_column classic"
  "parallel_pixel parallel_pixel classic"
  "q_linear linear queue"
  "q_parallel_row parallel_row queue"
  "q_parallel_col parallel_column queue"
  "q_parallel_pixel parallel_pixel queue"
)

for entry in "${algos[@]}"; do
    set -- $entry
    algo=$1
    mode=$2
    type=$3

    out_file="$RESULTS_DIR/$algo.txt"
    : > "$out_file"

    for dataset in small big; do
        "$BIN" "$DATASET_DIR/$dataset"/*.bmp emboss 2 "$OUT_DIR" "$mode" "$type" >> "$out_file"
    done
done
