#include "Scenario.h"
#include "Database.h"

#include <iostream>
#include <map>

int main()
{
    std::cout << "Booting up Spacecraft Simulation..." << std::endl;

    Scenario* scenario = new Scenario();

    scenario->getDatabase()->createTables();

    std::cout << "Loading in files..." << std::endl;

    if (!scenario->loadFiles())
    {
        std::cout << " - Failed to load files. TODO: Put in _errors" << std::endl;
    }

    std::cout << "Compiling data..." << std::endl;
    if (!scenario->compile())
    {
        std::cout << " - Failed to compile data." << std::endl;
    }

    std::cout << "Running simulation..." << std::endl;
    auto error_code = scenario->runSimulation();

    if (error_code > 0)
    {
        std::cerr << "An error has been found, errorCode = " << error_code << ". Ending simulation early." << std::endl;
    }
    
    std::cout << "Simulation completed..." << std::endl;

    delete scenario;

    return 0;
}


/*

To make the simulation more realistic, you can consider adding the following features:

Atmospheric model: The current environment model only considers air resistance as drag force. However, in a real scenario, the atmosphere affects the spacecraft in many ways such as drag, lift, thermal heating, and atmospheric entry. Consider incorporating more sophisticated atmospheric models to account for these effects.

Multibody dynamics: Currently, the simulation considers only a single body (the spacecraft). Consider incorporating multiple bodies such as planets, moons, and asteroids to simulate complex celestial interactions.

Propulsion model: The current simulation does not consider the effects of the spacecraft's propulsion system. Consider adding a propulsion model that considers factors such as fuel consumption, engine thrust, and specific impulse.

Control system: Currently, the control system only allows for setting the thrust and orientation. Consider adding more advanced control algorithms such as proportional-integral-derivative (PID) controllers, and guidance, navigation, and control (GNC) algorithms.

Orbit prediction: Currently, the simulation does not consider the effects of celestial bodies on the spacecraft's orbit. Consider incorporating algorithms that can predict the spacecraft's orbit and its interactions with other celestial bodies.

Visualization: Consider adding 3D visualization tools to provide a more intuitive and interactive representation of the simulation.

Data logging: Consider adding the capability to log and store data for further analysis and visualization.

*/

/*
Simulation loop: Set up a simulation loop with a fixed time step to update the spacecraft's state. This can be done with a for loop or a while loop.

User inputs: Add user inputs to control the spacecraft, such as setting the thrust, orientation, and target trajectory.

Trajectory planning: Implement a trajectory planning algorithm, such as a two-point boundary value problem (TPBVP), to calculate the optimal trajectory for the spacecraft to reach a target.

Guidance, Navigation, and Control (GNC): Implement a GNC system to control the spacecraft and ensure it stays on the desired trajectory. This could include a Proportional-Integral-Derivative (PID) controller or a model predictive control (MPC) algorithm.

Data logging: Add data logging capabilities to store the state of the spacecraft and the environment at each time step, so that you can later analyze and visualize the simulation results.

Visualization: Add visualization capabilities to display the state of the spacecraft and the environment in real-time, or after the simulation is complete. This could be done using a 3D graphics library like OpenGL, or a 2D graphics library like SFML.

Scenario testing: Add scenario testing capabilities to test the performance of the spacecraft under different conditions, such as different target trajectories, different control algorithms, and different environmental conditions.

*/