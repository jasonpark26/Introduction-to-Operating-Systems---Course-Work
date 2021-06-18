
# CSC 360, Summer 2021
## Assignment #1 evaluation

* Name: `Jason Park`
* VNum: `V00946007`
* Netlink: `jasonpark`

---

The evaluation below refers in places to specific test numbers.
Details of these tests can be found in `TESTS.md` located within
a file named `a1-testing-stuff.zip` that can be found on Brightspace
in the `Assignments` section. Also in that ZIP archive are the
`.vshrc` files used for the tests (i.e., files that were renamed
to `.vshrc` before running `./vsh`).

Evaluation attempted to avoid "double-docking" as much as possible
(for example, when testing for a command with output re-direction,
sub-commands without arguments were used just in case arguments are
not yet supported by the shell implementation).


---
### Preliminaries

* `vsh.c` submitted: **File submitted successfully.**
* `vsh.c` compiles without errors: **The program compiled successfully.**
* `README.md` (or equivalent) provided: **Well-written documentation.
 A good overview of functionality.**
* Prompt as required is shown; `exit` works (test 1): **Prompt appears as expected from .vshrc file. The program exits correctly.**


---
### Path

* Shell when single path `/bin` is given in `.vshrc`  (test 2): **Using a single path in the RC file (`/bin/`), the program runs correctly and once the command is completed, the shell prompt returns.**
* Shell when single path `/usr/bin` is given in `.vshrc`  (test 3): **Using a single path in the RC file (`/usr/bin/`), the program runs correctly and once the command is completed, the shell prompt returns.**
* Shell when single path `./` is given in `.vshrc`  (test 4): **Using a single path in the RC file (`./`), the program runs correctly and once the command is completed, the shell prompt returns.**

* Shell when all three paths are provided in `.vshrc` (test 5): **Using all 3 paths in the RC file (`/bin/`, `/usr/bin/`, `./`), the program runs correctly and once the command is completed, the shell prompt returns.**


---
### Command arguments

* Commands with three, five, or seven arguments work correctly (test 6): **Single command with three, five and seven arguments is executed correctly.**


---
### I/O re-direction

* Output redirection correctly implemented (test 7): **Output redirection is not implemented.**
* Input redirection correctly implemented (test 8): **Input redirection is not implemented.**


### Command timing

* Time taken to execute a command is correctly shown (test 9): **The program correctly displays the wallclock time required to execute the given command.**


### Combining features

* Both input and output re-direction simultaneously (test 10): **Input redirection is not implemented.
Output redirection is not implemented.**
* Both output and input re-direction simultaneously (test 11): **Input redirection is not implemented.
Output redirection is not implemented.**
* Output and input redirection *and* timing simultaneously (test 12): **Input redirection is not implemented.
Output redirection is not implemented.
Timing a command is not implemented.**


---
### Further comments
Good effort on the assignment.
For most of the given test cases the program displays a command not found.

---
### Grade for submitted work (either letter or out of 100)
* **C**

