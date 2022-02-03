==63321== Memcheck, a memory error detector
==63321== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
==63321== Using Valgrind-3.15.0-608cb11914-20190413 and LibVEX; rerun with -h for copyright info
==63321== Command: ./a.out 5\ -2\ 3\ 7\ 5\ -2\ -1\ -1\ -2\ 0\ 3\ 5\ -2
==63321== Parent PID: 21839
==63321== 
--63321-- 
--63321-- Valgrind options:
--63321--    --leak-check=full
--63321--    --show-leak-kinds=all
--63321--    --track-origins=yes
--63321--    --verbose
--63321--    --log-file=valgrind-out.txt
--63321-- Contents of /proc/version:
--63321--   Linux version 5.11.0-37-generic (buildd@lcy01-amd64-021) (gcc (Ubuntu 9.3.0-17ubuntu1~20.04) 9.3.0, GNU ld (GNU Binutils for Ubuntu) 2.34) #41~20.04.2-Ubuntu SMP Fri Sep 24 09:06:38 UTC 2021
--63321-- 
--63321-- Arch and hwcaps: AMD64, LittleEndian, amd64-cx16-lzcnt-rdtscp-sse3-ssse3-avx-avx2-bmi-f16c-rdrand
--63321-- Page sizes: currently 4096, max supported 4096
--63321-- Valgrind library directory: /usr/lib/x86_64-linux-gnu/valgrind
--63321-- Reading syms from /home/eclipthyx/Desktop/Informatica/year2/DS/datastructuren1/a.out
--63321-- Reading syms from /usr/lib/x86_64-linux-gnu/ld-2.31.so
--63321--   Considering /usr/lib/x86_64-linux-gnu/ld-2.31.so ..
--63321--   .. CRC mismatch (computed 975d0390 wanted 30bd717f)
--63321--   Considering /lib/x86_64-linux-gnu/ld-2.31.so ..
--63321--   .. CRC mismatch (computed 975d0390 wanted 30bd717f)
--63321--   Considering /usr/lib/debug/lib/x86_64-linux-gnu/ld-2.31.so ..
--63321--   .. CRC is valid
--63321-- Reading syms from /usr/lib/x86_64-linux-gnu/valgrind/memcheck-amd64-linux
--63321--    object doesn't have a symbol table
--63321--    object doesn't have a dynamic symbol table
--63321-- Scheduler: using generic scheduler lock implementation.
--63321-- Reading suppressions file: /usr/lib/x86_64-linux-gnu/valgrind/default.supp
==63321== embedded gdbserver: reading from /tmp/vgdb-pipe-from-vgdb-to-63321-by-eclipthyx-on-???
==63321== embedded gdbserver: writing to   /tmp/vgdb-pipe-to-vgdb-from-63321-by-eclipthyx-on-???
==63321== embedded gdbserver: shared mem   /tmp/vgdb-pipe-shared-mem-vgdb-63321-by-eclipthyx-on-???
==63321== 
==63321== TO CONTROL THIS PROCESS USING vgdb (which you probably
==63321== don't want to do, unless you know exactly what you're doing,
==63321== or are doing some strange experiment):
==63321==   /usr/lib/x86_64-linux-gnu/valgrind/../../bin/vgdb --pid=63321 ...command...
==63321== 
==63321== TO DEBUG THIS PROCESS USING GDB: start GDB like this
==63321==   /path/to/gdb ./a.out
==63321== and then give GDB the following command
==63321==   target remote | /usr/lib/x86_64-linux-gnu/valgrind/../../bin/vgdb --pid=63321
==63321== --pid is optional if only one valgrind process is running
==63321== 
--63321-- REDIR: 0x4022e10 (ld-linux-x86-64.so.2:strlen) redirected to 0x580c9ce2 (???)
--63321-- REDIR: 0x4022be0 (ld-linux-x86-64.so.2:index) redirected to 0x580c9cfc (???)
--63321-- Reading syms from /usr/lib/x86_64-linux-gnu/valgrind/vgpreload_core-amd64-linux.so
--63321--    object doesn't have a symbol table
--63321-- Reading syms from /usr/lib/x86_64-linux-gnu/valgrind/vgpreload_memcheck-amd64-linux.so
--63321--    object doesn't have a symbol table
==63321== WARNING: new redirection conflicts with existing -- ignoring it
--63321--     old: 0x04022e10 (strlen              ) R-> (0000.0) 0x580c9ce2 ???
--63321--     new: 0x04022e10 (strlen              ) R-> (2007.0) 0x0483f060 strlen
--63321-- REDIR: 0x401f5f0 (ld-linux-x86-64.so.2:strcmp) redirected to 0x483ffd0 (strcmp)
--63321-- REDIR: 0x4023370 (ld-linux-x86-64.so.2:mempcpy) redirected to 0x4843a20 (mempcpy)
--63321-- Reading syms from /usr/lib/x86_64-linux-gnu/libstdc++.so.6.0.28
--63321--    object doesn't have a symbol table
--63321-- Reading syms from /usr/lib/x86_64-linux-gnu/libgcc_s.so.1
--63321--    object doesn't have a symbol table
--63321-- Reading syms from /usr/lib/x86_64-linux-gnu/libc-2.31.so
--63321--   Considering /usr/lib/x86_64-linux-gnu/libc-2.31.so ..
--63321--   .. CRC mismatch (computed 86b78530 wanted e380f01c)
--63321--   Considering /lib/x86_64-linux-gnu/libc-2.31.so ..
--63321--   .. CRC mismatch (computed 86b78530 wanted e380f01c)
--63321--   Considering /usr/lib/debug/lib/x86_64-linux-gnu/libc-2.31.so ..
--63321--   .. CRC is valid
--63321-- Reading syms from /usr/lib/x86_64-linux-gnu/libm-2.31.so
--63321--   Considering /usr/lib/x86_64-linux-gnu/libm-2.31.so ..
--63321--   .. CRC mismatch (computed fcb42c76 wanted f6c95789)
--63321--   Considering /lib/x86_64-linux-gnu/libm-2.31.so ..
--63321--   .. CRC mismatch (computed fcb42c76 wanted f6c95789)
--63321--   Considering /usr/lib/debug/lib/x86_64-linux-gnu/libm-2.31.so ..
--63321--   .. CRC is valid
--63321-- REDIR: 0x4afc600 (libc.so.6:memmove) redirected to 0x48311d0 (_vgnU_ifunc_wrapper)
--63321-- REDIR: 0x4afb900 (libc.so.6:strncpy) redirected to 0x48311d0 (_vgnU_ifunc_wrapper)
--63321-- REDIR: 0x4afc930 (libc.so.6:strcasecmp) redirected to 0x48311d0 (_vgnU_ifunc_wrapper)
--63321-- REDIR: 0x4afb220 (libc.so.6:strcat) redirected to 0x48311d0 (_vgnU_ifunc_wrapper)
--63321-- REDIR: 0x4afb960 (libc.so.6:rindex) redirected to 0x48311d0 (_vgnU_ifunc_wrapper)
--63321-- REDIR: 0x4afddd0 (libc.so.6:rawmemchr) redirected to 0x48311d0 (_vgnU_ifunc_wrapper)
--63321-- REDIR: 0x4b18e60 (libc.so.6:wmemchr) redirected to 0x48311d0 (_vgnU_ifunc_wrapper)
--63321-- REDIR: 0x4b189a0 (libc.so.6:wcscmp) redirected to 0x48311d0 (_vgnU_ifunc_wrapper)
--63321-- REDIR: 0x4afc760 (libc.so.6:mempcpy) redirected to 0x48311d0 (_vgnU_ifunc_wrapper)
--63321-- REDIR: 0x4afc590 (libc.so.6:bcmp) redirected to 0x48311d0 (_vgnU_ifunc_wrapper)
--63321-- REDIR: 0x4afb890 (libc.so.6:strncmp) redirected to 0x48311d0 (_vgnU_ifunc_wrapper)
--63321-- REDIR: 0x4afb2d0 (libc.so.6:strcmp) redirected to 0x48311d0 (_vgnU_ifunc_wrapper)
--63321-- REDIR: 0x4afc6c0 (libc.so.6:memset) redirected to 0x48311d0 (_vgnU_ifunc_wrapper)
--63321-- REDIR: 0x4b18960 (libc.so.6:wcschr) redirected to 0x48311d0 (_vgnU_ifunc_wrapper)
--63321-- REDIR: 0x4afb7f0 (libc.so.6:strnlen) redirected to 0x48311d0 (_vgnU_ifunc_wrapper)
--63321-- REDIR: 0x4afb3b0 (libc.so.6:strcspn) redirected to 0x48311d0 (_vgnU_ifunc_wrapper)
--63321-- REDIR: 0x4afc980 (libc.so.6:strncasecmp) redirected to 0x48311d0 (_vgnU_ifunc_wrapper)
--63321-- REDIR: 0x4afb350 (libc.so.6:strcpy) redirected to 0x48311d0 (_vgnU_ifunc_wrapper)
--63321-- REDIR: 0x4afcad0 (libc.so.6:memcpy@@GLIBC_2.14) redirected to 0x48311d0 (_vgnU_ifunc_wrapper)
--63321-- REDIR: 0x4b1a0d0 (libc.so.6:wcsnlen) redirected to 0x48311d0 (_vgnU_ifunc_wrapper)
--63321-- REDIR: 0x4b189e0 (libc.so.6:wcscpy) redirected to 0x48311d0 (_vgnU_ifunc_wrapper)
--63321-- REDIR: 0x4afb9a0 (libc.so.6:strpbrk) redirected to 0x48311d0 (_vgnU_ifunc_wrapper)
--63321-- REDIR: 0x4afb280 (libc.so.6:index) redirected to 0x48311d0 (_vgnU_ifunc_wrapper)
--63321-- REDIR: 0x4afb7b0 (libc.so.6:strlen) redirected to 0x48311d0 (_vgnU_ifunc_wrapper)
--63321-- REDIR: 0x4b04d20 (libc.so.6:memrchr) redirected to 0x48311d0 (_vgnU_ifunc_wrapper)
--63321-- REDIR: 0x4afc9d0 (libc.so.6:strcasecmp_l) redirected to 0x48311d0 (_vgnU_ifunc_wrapper)
--63321-- REDIR: 0x4afc550 (libc.so.6:memchr) redirected to 0x48311d0 (_vgnU_ifunc_wrapper)
--63321-- REDIR: 0x4b18ab0 (libc.so.6:wcslen) redirected to 0x48311d0 (_vgnU_ifunc_wrapper)
--63321-- REDIR: 0x4afbc60 (libc.so.6:strspn) redirected to 0x48311d0 (_vgnU_ifunc_wrapper)
--63321-- REDIR: 0x4afc8d0 (libc.so.6:stpncpy) redirected to 0x48311d0 (_vgnU_ifunc_wrapper)
--63321-- REDIR: 0x4afc870 (libc.so.6:stpcpy) redirected to 0x48311d0 (_vgnU_ifunc_wrapper)
--63321-- REDIR: 0x4afde10 (libc.so.6:strchrnul) redirected to 0x48311d0 (_vgnU_ifunc_wrapper)
--63321-- REDIR: 0x4afca20 (libc.so.6:strncasecmp_l) redirected to 0x48311d0 (_vgnU_ifunc_wrapper)
--63321-- REDIR: 0x4be4490 (libc.so.6:__strrchr_avx2) redirected to 0x483ea10 (rindex)
--63321-- REDIR: 0x4af6260 (libc.so.6:malloc) redirected to 0x483b780 (malloc)
--63321-- REDIR: 0x4be4660 (libc.so.6:__strlen_avx2) redirected to 0x483ef40 (strlen)
--63321-- REDIR: 0x4be0c50 (libc.so.6:__memcmp_avx2_movbe) redirected to 0x48421e0 (bcmp)
--63321-- REDIR: 0x4bdfb60 (libc.so.6:__strcmp_avx2) redirected to 0x483fed0 (strcmp)
--63321-- REDIR: 0x4906b20 (libstdc++.so.6:operator new(unsigned long)) redirected to 0x483bdf0 (operator new(unsigned long))
--63321-- REDIR: 0x4be7670 (libc.so.6:__memcpy_avx_unaligned_erms) redirected to 0x48429f0 (memmove)
--63321-- REDIR: 0x4906b80 (libstdc++.so.6:operator new[](unsigned long)) redirected to 0x483c510 (operator new[](unsigned long))
--63321-- REDIR: 0x4904d70 (libstdc++.so.6:operator delete(void*)) redirected to 0x483cf50 (operator delete(void*))
--63321-- REDIR: 0x4be7650 (libc.so.6:__mempcpy_avx_unaligned_erms) redirected to 0x4843660 (mempcpy)
==63321== Invalid read of size 4
==63321==    at 0x10DDE3: Array<int>::expand() (in /home/eclipthyx/Desktop/Informatica/year2/DS/datastructuren1/a.out)
==63321==    by 0x10D48E: Array<int>::push(long long) (in /home/eclipthyx/Desktop/Informatica/year2/DS/datastructuren1/a.out)
==63321==    by 0x10C4FA: maxStapel<Array<int> >::decode(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >) (in /home/eclipthyx/Desktop/Informatica/year2/DS/datastructuren1/a.out)
==63321==    by 0x10B43A: maxStapel<Array<int> >::fill() (in /home/eclipthyx/Desktop/Informatica/year2/DS/datastructuren1/a.out)
==63321==    by 0x10ABD1: maxStapel<Array<int> >::maxStapel(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >) (in /home/eclipthyx/Desktop/Informatica/year2/DS/datastructuren1/a.out)
==63321==    by 0x10A65F: main (in /home/eclipthyx/Desktop/Informatica/year2/DS/datastructuren1/a.out)
==63321==  Address 0x4db1184 is 0 bytes after a block of size 4 alloc'd
==63321==    at 0x483C583: operator new[](unsigned long) (in /usr/lib/x86_64-linux-gnu/valgrind/vgpreload_memcheck-amd64-linux.so)
==63321==    by 0x10B08B: Array<int>::Array() (in /home/eclipthyx/Desktop/Informatica/year2/DS/datastructuren1/a.out)
==63321==    by 0x10AB72: maxStapel<Array<int> >::maxStapel(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >) (in /home/eclipthyx/Desktop/Informatica/year2/DS/datastructuren1/a.out)
==63321==    by 0x10A65F: main (in /home/eclipthyx/Desktop/Informatica/year2/DS/datastructuren1/a.out)
==63321== 
--63321-- REDIR: 0x4904da0 (libstdc++.so.6:operator delete[](void*)) redirected to 0x483d6e0 (operator delete[](void*))
==63321== Invalid read of size 4
==63321==    at 0x10DDE3: Array<int>::expand() (in /home/eclipthyx/Desktop/Informatica/year2/DS/datastructuren1/a.out)
==63321==    by 0x10D48E: Array<int>::push(long long) (in /home/eclipthyx/Desktop/Informatica/year2/DS/datastructuren1/a.out)
==63321==    by 0x10C4E7: maxStapel<Array<int> >::decode(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >) (in /home/eclipthyx/Desktop/Informatica/year2/DS/datastructuren1/a.out)
==63321==    by 0x10B43A: maxStapel<Array<int> >::fill() (in /home/eclipthyx/Desktop/Informatica/year2/DS/datastructuren1/a.out)
==63321==    by 0x10ABD1: maxStapel<Array<int> >::maxStapel(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >) (in /home/eclipthyx/Desktop/Informatica/year2/DS/datastructuren1/a.out)
==63321==    by 0x10A65F: main (in /home/eclipthyx/Desktop/Informatica/year2/DS/datastructuren1/a.out)
==63321==  Address 0x4db11d4 is 0 bytes after a block of size 4 alloc'd
==63321==    at 0x483C583: operator new[](unsigned long) (in /usr/lib/x86_64-linux-gnu/valgrind/vgpreload_memcheck-amd64-linux.so)
==63321==    by 0x10B08B: Array<int>::Array() (in /home/eclipthyx/Desktop/Informatica/year2/DS/datastructuren1/a.out)
==63321==    by 0x10AB82: maxStapel<Array<int> >::maxStapel(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >) (in /home/eclipthyx/Desktop/Informatica/year2/DS/datastructuren1/a.out)
==63321==    by 0x10A65F: main (in /home/eclipthyx/Desktop/Informatica/year2/DS/datastructuren1/a.out)
==63321== 
--63321-- REDIR: 0x4904d80 (libstdc++.so.6:operator delete(void*, unsigned long)) redirected to 0x483d160 (operator delete(void*, unsigned long))
--63321-- REDIR: 0x4af6850 (libc.so.6:free) redirected to 0x483c9d0 (free)
==63321== 
==63321== HEAP SUMMARY:
==63321==     in use at exit: 0 bytes in 0 blocks
==63321==   total heap usage: 59 allocs, 59 frees, 79,400 bytes allocated
==63321== 
==63321== All heap blocks were freed -- no leaks are possible
==63321== 
==63321== ERROR SUMMARY: 6 errors from 2 contexts (suppressed: 0 from 0)
==63321== 
==63321== 2 errors in context 1 of 2:
==63321== Invalid read of size 4
==63321==    at 0x10DDE3: Array<int>::expand() (in /home/eclipthyx/Desktop/Informatica/year2/DS/datastructuren1/a.out)
==63321==    by 0x10D48E: Array<int>::push(long long) (in /home/eclipthyx/Desktop/Informatica/year2/DS/datastructuren1/a.out)
==63321==    by 0x10C4E7: maxStapel<Array<int> >::decode(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >) (in /home/eclipthyx/Desktop/Informatica/year2/DS/datastructuren1/a.out)
==63321==    by 0x10B43A: maxStapel<Array<int> >::fill() (in /home/eclipthyx/Desktop/Informatica/year2/DS/datastructuren1/a.out)
==63321==    by 0x10ABD1: maxStapel<Array<int> >::maxStapel(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >) (in /home/eclipthyx/Desktop/Informatica/year2/DS/datastructuren1/a.out)
==63321==    by 0x10A65F: main (in /home/eclipthyx/Desktop/Informatica/year2/DS/datastructuren1/a.out)
==63321==  Address 0x4db11d4 is 0 bytes after a block of size 4 alloc'd
==63321==    at 0x483C583: operator new[](unsigned long) (in /usr/lib/x86_64-linux-gnu/valgrind/vgpreload_memcheck-amd64-linux.so)
==63321==    by 0x10B08B: Array<int>::Array() (in /home/eclipthyx/Desktop/Informatica/year2/DS/datastructuren1/a.out)
==63321==    by 0x10AB82: maxStapel<Array<int> >::maxStapel(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >) (in /home/eclipthyx/Desktop/Informatica/year2/DS/datastructuren1/a.out)
==63321==    by 0x10A65F: main (in /home/eclipthyx/Desktop/Informatica/year2/DS/datastructuren1/a.out)
==63321== 
==63321== 
==63321== 4 errors in context 2 of 2:
==63321== Invalid read of size 4
==63321==    at 0x10DDE3: Array<int>::expand() (in /home/eclipthyx/Desktop/Informatica/year2/DS/datastructuren1/a.out)
==63321==    by 0x10D48E: Array<int>::push(long long) (in /home/eclipthyx/Desktop/Informatica/year2/DS/datastructuren1/a.out)
==63321==    by 0x10C4FA: maxStapel<Array<int> >::decode(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >) (in /home/eclipthyx/Desktop/Informatica/year2/DS/datastructuren1/a.out)
==63321==    by 0x10B43A: maxStapel<Array<int> >::fill() (in /home/eclipthyx/Desktop/Informatica/year2/DS/datastructuren1/a.out)
==63321==    by 0x10ABD1: maxStapel<Array<int> >::maxStapel(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >) (in /home/eclipthyx/Desktop/Informatica/year2/DS/datastructuren1/a.out)
==63321==    by 0x10A65F: main (in /home/eclipthyx/Desktop/Informatica/year2/DS/datastructuren1/a.out)
==63321==  Address 0x4db1184 is 0 bytes after a block of size 4 alloc'd
==63321==    at 0x483C583: operator new[](unsigned long) (in /usr/lib/x86_64-linux-gnu/valgrind/vgpreload_memcheck-amd64-linux.so)
==63321==    by 0x10B08B: Array<int>::Array() (in /home/eclipthyx/Desktop/Informatica/year2/DS/datastructuren1/a.out)
==63321==    by 0x10AB72: maxStapel<Array<int> >::maxStapel(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >) (in /home/eclipthyx/Desktop/Informatica/year2/DS/datastructuren1/a.out)
==63321==    by 0x10A65F: main (in /home/eclipthyx/Desktop/Informatica/year2/DS/datastructuren1/a.out)
==63321== 
==63321== ERROR SUMMARY: 6 errors from 2 contexts (suppressed: 0 from 0)
