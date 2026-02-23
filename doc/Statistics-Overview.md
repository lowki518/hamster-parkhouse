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
----
10;2;1200  
**timestep;**  
**units of timestep (seconds, minutes, hours, weeks -> 1, 2, 3, 4);**  
**duration of the simulation in timesteps**  
550;50;82;3;8571265  
**parking spaces;**  
**maximum parking time in timesteps;**  
**probability of new cars arriving in percent;**  
**maximum amount of new cars per timestep;**  
**seed used for randomness**  

230;20;0;1;5690;VW  
**cars currently parked**  
**average parking time**     
**cars waiting in line**  
**how often the parkhouse was full**  
**amount of cars during simulation**  
**most parked car brand**  

----
5;2;5000  
600;60;60;10;6728495

0;0;0;0;0;NULL  
...  
600;40;5;100;10460;BMW  


## Output format
The priority is to get the code running so the primary output is a terminal output like this:  


| Timestep | Parked Cars | Avg. Parking Time | Simulated Cars | Cars Waiting | Full House Steps | Most Parked Brand |
|----------|-------------|-------------------|----------------|--------------|-------------------|--------------------|
|1|2|0|2|0|0|PEUGEOT|  
|2|3|0|3|0|0|PEUGEOT|
|...|...|...|...|...|...|...|
|234|400|46|1245|0|2|MERCEDES|

### Bonus
A secondary goal is to get a GUI running with plotted graphs and buttons to switch between them.  
## TODO
Add image of graph visualization