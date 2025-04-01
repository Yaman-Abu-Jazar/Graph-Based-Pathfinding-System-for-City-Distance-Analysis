//Yaman abu jazar
//Radi jarrar
//Section 4
//1212199
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CITIES 100
#define INFINITY 9999

// Structure to represent the name of the city
typedef struct City {
    char name[MAX_CITIES];
} City;

// Structure to represent the edge between nodes in the graph
typedef struct Node {
    int cityIndex;
    int distance;
    struct Node* next;
} Node;

// Structure to represent the graph of the map
typedef struct {
    Node* head;
} AdjList;

// Global Variables that i used in the functions
City cities[MAX_CITIES];
AdjList adjacencyList[MAX_CITIES];
int numCities = 0;

// Function used in the code
void loadCitiesFromFile(char* filename);

int getCityIndex(char* cityName);

void dijkstra(int source, int destination,FILE*file);

void bfs(int source, int destination,FILE *file);

void saveShortestDistanceToFile(int source, int destination,FILE*file);

int main() {

    // Declare the name of the file
    char filename[50];
    int sourceIndex = -1;
    int destinationIndex = -1;
    int menuChoice=0;
    printf("Shortest Path Finder\n");
    printf("--------------------\n");

    // Read the file name from the user
    printf("Enter the filename to load the cities data: ");
    scanf("%s", filename);

    FILE* file = fopen("shortest_distance.txt", "w");

    while (menuChoice!=5) {
            // Show the menu of the program
        printf("\nMenu:\n");
        printf("1. Load cities\n");
        printf("2. Enter source\n");
        printf("3. Enter destination\n");
        printf("4. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &menuChoice);// Read the choice from the user

        switch (menuChoice) {
        case 1: {
            // Load cities from the file and construct the graph
              loadCitiesFromFile(filename);
              printf("Cities are loaded.\n");
            break;
        }
        case 2: {
            // Read the source city from user
            char sourceCity[50];
            printf("Enter the source city: ");
            scanf("%s", sourceCity);
            sourceIndex = getCityIndex(sourceCity);

            // Check if the source city is exist or not
            if (sourceIndex == -1) {
                printf("Source city not found.\n");
            } else {
                printf("Source city set to %s.\n", cities[sourceIndex].name);
            }
            break;
        }
        case 3: {
            // Read the destination city from the user
            char destinationCity[50];
            printf("Enter the destination city: ");
            scanf("%s", destinationCity);
            destinationIndex = getCityIndex(destinationCity);

            // Check if the destination city is exist or not
            if (destinationIndex == -1) {
                printf("Destination city not found.\n");
            } else {
                printf("Destination city set to %s.\n", cities[destinationIndex].name);
            }
            // Find the dijkstra shortest path and BFS shortest path
            if (sourceIndex != -1 && destinationIndex != -1) {
                saveShortestDistanceToFile(sourceIndex, destinationIndex,file);
                sourceIndex = -1;
                destinationIndex = -1;
            }
            break;
        }

        case 4: {
            // Save the information to the output file and Exit the program
            if (sourceIndex != -1 && destinationIndex != -1) {
                saveShortestDistanceToFile(sourceIndex, destinationIndex,file);
            }
            printf("Shortest path information saved to 'shortest_distance.txt'.\n");
            printf("Exiting the program.\n");
            menuChoice=5;
            break;
        }
        default: {
            // If the user entered an invalid choice
            printf("Invalid choice. Please try again.\n");
            break;
        }
        }
    }
    // close the file
    fclose(file);

    return 0;
}

// Function to load cities from the Cities.txt file and construct the graph
void loadCitiesFromFile(char* filename) {

    // declare the input file
    FILE* file = fopen(filename, "r");
    if (file == NULL) {// if the file is not exist
        printf("Erroropening file '%s'.\n", filename);
        exit(0);
     }
    // declare local variables to read the information from the file
    char source[50], destination[50];
    int distance;

    while (fscanf(file, "%s %s %dkm", source, destination, &distance) == 3) {// Read the information from the file and store them in the variables
        int sourceIndex = -1, destinationIndex = -1;
        // Check if the city is added before or not
        for (int i = 0; i < numCities; i++) {
            if (strcmp(cities[i].name, source) == 0) {
                sourceIndex = i;
            }
            if (strcmp(cities[i].name, destination) == 0) {
                destinationIndex = i;
            }
        }
        // Add the source city
        if (sourceIndex == -1) {
            strcpy(cities[numCities].name, source);
            sourceIndex = numCities;
            numCities++;
        }
        // Add the destination city
        if (destinationIndex == -1) {
            strcpy(cities[numCities].name, destination);
            destinationIndex = numCities;
            numCities++;
        }
        // declare a new node and store the city in the graph
        Node* newNode = malloc(sizeof(Node));
        newNode->cityIndex = destinationIndex;
        newNode->distance = distance;
        newNode->next = adjacencyList[sourceIndex].head;
        adjacencyList[sourceIndex].head = newNode;
    }
    // close the file
    fclose(file);
}
// Function returns the index of the city
int getCityIndex(char* cityName) {
    for (int i = 0; i < numCities; i++) {
        if (strcmp(cities[i].name, cityName) == 0) {
            return i;
        }
    }
    return -1;
}
// Function Dijkstra to find the shortest path from a city to another
void dijkstra(int source, int destination,FILE *file) {
    // Variables used to determine the information about the nodes that we visit
    int visited[MAX_CITIES] = {0};
    int distances[MAX_CITIES];
    int parent[MAX_CITIES];
    // Initializing the variables
    for (int i = 0; i < numCities; i++) {
        distances[i] = INFINITY;
        parent[i] = -1;
    }
    // Initialize the distance of the source
    distances[source] = 0;
    // Reaching to the destination from the source from shortest path
    for (int i = 0; i < numCities - 1; i++) {
        int minDistance = INFINITY;
        int minIndex = -1;

        for (int j = 0; j < numCities; j++) {
            if (!visited[j] && distances[j] < minDistance) {
                minDistance = distances[j];
                minIndex = j;
            }
        }

        if (minIndex == -1 || visited[destination]) {
            break;
        }

        visited[minIndex] = 1;

        Node* current = adjacencyList[minIndex].head;
        while (current != NULL) {
            int cityIndex = current->cityIndex;
            int newDistance = distances[minIndex] + current->distance;
            if (!visited[cityIndex] && current->distance != 0 && newDistance < distances[cityIndex]) {
                distances[cityIndex] = newDistance;
                parent[cityIndex] = minIndex;  // Update parent
            }
            current = current->next;
        }
    }
    // Check if the destination is found or not
    if (distances[destination] == INFINITY) {
        printf("No path found from %s to %s.\n", cities[source].name, cities[destination].name);
        return;
    }
    // Print the shortest path to the screen and to the output file
    printf("Shortest path using Dijkstra's algorithm from %s to %s:\n", cities[source].name, cities[destination].name);
    int current = destination;
    while (current != source) {
        printf("%s<-", cities[current].name);
        fprintf(file,"%s<-",cities[current].name);
        current = parent[current];  // Move to parent city
    }
    printf("%s", cities[source].name);
    fprintf(file,"%s",cities[source].name);

    printf("\nTotal distance: %d\n", distances[destination]);
    fprintf(file,"\nTotal distance: %d\n", distances[destination]);
}
// Function BFS to find the shortest path from a city to another
void bfs(int source, int destination,FILE *file) {
    // Variables used to determine the information about the nodes that we visit
    int visited[MAX_CITIES] = {0};
    int parent[MAX_CITIES] = {-1};
    int queue[MAX_CITIES];
    int front = -1, rear = -1;

    queue[++rear] = source;
    visited[source] = 1;
    // Reaching to the destination from the source from shortest path
    while (front != rear) {
        int current = queue[++front];

        if (current == destination) {
            break;
        }

        Node* currentNode = adjacencyList[current].head;
        while (currentNode != NULL) {
            int cityIndex = currentNode->cityIndex;

            if (!visited[cityIndex]) {
                queue[++rear] = cityIndex;
                visited[cityIndex] = 1;
                parent[cityIndex] = current;
            }

            currentNode = currentNode->next;
        }
    }

    // Check if the destination is found or not
    if (parent[destination] == -1) {
        printf("No path found from %s to %s.\n", cities[source].name, cities[destination].name);
        return;
    }
    // Print the shortest path to the screen and to the output file
    printf("Shortest path using BFS algorithm from %s to %s:\n", cities[source].name, cities[destination].name);
    printf("%s", cities[destination].name);
    fprintf(file,"%s", cities[destination].name);
    int current = destination;
    while (current != source) {
        printf("<-%s", cities[parent[current]].name);
        fprintf(file,"<-%s", cities[parent[current]].name);
        current = parent[current];
    }

    printf("\n");
}
// Function to save the information to the output file
void saveShortestDistanceToFile(int source, int destination,FILE*file) {
    if (file == NULL) {
        printf("Error creating file 'shortest_distance.txt'.\n");
        exit(1);
    }

    fprintf(file, "\nShortest path from %s to %s in dijkstra:\n", cities[source].name, cities[destination].name);

    dijkstra(source, destination,file);

    fprintf(file, "\nShortest path from %s to %s in bfs:\n", cities[source].name, cities[destination].name);

    bfs(source, destination,file);
}
