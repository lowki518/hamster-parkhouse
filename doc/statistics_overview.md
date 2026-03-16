# Overview of planned statistics

## Given data
### Garage
- Number of parking spaces
- Maximum parking duration
- Timestep of simulation
- Probability of a new car arriving
- Maximum amount of cars arriving
- Seed for "randomness"

### Car
- Car ID
- Remaining parking time 
- Time of entering 
- Random parking time (0 - max duration)

This data is needed to determine which statistic can be drawn from it.

## Useful statistic ideas
- Amount of cars currently in the garage
- Average parking time
- Total amount of cars in the garage during the simulation
- Amount of cars in waiting line
- Average waiting time
- Number of times the parkhouse was full
- __Bonus: which car type parked the most__

## File format

The calculated statistics will be put into a __.txt__ file when the simulation is exited. The format will be like this:


![File of Statistics output](img/statistics/statistics_file.png)

## Output format
The priority is to get the code running so the primary output is a terminal output like this:  

![Terminal of Statistics output](img/statistics/statistics_terminal.png)

----

### Bonus
A secondary goal is to get a GUI running with plotted graphs and buttons to switch between them.  
This is not a graph of data, it's just randomly generated numbers.

![Graph of Statistics output](img/statistics/statistics_graph.png)

## Justification

At first, we thought about a normal parking garage and which data the owner and users would like to have.

We thought about the simplest, yet most elegant way to display the data.
Hence the console output and the file are identical and structured for the best visibility.

We decided that a better visualization for long simulations would also be nice. So we searched across the web for a good solution
to visualize a graph for the data. We found the method of using Dear ImGui, but we didn't like the looks of it, so we decided to use the very basic libraries SDL3 and SDL3_TTF for fonts. This made the Visualization a bit harder, since we needed our own functions for __everything__. The given GUI is also just a basic version, since it was done as fast as possible, to just get an idea of how it will look.


# Documentation
The statistics first had to be split up into visualization and data processing.
## Data Processing
- The values had to be computed and interpolated to the size of the plotted graph. This belonged to the mathematical process of it all.
- Furthermore the data had to be read out of the files again.

## Visualization
- Consists of a graphic window with navigation options (x-> Exit (duh), > right (obv), < left (there were the thumb is on the right))
- Coordinate graph with a graph plotted in the middle 
---
- Why SDL? Cause Ben said so
- Why did Ben do it? Cause half psycho and cause he had done something similar before
- How was it tested? Using SDL local
## Biggest Problem + Solution
Due to codespace only using vscode, the window to the graph was everything but easy :(

With the use of lovely Youtube and Claude AI (<3) the idea of using Shell scripts and Docker to plot a whole Ubuntu desktop environment (WTF??? That workssss?!?!?!) (using vnc btw for those actually interested in the functionality of it all) in codespaces.

## Result
We have a graph :)

---
The whole graphic output took way too long. Would Ben do it again? Yes but using way more AI. Coding this whole thing robs you from every bit of happiness left in your soul

Anywayss...

The library is fucking huge as it needs to be that big. The GUI function is humungous, but that's due to the fact that it handles the whole entire GUI (makes sense right?).