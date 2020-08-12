# CLI Calendar
School project that turned out really well, this app displays a calendar for the entered date and you further have the ability to interactively create a profile, add and remove events for any date.
The users password is hashed and the event text is encrypted, albeit currently with a weak homebrew encryption algorithm.
The program compiles and works on both Windows(MSVS) and Linux.

### Further improvements that could be implemented

* Replace the current hashing algorithm, MurmurOAAT64, with a cryptographic hash function
* Replace the current text encryption algorithm with a proper one
* Add the ability to add, remove and view user events for dates with just input from the command line
