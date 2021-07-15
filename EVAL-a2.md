
# CSC 360, Summer 2021
## Assignment #2 evaluation

* Name: `Jason Park`
* VNum: `V00946007`
* Netlink: `jasonpark`

---

The evaluation below refers in places to specific test numbers.
Details of these tests can be found in `TESTS.md` located within
a file named `a2-testing-stuff.zip` that can be found on Brightspace
in the `Assignments` section. Also in that ZIP archive are the
various scripts used for the tests. (Note that automatic marking was
not performed; rather, the scripts were designed to elicit certain
behavior from submitted solutions.)


---
### Preliminaries

* `vaccine.c` submitted (Part A): **File submitted successfully.**
* `vaccine` compiles without errors (Part A): **The program compiled successfully.**
* `meetup.c` submitted (Part B): **File submitted successfully.**
* `meetup` compiles without errors (Part B): **The program compiled successfully.**
* `README.md` (or equivalent) provided: **Suitably formatted README.md submitted.**


---
### Part A: Vaccine stations

* Single person, single station (test 0): **Single person, with a single station works correctly.**
* Three stations (test 1): **Using 3 stations works correctly.**
* Two stations (test 2): **Only one of two stations was doing the vaccination.**
* Five stations (test 3): **Using 5 stations works correctly.**
* Required synchronization primitives used?: **Only CVs and mutexes used.**
* Sensible overall structure of code: **Since `is_vac_completed = true;` is shared between it should be executed in a critical section.** 


---
### Part B: Boba-Fett-cosplay meetups

* Two threads, meetup sized two, meetfirst (test 4): **Both tasks meet up with "A" correctly shared.**
* Two threads, meetup sized two, meetlast (test 5): **Both tasks meet up with "B" correctly shared.**
* Seven threads, meetup sized three, meet first (test 6): **First three tasks meet up in a suitable way, with "A" as the codeword. The second set of tasks meetup with incorrect codeword.**
* Seven threads, meetup sized three, meet last (test 7): **First three tasks meet up in a suitable way, with "Z" as the codeword. The second set of tasks meetup with incorrect codeword.**
* Required synchronization primitives used?: **Only semaphores used.**
* Sensible overall structure of code: **Well structured well documented code.
The solutoin could have been implemented with fewer semaphores.** 


---
### Further comments

* Helpful `README.md` with description of solutions: **Overview of the implementation strategy provided.**
* Any other comments: **Good effort on the assignment.**

---
### Grade for submitted work (either letter or out of 100)
* **C**

