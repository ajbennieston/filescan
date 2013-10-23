filescan
========

Search binary files for a string and see a hexdump of the surrounding
region. For example, searching for the string 'context' within the
filescan binary:

```
./filescan context filescan
---- Offset: 3931 (0x00000000000f5b) ----
0000000000000f1b: 000f 1f40 0048 89f2 89fe bf01 0000 00e9   ...@.H..........
0000000000000f2b: 71f7 ffff 9048 83ec 0848 83c4 08c3 0000   q....H...H......
0000000000000f3b: 0001 0002 0055 7361 6765 3a20 2573 2073   .....Usage:.%s.s
0000000000000f4b: 7472 696e 6720 6669 6c65 6e61 6d65 205b   tring.filename.[
0000000000000f5b: 636f 6e74 6578 742d 6c65 6e67 7468 5d0a   context-length].
0000000000000f6b: 0025 3031 367a 783a 2000 2532 2e32 6868   .%016zx:..%2.2hh
0000000000000f7b: 7800 2020 2573 0a00 2d2d 2d2d 204f 6666   x...%s..----.Off
0000000000000f8b: 7365 743a 2025 7a75 2028 2523 3031 367a   set:.%zu.(%#016z
```
