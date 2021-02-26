#pragma once
#include <list>
#include <queue>
#include <stddef.h>
#include <string>
#include <vector>

class Car;

class Street
{
public:
    Street()
    {}

    // attributes
    int src;
    int dst;
    std::string name;
    int time;
    std::vector<Car> cars;

    // operators
    bool operator==(const Street &other);
    bool operator!=(const Street &other);
};

class Car
{
public:
    Car()
    {}

    Car(std::vector<Street> streets)
        : streets(streets)
    {
        location = streets[0].dst;
        availability = 0;
    }
    int location;
    std::vector<Street> streets;
    int availability;
};

class Graph
{
public:
    Graph()
    {}
    Graph(int num_intersection);
    void add_street(Street &new_street);
    int street_count(int intersect_index);
    std::vector<Street> street_to_intersect(int intersect);

private:
    size_t nb_intersection;
    std::vector<int> adj_mat;
    std::vector<Street> street_list;
};
