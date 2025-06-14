# Spacecraft Simulation

This project simulates a single spacecraft traveling between planets.  Input data
for systems, planets and spacecraft are provided in CSV files located in the
`SpacecraftSim` directory.  Simulation results are stored in a SQLite database.

## Building

The project can be built with any C++17 compiler.  On Linux with
`libsqlite3-dev` installed the following command will build the executable:

```bash
g++ -std=c++17 -I SpacecraftSim SpacecraftSim/*.cpp -lsqlite3 -o spacecraft_sim
```

Alternatively a simple `Makefile` is provided.  Run:

```bash
make
```

## Running the simulation

Run the executable from inside the `SpacecraftSim` directory so that the CSV
files can be located:

```bash
cd SpacecraftSim
../spacecraft_sim
```

A database named `spacecraft_simulation.db` will be created in the directory
containing the executable.  The `simulation_data` table will contain the logged
position, velocity and acceleration for each time step.

## Dependencies

- C++17 compiler
- SQLite development library
