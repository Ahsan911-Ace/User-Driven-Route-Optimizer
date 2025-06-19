User-Driven-Route-Finder-and-Optimizer
A C++ console program that helps users find and optimize travel routes between locations, considering distance, traffic, construction, and travel mode. This project demonstrates object-oriented programming (OOP), user input handling, and basic pathfinding in C++.

Table of Contents
Features
How the Code Works
Code Structure
How to Use
Example Output
Customization
Who Should Use This
Features
Predefined Locations and Routes: Includes locations like Home, School, Mall, Park, Hospital, and Office, each connected by routes with specific attributes.
User Input: Select start and end locations, and choose your travel mode (walk, bike, car).
Pathfinding: Finds all possible routes between your selected locations using depth-first search (DFS).
Route Details: For each route, displays segment-by-segment breakdown, including distance, U-turns, traffic conditions, and construction status.
Time Estimation: Calculates estimated travel time for each route and travel mode, adjusting for traffic and U-turns.
Top Route Options: Shows the two fastest route options with detailed breakdowns and travel time estimates for all modes.
Best Route Recommendation: Recommends the best route, prioritizing those without construction if their time is within 10% of the fastest route.
User-Friendly Prompts: Validates all user inputs and guides the user through each step with clear messages.
How the Code Works
Initialization:
The program creates a set of locations and routes. Each route has a start and end location, distance, number of U-turns, traffic level, and construction status.

User Interaction:
The user is shown a list of locations and asked to select a start and end point by number. The user then selects a travel mode.

Pathfinding:
The program uses a depth-first search (DFS) algorithm to find all possible paths between the selected locations, avoiding cycles and limiting the path length for practicality.

Route Analysis:
For each route, the program calculates total distance, U-turns, construction presence, and estimates travel time for the chosen mode (with adjustments for traffic and U-turns).

Results Display:
The two fastest routes are displayed with all details. The best route is recommended, favoring those without construction if their travel time is close to the fastest.

Repeat or Exit:
The user can search for more routes or exit the program. All inputs are validated for correctness.

Code Structure
Class	Purpose
Location	Stores the name of each location
Route	Represents a route segment with start/end, distance, U-turns, traffic, construction
RouteManager	Manages locations, routes, and all pathfinding and display logic
UserInputHandler	Handles all user prompts, input validation, and main interaction loop
Key Algorithms and Functions:

DFS Pathfinding:
Finds all possible paths from start to destination, avoiding cycles and limiting depth.

Travel Time Calculation:
Calculates estimated time for each route segment and entire path, with speed and traffic adjustments.

Route Display:
Shows each segment’s details and summarizes total distance, U-turns, construction, and travel times.

Best Route Recommendation:
Compares all routes and recommends the best one based on construction status and travel time.

How to Use
Compile the code using a C++ compiler (e.g., g++): g++ -o route_optimizer User-Driven-Route-Optimizer.cpp
text

Run the program: ./route_optimizer
text

Follow the prompts to select start and end locations and travel mode.

View the results for route options and recommendations.

Example Output
==================== USER DRIVEN ROUTE FINDER and OPTIMIZER ====================

Available Locations:

Home

School

Mall

Park

Hospital

Office

Enter start location number: 1 Enter destination number: 3 Enter travel mode (walk/bike/car): car

==================== AVAILABLE ROUTES ====================

Route Option 1: Segment 1: Home -> School Distance: 3 km U-Turns: 1 Traffic: Low Construction: No ... Estimated Travel Time (car): 8.25 mins

==================== RECOMMENDATION ====================

Best Route (No Construction, within 10% time of fastest): [Route details shown here]

text

Customization
Add More Locations or Routes: Edit the initialization section in main() to include new locations or routes.
Adjust Traffic or Construction: Change the route attributes to simulate different scenarios.
Extend Features: Add more advanced features, such as live traffic updates or route saving.
Who Should Use This
This project is ideal for beginners learning C++ and object-oriented programming. It demonstrates class design, user input handling, basic algorithms (DFS), and simple route optimization.

For questions or suggestions, please open an issue in this repository.

