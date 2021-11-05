# ICS462
Operating System Project

The objective of this assignment is to provide some hands-on experience on multithreaded programming and thread synchronization. 

You will design and implement a multithreaded program to control traffic on an imaginary bridge with two lanes.

Traffic Patterns: Traffic will consist of cars and vans. A vehicle (car or van) may be traveling southbound or northbound: The direction and type of each vehicle will be determined according to given probability distributions (see below). Cars and vans will weigh 200 and 300 units each, respectively. Each vehicle will take 3 seconds to cross the bridge. Your program will enforce traffic control policies and bridge restrictions while providing fair and fast traffic flow.

Bridge Restrictions:

R1. Vehicles cannot change lanes while on the bridge, and a given lane should never be opened to traffic in opposite directions at the same time (otherwise, a collision will occur).

R2. Due to the construction constraints, the total weight of all the vehicles on the bridge cannot exceed 1200 units at any time.

Traffic Control Policies: 

The traffic flow will be controlled according to the following rules.

P1. As long as oncoming traffic is in both directions, one lane will be allocated for southbound and the other for northbound traffic.

P2. If there are no waiting/crossing vehicles in one direction, then both lanes will be used by the vehicles coming from the other direction (if any) to improve the traffic flow. And, while both lanes are being used in one direction, policy P1 will be effective as soon as vehicles arrive at the other end of the bridge. However, before changing the traffic direction in a given lane, your program must wait until all the vehicles already in that lane leave (otherwise, a crash will occur).

P3. Subject to restrictions R1-R2 and policies P1-P2, no vehicle should incur a delay. If there is sufficient traffic, the bridge capacity must be fully utilized (for example, having vehicles cross the bridge one by one is not acceptable). The deadlocks should also be prevented.

Representing Vehicles as Threads: You will represent each vehicle by one thread, which executes the procedure Vehicle-Routine upon arrival at the bridge: Vehicle-Routine(parameter-list)

{

Arrive(. . .) 

Cross(. . .) 

Leave(. . .)

}

The parameter-list in Vehicle-Routine above should at least contain:

Vehicle-id (an integer uniquely identifying the vehicle).
Vehicle-type (car or van).
Vehicle direction (northbound or southbound).
You can use additional parameters and determine the parameter lists of procedures Arrive, Cross, and Leave.

The Arrive procedure must check all the traffic/bridge restrictions, and it must not return until it is safe for the vehicle to cross the bridge. The Cross procedure will delay for 3 seconds for each vehicle. Finally, the Leave procedure must take additional steps to let the traffic control mechanism update its internal state and let additional vehicles, if any, cross the bridge. 

These procedures must print all the information necessary to follow the arrival and departure patterns and the list of waiting queues and crossing vehicles. Your program should, clearly, print the list of the vehicles that have arrived but not started to cross the bridge (i. e., the waiting vehicles). The format shown in the following example can be adopted:

Car #6 (northbound) arrived. Van #7 (northbound) arrived. Van #8 (southbound) arrived. .......

Car #6 is now crossing the bridge.

Van #7 is now crossing the bridge.

Van #8 is now crossing the bridge.

Bridge Status: Lane 1 - Northbound: [Car #3, Car #6, Van #7] Lane 2 - Southbound: [Car #5, Van #8]

Waiting Queue (Northbound): [Car #9, Van #10, Car #12] Waiting Queue (Southbound): [Van #4, Car # 11]

.......

Car #6 exited the bridge.

.......

You should print the traffic flow and list of the waiting vehicles.

Synchronization Primitives: You will write the Arrive, Cross, and Leave procedures using mutex locks and condition variables. 

Your solution must not employ busy waiting. In this assignment, for full credit, you should only use pthread_cond_wait()/pthread_cond_signal(). Avoid using the pthread_cond_broadcast() function or any other primitive that releases all the blocked threads at once. Because of the thread scheduling mechanism of the library you are using, you may occasionally observe that the vehicles do not leave the bridge in the same order they entered. You do not have to fix this problem.

Running Your Program with Specific Schedules: In this assignment, you have to run your program for 5 (five) vehicle arrival schedules given below:

10 [N/S: 1.0/0.0] : DELAY (10) : 10 [N/S: 1.0/0.0] : DELAY (10) : 10 [N/S: 1.0/0.0]
10 [N/S: 1.0/0.0] : DELAY (3) : 10 [N/S: 0.0/1.0] : DELAY (20) : 10 [N/S: 0.5/0.5]
15 [N/S: 0.6/0.4] : DELAY (3) : 15 [N/S: 0.6/0.4]
20 [N/S: 0.8/0.2] : DELAY (10) : 10 [N/S: 0.5/0.5]
15 [N/S: 0.7/0.3] : DELAY (3) : 15 [N/S: 0.0/1.0]
Each schedule will include a number of groups of vehicles separated by the DELAY time. The first integer in the specification of each group represents the number of vehicles arriving simultaneously at the bridge in that group, followed by the probability distribution determining the direction of the vehicles (northbound or southbound (N/S)). Then, the word DELAY and a number in parentheses indicate the delay before the next arrival(s). For example, schedule (4) 20 vehicles arrive at the bridge at the start of the experiment, then 10 more vehicles arrive 10 seconds after the arrival of the first 20 vehicles. Under each schedule, 30 vehicles arrive at the bridge during the course of the experiment.

The direction and type of each vehicle will be randomly determined when it arrives, according to the provided probabilities. First, the probabilities given in pair [N/S:...] determine the direction of each arriving vehicle for each group of vehicles. For example, in the schedule (4), each vehicle’s direction in the first group of 20 will be generated by the probabilities [N/S: 0.8/0.2], meaning that the probability that an arriving vehicle is northbound is 0.8, and the probability that an arriving vehicle is southbound is 0.2. Ten seconds later, the second group will arrive with probabilities are 0.5 for both northbound and southbound. Note that schedule (1) represents a northbound-only traffic pattern. Second, to determine the type of each vehicle, you will apply a similar technique, but you will assume that the probability that whether a given vehicle is a car or van is always 0.5 (that is why the “car/van” probability is not needed in the schedules). 

For each arriving vehicle, first, determine its direction and then its type according to these probabilities. You can use your system library functions to generate random numbers. Note that simply multiplying the total number of vehicles by the northbound/southbound (N/S) probabilities will NOT give the number of northbound/southbound vehicles in that group: you should generate a random number when determining each vehicle’s direction. For example, suppose there are 10 vehicles in a group with an N/S probability of 0.8/0.2; this does not mean you can immediately set the number of northbound and southbound vehicles to 8 and 2, respectively. The same applies to its type.

Important Note: The incoming vehicles must be generated one after the other without any delay unless the schedule calls for an explicit delay through DELAY statement. You should determine the directions of all the vehicles arriving simultaneously in a group before deciding on the next traffic flow pattern (one lane for each direction or both lanes for one-way traffic). As an example, in the schedule (4), your program should generate the directions of all 20 vehicles in the first group before determining the first traffic flow pattern. Unless all 20 vehicles are bound for the same direction, a separate lane should be initially allocated to the traffic in each direction.

Entering Schedules: You should design a simple user interface to allow the grader to enter any of the above schedules (or any other schedule). For example, your program may first prompt about the number of groups in the schedule. Then it may get information from the user about the number of vehicles in each group, the Northbound/Southbound probability for that group, followed by the DELAY before the next one (when applicable). The Car/Van probability can be set to 0.5/0.5 statically. Alternatively, your program may read the schedule information from a file according to a format of your preference. The bottom line is that the grader should not have to modify your source program and that he should be able to enter the information about the schedule of his preference easily. Your program should allow the user to enter a schedule other than the schedules shown above.
