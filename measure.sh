#!/bin/bash
time ./a.out 1 < test3.txt &&
time ./a.out 2 < test3.txt &&
time ./a.out 3 < test3.txt &&
time ./a.out 4 < test3.txt &&
time ./a.out 6 < test3.txt &&
time ./a.out 8 < test3.txt &&
time ./a.out 12 < test3.txt &&
time ./a.out 16 < test3.txt &&
time ./a.out 24 < test3.txt &&
time ./a.out 32 < test3.txt
