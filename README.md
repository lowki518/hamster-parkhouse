# Hamster Parkhaus
> Ben Hibinger, Lionel Keilhack, Pamina Lessle

## Description
Welcome to our students project of a car park simulation. 

## How to Run 
 1. Start Codespaces

 2. Run in Terminal:
 ```bash 
 ./run.sh
 ```

 3. Wait a couple of moments until you see "Setup Complete!"

 4. Go to the "PORTS" menu

 5. Click on the link with "noVNC"

 6. Click on "VNC.html"

 7. Click on "connect"

 8. Open Terminal

 9. Run in Terminal: 
 
 ```bash
 ./park-house
 ```
10. Run the simulation as many times as you please 

11. Change specific configurations by their name and the new value, e.g.:
```bash
max_car_cells 500
```
This sets the max_car_cells to 500. 
For calculation reasons all integer values cannot get bigger than 2000000000 - 1, but I believe, that should be more than enough to simulate a wide range of hamster parkhouses ;)
