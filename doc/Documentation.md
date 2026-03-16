# Documentation 
## How did we go about solving the task?
Part 1:

Firstly we are making sure that we are all on the same page on how we want our parking garage to work. We split up the work evenly to cater to each strength. 

Everyone has worked on their part of the work load. We discussed how we want to itterate through the parking lot to check for any available parking spaces (heated discussion). 

The visualization is getting along, and the base data types are almost finished. 

We had our last meet up and checked everything was cleaned and ready to upload.

    Main Work distributions:

    Pamina: Flow charts, pseudo code
    Ben:    Visualization, statistics, cook
    Lionel: Config. base data types
    -> Helped each other

Part 2: 
    
    Main Work distributions:

    Pamina: translating pseudo code, main functions, documentation
    Ben:    Visualization, statistics, cook
    Lionel: Config. base data types
    -> Unit tests were made together and mixed
    -> Helped each other

The translating to code out of pseudo code is nearly finished. The visualization is prooving to be difficult (Lionel wants to give up). Realized we are missing some main functions (oopsie dasies, oh well).

Functions have been written, unit tests almost done, CLI getting along.

Functions and unit tests finished, CLI done. Cleaning up the final details and working on getting the simulation going. We hate seg faults. Got it built and eventually (finallllyyyyy) running!

It's not running anymore :( Final unit tests are done (file tests were made with AI as it's annoying), hoping to get this thing running. Motivation has left the chat. It runs. It doesn't. It runs. It doesn't. Do i need to continue? ITTTT WOOORRKKKSSSS. Finishing last touches

## What different options did we have regarding the structure and implementation? 
>Naming of parking...:

- ...house (winner)
- ...lot
- ...garage

- ...cell (winner)
- ...space
- ...slot

    Pamina unhappy

-> Decided by poll
>Free parking cells:

- When a car arrives itterate through cells, then queue
- When a car arrives go straight in queue and follow queue procedure
   
-> Nr. 2: as it's easier to implement in code

>Config

- One exit status
- Multiple ways to exit the simulation

->Nr. 2: It's way more logical because people are stupid and will probably still manage to not exit

>Statistics

- Most brand in car park
- Leave it out

-> Nr. 1: Because it's cool and it's yet another statistics


>GUI: 

- Have our output be a txt file, or just be printed in the CLI
- Have a GUI

    Lionel thinks it's too much work

-> Nr. 2: Do we even need a reason?! It's fcking cool!

- Make Ben have to work (have a GUI)
- Give Ben a break

    Ben wanted to

-> Nr. 1: Because GUI?

- Keep the colour blue
- Change the colour to a horrfic pink

    The majority (boys) won

-> Nr. 2: Not even I can explain it

- GUI Function all in one
- GUI Functions all split up

-> Nr. 1. Because splitting the functions up would mean that each smaller function would need an absurd amount of input variables and the library would be abnormally large 

- Keeping the library small
- Humongous GUI library

-> Nr. 2: Mainly cause it just needs to be (?)

## What could/would we do differently next time?
No GUI planning from the first day on (Lionel's opinion)

Start with a Flip Chart to explain evreyones perspective on how the parking house should work and look like.

Not using Codespaces and instead using Docker. Would it take longer? Yes. Would it save us from depressions? Also yes.

Cry more often (self car(e) is important).

Protect the "main" branch from day 1 (BENNNNNNN)

## Were there any issues regarding the Team?
As we mainly met up in person to discuss the progress, most issues were resolved pretty quickly. And when we needed a break, a movie and Ben's cooking (btw fcking great) usually helped.

## What worked well?
Team work and keeping each other updated on the progress. 

Had a lot of fun along the way (Test Simulation 69 with >70 MB space) with extremely long simulations. 

The unit tests usually worked first try, as long as the library was correct.

Hopefully our humor :)

## Wonderful Quotes to our project:

"AAAHHHHHHH" - Ben

"I have tech tourettes" - Lionel

"Have you tried giving up and leaving?"

"Merde" - Lionel, Ben, Pamina

"Awwww you want a cookie?" - Lionel

"TEAM - Toll Ein Anderer Machts" - Ben

"You are to incompetent for anything and everythig" - Lionel (talking to his laptop)

"YOU DON'T NEED TO DEREFRENCE THE POINTER!!!!!!" - Lionel

"YOU NEED TO DEFERENCE THE POINTER!!!!" - Lionel (honestly just make up your mind)

"Function pointers are fucking great" - Lionel

"I don't think the function pointers work" - Ben    ("Fuck" - Lionel)

"I'm not mad, I'm just deeply dissapointed" - Everyone at least once

"I do HipHop. HipHop off of the 7th floor" - Lionel (should we be worried?)

"Cetero censio Github Codespaces esse delendam" - Lionel 

"Your commit messages are way to boring" - Lionel

"That pink looks gay" - Pamina 

"Ha! GUIIIIIII" - Lionel