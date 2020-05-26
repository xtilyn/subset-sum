#!/bin/bash
time ./subset 1 < test3.txt &&
time ./subset 2 < test3.txt &&
time ./subset 3 < test3.txt &&
time ./subset 4 < test3.txt &&
time ./subset 6 < test3.txt &&
time ./subset 8 < test3.txt &&
time ./subset 12 < test3.txt &&
time ./subset 16 < test3.txt &&
time ./subset 24 < test3.txt &&
time ./subset 32 < test3.txt
