boggle
======

A little boggle solver i did in an hour or so.

Usage: ./boggle /path/to/disctionary/file

Currently requires you to edit the source file to change the boggle layout.

Does full boggle rules. No using a tile that has already been used. No words under 4 letters, etc.

Can theoretically support boggle boards of any size. Tested with 5x5

Very fast as well. 0.04 seconds to do a 5x5 boggle board with a maximum depth of 25 (on my laptop with one thread)

Currently does not do Qu. I have an idea to fix it, but it will be a little weird.

I also have an idea to get this thing multithreaded.


Example 

./boggle /usr/share/dict/words | sort -u | uniq

Sorts and removes duplicate results.


Compiling

gcc -Wall -O3 boggle.c -o boggle 
