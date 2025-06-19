#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <limits>

using namespace std;

enum class Traffic { LOW, MEDIUM, HIGH };

class Location {
    string name;
public:
    Location(const string& n) : name(n) {}
    string getName() const { return name; }
};

class Route {
    size_t startIndex;
    size_t endIndex;
    double distance;
    int uTurns;
    Traffic traffic;
    bool construction;

public:
    Route(size_t s, size_t e, double dist, int u, Traffic t, bool c)
        : startIndex(s), endIndex(e), distance(dist), uTurns(u), traffic(t), construction(c) {}

    size_t getStartIndex() const { return startIndex; }
    size_t getEndIndex() const { return endIndex; }
    double getDistance() const { return distance; }
    int getUTurns() const { return uTurns; }
    Traffic getTraffic() const { return traffic; }
    bool hasConstruction() const { return construction; }

    double calculateTime(const string& mode) const {
        double speed = 0;
        if (mode == "walk") speed = 5.0;
        else if (mode == "bike") speed = 15.0;
        else if (mode == "car") speed = 30.0;
        else speed = 5.0;

        double time = (distance / speed) * 60; // minutes

        if (mode == "car") {
            switch (traffic) {
                case Traffic::LOW: break;
                case Traffic::MEDIUM: time *= 1.1; break;
                case Traffic::HIGH: time *= 1.25; break;
            }
        }
        return time + uTurns * 2; // 2 minutes penalty per u-turn
    }
};

class RouteManager {
    vector<Location> locations;
    vector<Route> routes;
    const size_t maxDepth = 6;

    void dfs(size_t current, size_t destination, vector<size_t>& visited,
             vector<size_t>& currentPath, vector<vector<size_t>>& allPaths) {
        if (currentPath.size() > maxDepth) return;
        if (current == destination) {
            allPaths.push_back(currentPath);
            return;
        }
        for (size_t i = 0; i < routes.size(); ++i) {
            const Route& r = routes[i];
            if (r.getStartIndex() == current &&
                find(visited.begin(), visited.end(), r.getEndIndex()) == visited.end()) {
                visited.push_back(r.getEndIndex());
                currentPath.push_back(i);
                dfs(r.getEndIndex(), destination, visited, currentPath, allPaths);
                currentPath.pop_back();
                visited.pop_back();
            }
        }
    }

    string trafficToString(Traffic t) const {
        switch (t) {
            case Traffic::LOW: return "Low";
            case Traffic::MEDIUM: return "Medium";
            case Traffic::HIGH: return "High";
        }
        return "Unknown";
    }

public:
    void addLocation(const Location& loc) { locations.push_back(loc); }
    void addRoute(const Route& route) { routes.push_back(route); }
    const vector<Location>& getLocations() const { return locations; }

    vector<vector<size_t>> findAllPaths(size_t start, size_t end) {
        vector<vector<size_t>> allPaths;
        if (start >= locations.size() || end >= locations.size()) return allPaths;
        vector<size_t> visited = {start};
        vector<size_t> currentPath;
        dfs(start, end, visited, currentPath, allPaths);
        return allPaths;
    }

    void displayRoute(const vector<size_t>& path, const string& mode, bool showAllModes) const {
        double totalDistance = 0;
        bool constructionPresent = false;
        for (size_t i = 0; i < path.size(); ++i) {
            const Route& r = routes[path[i]];
            cout << "Segment " << i + 1 << ": " << locations[r.getStartIndex()].getName()
                 << " -> " << locations[r.getEndIndex()].getName() << "\n";
            cout << "  Distance: " << r.getDistance() << " km\n";
            cout << "  U-Turns: " << r.getUTurns() << "\n";
            cout << "  Traffic: " << trafficToString(r.getTraffic()) << "\n";
            cout << "  Construction: " << (r.hasConstruction() ? "Yes" : "No") << "\n";
            totalDistance += r.getDistance();
            if (r.hasConstruction()) constructionPresent = true;
        }
        cout << "--------------------------------------------------\n";
        cout << "Total Distance: " << totalDistance << " km\n";
        cout << "Construction: " << (constructionPresent ? "Present" : "None") << "\n";
        if (showAllModes) {
            cout << "Time Estimates:\n";
            cout << "  Walking: " << calculateTotalTime(path, "walk") << " mins\n";
            cout << "  Biking: " << calculateTotalTime(path, "bike") << " mins\n";
            cout << "  Driving: " << calculateTotalTime(path, "car") << " mins\n";
        } else {
            cout << "Estimated Travel Time (" << mode << "): " << calculateTotalTime(path, mode) << " mins\n";
        }
        cout << endl;
    }

    void displayTopRoutes(const vector<vector<size_t>>& allPaths, const string& mode) const {
        vector<pair<vector<size_t>, double>> routesWithTime;
        for (const auto& path : allPaths) {
            double time = 0;
            for (size_t idx : path) {
                time += routes[idx].calculateTime(mode);
            }
            routesWithTime.emplace_back(path, time);
        }
        sort(routesWithTime.begin(), routesWithTime.end(),
             [](const auto& a, const auto& b) { return a.second < b.second; });

        size_t displayCount = min(static_cast<size_t>(2), routesWithTime.size());
        cout << "\n==================== AVAILABLE ROUTES ====================\n";
        for (size_t i = 0; i < displayCount; ++i) {
            cout << "\nRoute Option " << i + 1 << ":\n";
            cout << "--------------------------------------------------\n";
            displayRoute(routesWithTime[i].first, mode, true);
        }
    }

    void recommendBestRoute(const vector<vector<size_t>>& allPaths, const string& mode) const {
        struct RouteInfo {
            vector<size_t> path;
            double time;
            double distance;
            bool construction;
        };

        vector<RouteInfo> noConstructionRoutes;
        vector<RouteInfo> constructionRoutes;

        for (const auto& path : allPaths) {
            double time = 0;
            double distance = 0;
            bool underConstruction = false;
            for (size_t idx : path) {
                const Route& r = routes[idx];
                time += r.calculateTime(mode);
                distance += r.getDistance();
                if (r.hasConstruction()) underConstruction = true;
            }
            RouteInfo info{path, time, distance, underConstruction};
            if (underConstruction)
                constructionRoutes.push_back(info);
            else
                noConstructionRoutes.push_back(info);
        }

        // Sort no-construction routes by time, then distance
        sort(noConstructionRoutes.begin(), noConstructionRoutes.end(),
             [](const RouteInfo& a, const RouteInfo& b) {
                 if (a.time != b.time) return a.time < b.time;
                 return a.distance < b.distance;
             });

        // Sort construction routes by time
        sort(constructionRoutes.begin(), constructionRoutes.end(),
             [](const RouteInfo& a, const RouteInfo& b) {
                 return a.time < b.time;
             });

        cout << "\n==================== RECOMMENDATION ====================\n";
        if (!noConstructionRoutes.empty()) {
            cout << "\n* Best Route (No Construction):\n";
            displayRoute(noConstructionRoutes.front().path, mode, false);
        } else if (!constructionRoutes.empty()) {
            cout << "\n! All routes have construction! Showing fastest available:\n";
            displayRoute(constructionRoutes.front().path, mode, false);
        } else {
            cout << "\nNo available routes found!\n";
        }
    }

private:
    double calculateTotalTime(const vector<size_t>& path, const string& mode) const {
        double total = 0;
        for (size_t idx : path) {
            total += routes[idx].calculateTime(mode);
        }
        return total;
    }
};

class UserInputHandler {
    RouteManager& manager;
    int getValidatedNumber(int min, int max, const string& prompt) {
        int choice;
        while (true) {
            cout << prompt;
            if (!(cin >> choice) || choice < min || choice > max) {
                cout << "Invalid input. Please enter a number between " << min << " and " << max << ".\n";
                cin.clear();
                char ch;
                while ((ch = cin.get()) != '\n' && ch != EOF) {}
            } else {
                // Do NOT clear buffer here, to avoid interfering with getline
                return choice;
            }
        }
    }

    string getValidatedMode() {
        string mode;
        while (true) {
            cout << "Enter travel mode (walk/bike/car): ";
            getline(cin, mode);
            if (mode == "walk" || mode == "bike" || mode == "car") {
                return mode;
            } else {
                cout << "Invalid travel mode. Please enter 'walk', 'bike', or 'car'.\n";
            }
        }
    }

    bool askYesNo(const string& question) {
        string response;
        while (true) {
            cout << question << " (yes/no): ";
            getline(cin, response);
            if (response == "yes" || response == "y") return true;
            if (response == "no" || response == "n") return false;
            cout << "Please answer 'yes' or 'no'.\n";
        }
    }

public:
    UserInputHandler(RouteManager& rm) : manager(rm) {}

    void run() {
        const auto& locs = manager.getLocations();
        if (locs.empty()) {
            cout << "No locations available.\n";
            return;
        }

        cout << "==================== USER DRIVEN ROUTE FINDER / OPTIMIZER ====================\n\n";
        
        do {
            cout << "Available Locations:\n";
            for (size_t i = 0; i < locs.size(); ++i) {
                cout << "  " << (i + 1) << ". " << locs[i].getName() << "\n";
            }

            int startIndex = getValidatedNumber(1, static_cast<int>(locs.size()), "Enter start location number: ");
            int endIndex;
            do {
                endIndex = getValidatedNumber(1, static_cast<int>(locs.size()), "Enter destination number: ");
                if (endIndex == startIndex) {
                    cout << "Destination cannot be the same as start location. Please enter again.\n";
                }
            } while (endIndex == startIndex);

            // Clear input buffer before getline for travel mode
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            string mode = getValidatedMode();

            auto allPaths = manager.findAllPaths(static_cast<size_t>(startIndex - 1), static_cast<size_t>(endIndex - 1));

            if (allPaths.empty()) {
                cout << "No routes found!\n";
            } else {
                manager.displayTopRoutes(allPaths, mode);
                manager.recommendBestRoute(allPaths, mode);
            }

        } while (askYesNo("Do you want to find routes for another trip?"));

        cout << "Thank you for using the route finder. Goodbye!\n";
    }
};

int main() {
    RouteManager manager;

    vector<Location> locations = {
        Location("Home"),
        Location("School"),
        Location("Mall"),
        Location("Park"),
        Location("Hospital"),
        Location("Office")
    };

    for (const auto& loc : locations) manager.addLocation(loc);

    vector<Route> routes = {
        // Home <-> School
        Route(0, 1, 3, 1, Traffic::LOW, false),
        Route(1, 0, 3, 1, Traffic::LOW, false),

        // School <-> Mall
        Route(1, 2, 2, 0, Traffic::MEDIUM, true),
        Route(2, 1, 2, 0, Traffic::MEDIUM, true),

        // Mall <-> Park
        Route(2, 3, 1.5, 2, Traffic::HIGH, false),
        Route(3, 2, 1.5, 2, Traffic::HIGH, false),

        // Park <-> Hospital
        Route(3, 4, 4, 1, Traffic::LOW, true),
        Route(4, 3, 4, 1, Traffic::LOW, true),

        // Hospital <-> Office
        Route(4, 5, 5, 0, Traffic::MEDIUM, false),
        Route(5, 4, 5, 0, Traffic::MEDIUM, false),

        // Home <-> Mall (direct)
        Route(0, 2, 4.2, 0, Traffic::MEDIUM, false),
        Route(2, 0, 4.2, 0, Traffic::MEDIUM, false),

        // Mall <-> Office
        Route(2, 5, 3.8, 1, Traffic::HIGH, true),
        Route(5, 2, 3.8, 1, Traffic::HIGH, true),

        // School <-> Park
        Route(1, 3, 2.5, 0, Traffic::LOW, false),
        Route(3, 1, 2.5, 0, Traffic::LOW, false),

        // Park <-> Office
        Route(3, 5, 2.2, 1, Traffic::MEDIUM, false),
        Route(5, 3, 2.2, 1, Traffic::MEDIUM, false),

        // Home <-> Hospital
        Route(0, 4, 5.5, 0, Traffic::HIGH, true),
        Route(4, 0, 5.5, 0, Traffic::HIGH, true)
    };

    for (const auto& route : routes) manager.addRoute(route);

    UserInputHandler handler(manager);
    handler.run();

    return 0;
}
