# CLI - Commands

## change <variable>
Possible to change all variables by name:
- simulation_time
- max_parking_cells
- ...
```bash
> <variable> <new_value>
```

## start simulation
```bash
> start
```

## exit program
```bash
> exit
> q
> quit
```

## help
print overview over all commands and usage
-> start menu will start with 'help' command run

## print configs
```bash
> configs
```
List config names and values in nice format. 
Needed to read them. 

# CLI - How it looks:
Once you start the program it will look like this: 
```
Welcome to our car park simulation. 
How to use: 
  > command1     Explanation
  > command2     ...
  > ...
Change configs: 
  > $config_name $new_value
  Allowed types for configurations: 
    - config_name(s): input_type
    - ...
...
Please enter your input: 
> 
```
To get a list of all configs you will have to run config command. 
Otherwise it should be very straightforward since we don't plan on having too many commands.
We will however not clear the terminal after each command/specific commands (except maybe "clear" command. To be discussed though)
 