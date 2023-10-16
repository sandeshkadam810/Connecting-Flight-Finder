#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>
#include <stdlib.h>

#define NUM_CITIES 10 // number of cities

// Structure to represent flight connection
struct FlightConnection
{
    int destination;
    int distance;
    int cost;
    int duration;
};

// Function to print shortest path from source to destination
void printShortestPath(int parent[], int j, char cities[NUM_CITIES][20], int distances[], int costs[], int durations[])
{
    if (parent[j] == -1)
        return;

    printShortestPath(parent, parent[j], cities, distances, costs, durations);
    printf(" -> %d. %s (Distance: %d km, Cost: Rs%d, Duration: %d hours)\n", j + 1, cities[j], distances[j], costs[j], durations[j]);
}

// Priority function based on time duration
int getPriorityValue(int priority, int distance, int cost, int duration)
{
    if (priority == 1)
    {
        return distance;
    }
    else if (priority == 2)
    {
        return cost;
    }
    else if (priority == 3)
    {
        return duration;
    }
    else
    {
        return INT_MAX; // Handle invalid priority
    }
}

// Function to find the shortest path
void dijkstra(struct FlightConnection *graph[], int num_vertices, int src, int dest, char cities[NUM_CITIES][20], int priority)
{
    int distances[NUM_CITIES];
    int costs[NUM_CITIES];
    int durations[NUM_CITIES];
    bool sptSet[NUM_CITIES];
    int parent[NUM_CITIES];
    int citiesVisited = 0;

    for (int i = 0; i < NUM_CITIES; i++)
    {
        distances[i] = INT_MAX;
        costs[i] = INT_MAX;
        durations[i] = INT_MAX;
        sptSet[i] = false;
    }

    distances[src] = 0;
    costs[src] = 0;
    durations[src] = 0;
    parent[src] = -1;

    for (int count = 0; count < NUM_CITIES - 1; count++)
    {
        int u = -1;
        for (int v = 0; v < num_vertices; v++)
        {
            if (!sptSet[v] && (u == -1 || getPriorityValue(priority, distances[v], costs[v], durations[v]) < getPriorityValue(priority, distances[u], costs[u], durations[u])))
                u = v;
        }

        sptSet[u] = true;

        for (int v = 0; v < num_vertices; v++)
        {
            if (!sptSet[v] && graph[u][v].distance != INT_MAX && distances[u] != INT_MAX && durations[u] + graph[u][v].duration < durations[v])
            {
                distances[v] = distances[u] + graph[u][v].distance;
                costs[v] = costs[u] + graph[u][v].cost;
                durations[v] = durations[u] + graph[u][v].duration;
                parent[v] = u;
            }
        }
    }

    int currentCity = dest;
    while (currentCity != src)
    {
        currentCity = parent[currentCity];
        citiesVisited++;
    }

    if (distances[dest] == INT_MAX)
    {
        int connectingCity = -1;
        int minDist = INT_MAX;

        for (int i = 0; i < NUM_CITIES; i++)
        {
            if (i != src && i != dest)
            {
                if (distances[i] != INT_MAX && graph[i][dest].distance != INT_MAX && distances[i] + graph[i][dest].distance < minDist)
                {
                    minDist = distances[i] + graph[i][dest].distance;
                    connectingCity = i;
                }
            }
        }

        if (connectingCity != -1)
        {
            printf("Connecting flight is available between %s and %s via %s:\n", cities[src], cities[dest], cities[connectingCity]);
            printShortestPath(parent, connectingCity, cities, distances, costs, durations);
            printf(" -> %d. %s (Distance: %d km, Cost: Rs%d, Duration: %d hours)\n", dest + 1, cities[dest], distances[dest], costs[dest], durations[dest]);
        }
        else
        {
            printf("No direct or connecting flight found between %s and %s.\n", cities[src], cities[dest]);
        }
    }
    else if (citiesVisited > 1)
    {
        printf("Connecting flight is available between %s and %s via %d cities:\n", cities[src], cities[dest], citiesVisited - 1);
        printf("Shortest path from %s to %s (Distance: %d km, Cost: Rs%d, Duration: %d hours):\n", cities[src], cities[dest], distances[dest], costs[dest], durations[dest]);
        printShortestPath(parent, dest, cities, distances, costs, durations);
        printf("\n");
    }
    else
    {
        printf("Direct flight is available between %s and %s (Distance: %d km, Cost: Rs%d, Duration: %d hours):\n", cities[src], cities[dest], distances[dest], costs[dest], durations[dest]);
        printf("Shortest path from %s to %s:\n", cities[src], cities[dest]);
        printShortestPath(parent, dest, cities, distances, costs, durations);
        printf("\n");
    }
}

int main()
{
    char cities[NUM_CITIES][20] = {"Mumbai", "Pune", "Kolkata", "Bengaluru", "New Delhi",
                                   "San Francisco", "Dubai", "Beijing", "Singapore", "Paris"};

    struct FlightConnection *flights[NUM_CITIES];

    for (int i = 0; i < NUM_CITIES; i++)
    {
        flights[i] = (struct FlightConnection *)malloc(sizeof(struct FlightConnection) * NUM_CITIES);
        for (int j = 0; j < NUM_CITIES; j++)
        {
            flights[i][j].destination = j;
            if (i == j)
            {
                flights[i][j].distance = 0;
                flights[i][j].cost = 0;
                flights[i][j].duration = 0;
            }
            else
            {
                flights[i][j].distance = INT_MAX;
                flights[i][j].cost = INT_MAX;
                flights[i][j].duration = INT_MAX;
            }
        }
    }

    // Set distances, costs, and durations for direct flights (both directions)
    flights[0][1].distance = 148;
    flights[1][0].distance = 148;
    flights[0][1].cost = 1700;
    flights[1][0].cost = 1700;
    flights[0][1].duration = 1;
    flights[1][0].duration = 1;

    flights[0][6].distance = 1934;
    flights[6][0].distance = 1934;
    flights[0][6].cost = 10000;
    flights[6][0].cost = 10000;
    flights[0][6].duration = 4;
    flights[6][0].duration = 4;

    flights[0][8].distance = 3910;
    flights[8][0].distance = 3910;
    flights[0][8].cost = 11000;
    flights[8][0].cost = 11000;
    flights[0][8].duration = 6;
    flights[8][0].duration = 6;

    flights[0][3].distance = 800;
    flights[3][0].distance = 800;
    flights[0][3].cost = 3500;
    flights[3][0].cost = 3500;
    flights[0][3].duration = 3;
    flights[3][0].duration = 3;


    flights[1][3].distance = 753;
    flights[3][1].distance = 753;
    flights[1][3].cost = 4000;
    flights[3][1].cost = 4000;
    flights[1][3].duration = 3;
    flights[3][1].duration = 3;

    flights[1][4].distance = 1175;
    flights[4][1].distance = 1175;
    flights[1][4].cost = 5100;
    flights[4][1].cost = 5100;
    flights[1][4].duration = 3;
    flights[4][1].duration = 3;

    flights[0][4].distance = 1175;
    flights[4][0].distance = 1175;
    flights[0][4].cost = 4100;
    flights[4][0].cost = 4100;
    flights[0][4].duration = 3;
    flights[4][0].duration = 3;

    flights[1][2].distance = 1575;
    flights[2][1].distance = 1575;
    flights[1][2].cost = 8000;
    flights[2][1].cost = 8000;
    flights[1][2].duration = 4;
    flights[2][1].duration = 4;

    flights[4][6].distance = 2202;
    flights[6][4].distance = 2202;
    flights[4][6].cost = 14000;
    flights[6][4].cost = 14000;
    flights[4][6].duration = 9;
    flights[6][4].duration = 9;

    flights[4][7].distance = 3776;
    flights[7][4].distance = 3776;
    flights[4][7].cost = 30000;
    flights[7][4].cost = 30000;
    flights[4][7].duration = 15;
    flights[7][4].duration = 15;

    flights[2][7].distance = 3266;
    flights[7][2].distance = 3266;
    flights[2][7].cost = 21000;
    flights[7][2].cost = 21000;
    flights[2][7].duration = 12;
    flights[7][2].duration = 12;

    flights[3][8].distance = 3155;
    flights[8][3].distance = 3155;
    flights[3][8].cost = 10000;
    flights[8][3].cost = 10000;
    flights[3][8].duration = 5;
    flights[8][3].duration = 5;

    flights[7][5].distance = 9501;
    flights[5][7].distance = 9501;
    flights[7][5].cost = 48000;
    flights[5][7].cost = 48000;
    flights[7][5].duration = 22;
    flights[5][7].duration = 22;

    flights[6][9].distance = 5244;
    flights[9][6].distance = 5244;
    flights[6][9].cost = 35000;
    flights[9][6].cost = 35000;
    flights[6][9].duration = 19;
    flights[9][6].duration = 19;

    flights[7][8].distance = 4479;
    flights[8][7].distance = 4479;
    flights[7][8].cost = 30000;
    flights[8][7].cost = 30000;
    flights[7][8].duration = 6;
    flights[8][7].duration = 6;

  

    while (true)
    {
        int source, destination, priority;
        printf("Cities:\n");
        for (int i = 0; i < NUM_CITIES; i++)
        {
            printf("%d. %s\n", i + 1, cities[i]);
        }
        printf("Enter source city (1-%d): ", NUM_CITIES);
        scanf("%d", &source);
        printf("Enter destination city (1-%d): ", NUM_CITIES);
        scanf("%d", &destination);
        printf("Enter priority (1 for distance, 2 for cost, 3 for duration): ");
        scanf("%d", &priority);

        source--;
        destination--;

        if (source >= 0 && source < NUM_CITIES && destination >= 0 && destination < NUM_CITIES && (priority >= 1 && priority <= 3))
        {
            dijkstra(flights, NUM_CITIES, source, destination, cities, priority);
        }
        else
        {
            printf("Invalid city selection or priority.\n");
        }
    }

    for (int i = 0; i < NUM_CITIES; i++)
    {
        free(flights[i]);
    }

    return 0;
}