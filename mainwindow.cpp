#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>
#include <stdlib.h>
#include <QPixmap>
#define NUM_CITIES 10 // number of cities

int source,destination,result=0;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QPixmap pix("C:/Users/SANDESH/Desktop/Notes SEM III/ADS/ADS CP/flight.png");
    int w=ui->label_3->width();
    int h=ui->label_3->height();
    ui->label_3->setPixmap(pix.scaled(w,h,Qt::KeepAspectRatio));
    QPixmap pix1("C:/Users/SANDESH/Desktop/Notes SEM III/ADS/ADS CP/cloud.png");
    ui->label_2->setPixmap(pix1);
    ui->comboBox->addItem("Mumbai");
    ui->comboBox->addItem("Pune");
    ui->comboBox->addItem("Kolkata");
    ui->comboBox->addItem("Bengaluru");
    ui->comboBox->addItem("New Delhi");
    ui->comboBox->addItem("San Francisco");
    ui->comboBox->addItem("Dubai");
    ui->comboBox->addItem("Beijing");
    ui->comboBox->addItem("Singapore");
    ui->comboBox->addItem("Paris");
    ui->comboBox2->addItem("Mumbai");
    ui->comboBox2->addItem("Pune");
    ui->comboBox2->addItem("Kolkata");
    ui->comboBox2->addItem("Bengaluru");
    ui->comboBox2->addItem("New Delhi");
    ui->comboBox2->addItem("San Francisco");
    ui->comboBox2->addItem("Dubai");
    ui->comboBox2->addItem("Beijing");
    ui->comboBox2->addItem("Singapore");
    ui->comboBox2->addItem("Paris");

}


struct FlightConnection
{
    int destination;
    int distance;
    int cost;
    int duration;
};

// Function to print shortest path from source to destination
void printShortestPath(int parent[], int j, char cities[NUM_CITIES][20], int distances[], int costs[], int durations[], char *output)
{
    if (parent[j] == -1)
        return;

    printShortestPath(parent, parent[j], cities, distances, costs, durations, output);
    char temp[512]; // Temporary buffer for building the output
    sprintf(temp, " ->%s (Distance: %d km, Cost: Rs%d, Duration: %d hours)\n", cities[j], distances[j], costs[j], durations[j]);
    strcat(output, temp);
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
void dijkstra(struct FlightConnection *graph[], int num_vertices, int src, int dest, char cities[NUM_CITIES][20], int priority, char *output)
{
    int distances[NUM_CITIES];
    int costs[NUM_CITIES];
    int durations[NUM_CITIES];
    bool sptSet[NUM_CITIES];
    int parent[NUM_CITIES];
    int citiesVisited = 0;
    int i, count, v;

    for (i = 0; i < NUM_CITIES; i++)
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

    for (count = 0; count < NUM_CITIES - 1; count++)
    {
        int u = -1;
        for (v = 0; v < num_vertices; v++)
        {
            if (!sptSet[v] && (u == -1 || getPriorityValue(priority, distances[v], costs[v], durations[v]) < getPriorityValue(priority, distances[u], costs[u], durations[u])))
                u = v;
        }

        sptSet[u] = true;

        for (v = 0; v < num_vertices; v++)
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

        for (i = 0; i < NUM_CITIES; i++)
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
            char temp[512]; // Temporary buffer for building the output
            sprintf(temp, "Connecting flight is available between %s and %s via %s:\n", cities[src], cities[dest], cities[connectingCity]);
            strcat(output, temp);
            printShortestPath(parent, connectingCity, cities, distances, costs, durations, output);
            sprintf(temp, " -> %d. %s (Distance: %d km, Cost: Rs%d, Duration: %d hours)\n", dest + 1, cities[dest], distances[dest], costs[dest], durations[dest]);
            strcat(output, temp);
        }
        else
        {
            char temp[512]; // Temporary buffer for building the output
            sprintf(temp, "No direct or connecting flight found between %s and %s.\n", cities[src], cities[dest]);
            strcat(output, temp);
        }
    }
    else if (citiesVisited > 1)
    {
        char temp[512]; // Temporary buffer for building the output
        sprintf(temp, "Connecting flight is available between %s and %s via %d cities:\n", cities[src], cities[dest], citiesVisited - 1);
        strcat(output, temp);
        sprintf(temp, "Shortest path from %s to %s (Distance: %d km, Cost: Rs%d, Duration: %d hours):\n", cities[src], cities[dest], distances[dest], costs[dest], durations[dest]);
        strcat(output, temp);
        printShortestPath(parent, dest, cities, distances, costs, durations, output);
        strcat(output, "\n");
    }
    else
    {
        char temp[512]; // Temporary buffer for building the output
        sprintf(temp, "Direct flight is available between %s and %s (Distance: %d km, Cost: Rs%d, Duration: %d hours):\n", cities[src], cities[dest], distances[dest], costs[dest], durations[dest]);
        strcat(output, temp);
        sprintf(temp, "Shortest path from %s to %s:\n", cities[src], cities[dest]);
        strcat(output, temp);
        printShortestPath(parent, dest, cities, distances, costs, durations, output);
        strcat(output, "\n");
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_comboBox_currentIndexChanged(int index)
{
    source=index;
}

void MainWindow::on_comboBox2_currentIndexChanged(int index)
{
    destination=index;
}

void MainWindow::on_pushButton_2_clicked()
{
    char cities[NUM_CITIES][20] = {"Mumbai", "Pune", "Kolkata", "Bengaluru", "New Delhi",
                                   "San Francisco", "Dubai", "Beijing", "Singapore", "Paris"};
    int i, j;

    struct FlightConnection *flights[NUM_CITIES];

    for (i = 0; i < NUM_CITIES; i++)
    {
        flights[i] = (struct FlightConnection *)malloc(sizeof(struct FlightConnection) * NUM_CITIES);
        for (j = 0; j < NUM_CITIES; j++)
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

    // Create a buffer for the output
    char output[4096]; // Adjust the size as needed

    // Call the dijkstra function with the output buffer
    dijkstra(flights, NUM_CITIES,source,destination, cities, 1, output);

    for (i = 0; i < NUM_CITIES; i++)
    {
        free(flights[i]);
    }
    ui->label->setText(output);

}



void MainWindow::on_pushButton_3_clicked()
{
    char cities[NUM_CITIES][20] = {"Mumbai", "Pune", "Kolkata", "Bengaluru", "New Delhi",
                                   "San Francisco", "Dubai", "Beijing", "Singapore", "Paris"};
    int i, j;

    struct FlightConnection *flights[NUM_CITIES];

    for (i = 0; i < NUM_CITIES; i++)
    {
        flights[i] = (struct FlightConnection *)malloc(sizeof(struct FlightConnection) * NUM_CITIES);
        for (j = 0; j < NUM_CITIES; j++)
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

    // Create a buffer for the output
    char output[4096]; // Adjust the size as needed

    // Call the dijkstra function with the output buffer
    dijkstra(flights, NUM_CITIES,source,destination, cities, 2, output);

    for (i = 0; i < NUM_CITIES; i++)
    {
        free(flights[i]);
    }
    ui->label->setText(output);

}


void MainWindow::on_pushButton_clicked()
{
    char cities[NUM_CITIES][20] = {"Mumbai", "Pune", "Kolkata", "Bengaluru", "New Delhi",
                                   "San Francisco", "Dubai", "Beijing", "Singapore", "Paris"};
    int i, j;

    struct FlightConnection *flights[NUM_CITIES];

    for (i = 0; i < NUM_CITIES; i++)
    {
        flights[i] = (struct FlightConnection *)malloc(sizeof(struct FlightConnection) * NUM_CITIES);
        for (j = 0; j < NUM_CITIES; j++)
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

    // Create a buffer for the output
    char output[4096]; // Adjust the size as needed

    // Call the dijkstra function with the output buffer
    dijkstra(flights, NUM_CITIES,source,destination, cities, 3, output);

    for (i = 0; i < NUM_CITIES; i++)
    {
        free(flights[i]);
    }
    ui->label->setText(output);

}

