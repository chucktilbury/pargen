# Random Notes
This file contains random thoughts about solving problems and future activities. Notes toward the beginning of the file are newer.

## Emitting data structures.
The data structure emitter works by traversing the AST. All of the information needed is present to do that, but the data cannot be generated using the pre and post functions at each node. It's because those calls are stateless and independent of one another. The communication between states happens outside of the those functions. 

### possible solutions
* Create a traverse function that is not recursive so that it has access to the data in individual nodes. That way, a single state machine could be created. However, that defeats the generic nature of the tree structure. 
* Create a pre/post function structure that drives a separate state machine using a local static state. All the pre/post functions would do is change that static state. This has the advantage of keeping the generic nature of the traverse function and focuses all of the state in a single (albeit large) function that handles all actions.
* Pass a data structure around that contains the state of the AST. (Or have a global state) This has the advantage of keeping all of the state data in one place, but the disadvantage may be to obfuscate how the states are used and updated. The state machine function will still be large.

### observations
* It would be fairly easy to solve this using OOP inheritance. It would be relatively easy to implement the traverse functionality in a parent class and the state machine in a child class.