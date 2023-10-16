# string-pattern-matching
String Pattern Matching Algorithm EX1
slist.c : Contains the doublylist definition and functions
pattern_matching.c:Builds the dfa and contains the pattern matching functions

==Description==
This program generate a Deterministic Finite Automaton (DFA) which we use for this program to locate pattern matches in a string, the program finds multiple pattern matches simultanlesly from a predefined string, we use a doubly list in this program to help us construct the transitions in the automaton, the doubly list is initialised and all if it's functions are defined in slist.c.

==Functions==
slist.c:
1- void dbllist_init : the function initilizes the doubly list (haid,tail).
2- void dbllist_destroy : this function destroys the lists by calling remove on all of its nodes.
3- int dbllist_append : this function adds (appends) a node to the end of the list.
4- int dbllist_prepend : this function adds a node to the beginning of the list.
5- int dbllist_remove :this function removes a node specified in its parameters from the list, also the function decides whether to free the data with the node or not depending on a parameter that tells it to.

pattern_matching.c:
1- int pm_init(pm_t *) : this function initializes the dfa by initilizing all of it's details.

2- pm_state_t* pm_goto_get(pm_state_t *state,unsigned char symbol) : this function searches for the state with the same symbol that is delivered in the parameter, returns the destination state if found and returns null otherwise.

3- int pm_goto_set(pm_state_t *from_state,unsigned char symbol,pm_state_t *to_state) : this function creates a new edge (transition) and attaches it to a from_state to a destination state, allowing it to connect states through updating the transition doubly list between states.

4- int pm_addstring(pm_t *,unsigned char *, size_t n) : this function recieves a dfa, a string and the string's length, the function calls the set function on every character of the string's characters and creates an edge for them if the transition for this symbol was not already there and it does that by checking if the get function for this character from our state to the destination is null, then  the character is already there, otherwise, it calls the set function to create a new edge and state for the symbol.

5- int pm_makeFSM(pm_t *): this function constructs the failiure states, for every state there is a failiure state, it is used if the pattern did not find the symbol it was looking for in its way then it transports to the state's failiure state and continue it's work from there, it does that through building a queue and adding states to it and combining them to the same symbol states and therfore connecting them to each other throught the state fail field in the struct, then we pop the queue one at a time until we have a fail state for all states.

6- dbllist_t* pm_fsm_search(pm_state_t *,unsigned char *,size_t) : this function recieves an initial state and a string to search for and its size, the function searches for the pattern in the automation, by defining a match and updating it's fields in the struct (end pos, start pos, etc...)  and appends every match to a match doubly list that contains all the infromation about where was the pattern found and where does it start and where does it end using and checking the output list of every state(the list which contains the pattern until that state).
.
7- void pm_destroy(pm_t *) :this function detroys the dfa through destroying and freeing every allocated element in the program like the transitions and outupts and edges and nodes, the function is calls a helper function and not from the function itself because we need to recursively destroy every state and the pm_destroy function only recieves the pm. the function checks if the pm is null and stops if so.

8- void destroy_pv(pm_state_t *state): this is the helper function for the pm_destroy function, it runs recursivly to destroy and free all the edges as well the lists(to free and destroy the list we call the doubly list free from slist to that specific list, mostly transitions and output lists).

==Program Files==
pattern_matching.c
slist.c


==How to compile==
Compile : gcc main.c slist.c pattern_matching.c -o ex1
Run :    ./ex1
Valgrind : valgrind --leak-check=yes ./ex1

==Input==
The user does not input but the program should take a set of patterns to build the dfa and a string. then to call the addstring.

==Output==

First output is the proccess of allocating states respectively in their order.
Second output is printing every failiure state for every state.
=====================================================================================
