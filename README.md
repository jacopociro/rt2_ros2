This package contains the ros2 files for the first assignment of Research Track 2.
# DESCRIPTION OF THE CONTENTS OF THE PACKAGE
The package is composed of:
- Two nodes: 
    - psotion_server.cpp
    - state_machine.cpp
- Three custom services:
    - Command.srv
    - Position.srv
    - RandomPosition.srv
- One launch file:
    - sim.py
- One mapping rule file:
    - mapping_rule.yaml
- Three source fils:
    - ros1.sh
    - ros2.sh
    - ros12.sh
## NODES
### position_server.cpp
Implements a service (RandomPosition.srv) that, given a range, returns a random position for the robot to reach.
### state_machine.cpp
This is the node that calls both the services in go_to_point.py and position_service.cpp and the service that is called in user_interface.py. This node waits for user_interface.py to tell it to call the other two services and start the robot.

## MAPPING RULE
This contains the name of the packages that have to communicate via the ros bridge. If you change names to the package you must also edit this file.
