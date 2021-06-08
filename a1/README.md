For a, using appendix_a.c, I created a main file that loops through commands and uses vsh%

For b, using the 9 arguments, I set up global variables. For being able to read .vshrc files I had a lot of help from stackoverflow using fopen, and can have an error message

For c-f, I couldn't get very far but my thought process was that I set up flags for each of the commands, ::ex, ex::, ##, and creating if statements for each case. For >, < equivallents, I'd find the string that has them using helper functions and try to strip the "::" from the string to have them set up as output/input files, for timing I tried to use appendix_f but was unsuccessful.