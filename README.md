# Dancing-Links
C implementation of Knuth's dancing links algorithm acording to his article: [Dancing Links](https://www.ocf.berkeley.edu/~jchu/publicportal/sudoku/0011047.pdf).
The purpose to Knuth's algorithm is to solve the Exact Set Cover.

Many problems can be solved as an exact set cover, such as polyminos, sudoku and n-queens problem.

See the wikipedia [page](https://en.wikipedia.org/wiki/Exact_cover) for more details.

# Usage

You can use ```make``` to build the program. To run it you should use ```./dlx n fixed_rows```, where ```n```
is the size of the ```n x n``` board and ```fixed_rows``` is the ammount of pre processed rows. For the serial
implementation ```fixed_rows``` should be 1 or 2. For the mpi version, 2 or 3 works better, you should only
increase it above 3 if the program complains.

# License

See `LICENSE` for details.
