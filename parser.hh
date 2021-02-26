#pragma once
#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>

class Simulation
{
public:
    Simulation()
    {}

    friend std::ostream &operator<<(std::ostream &os, Simulation simu);

    int duration;
    int intersection_count;
    int street_count;
    int car_count;
    int bonus_point;

    std::vector<Street> streets;
    std::vector<Car> cars;
    Graph graph;
};
/*
std::ostream& operator<<(std::ostream& os, Simulation simulation)
{
    os << simulation.duration  << " " << simulation.intersection_count
        << " " << simulation.street_count  << " " << simulation.car_count  << "
" << simulation.bonus_point << std::endl;

    for (size_t i = 0; i < simulation.street_count; i++) {
        os << simulation.streets[i].src  << " " << simulation.streets[i].dst  <<
" " << simulation.streets[i].name  << " " << simulation.streets[i].time_ <<'\n';
    }

    for (size_t i = 0; i < simulation.car_count; i++) {
        os << simulation.cars[i].streets.size() << " ";
        for (size_t j = 0; j < simulation.cars[i].streets.size(); j++) {
            os << simulation.cars[i].streets[j].name << ' ';
        }
        os << '\n';
    }

    return os;
}
*/

Simulation parser(std::string filename)
{
    std::ifstream file_in(filename);
    std::string token;
    Simulation simulation;

    getline(file_in, token);
    sscanf(token.data(), "%i %i %i %i %i", &simulation.duration,
           &simulation.intersection_count, &simulation.street_count,
           &simulation.car_count, &simulation.bonus_point);

    simulation.graph = Graph(simulation.intersection_count);

    for (int i = 0; i < simulation.street_count; i++)
    {
        Street street;
        file_in >> street.src;
        file_in >> street.dst;
        file_in >> street.name;
        file_in >> street.time;

        simulation.streets.push_back(street);
    }

    for (int i = 0; i < simulation.car_count; i++)
    {
        int street_count;
        std::vector<Street> streets;

        file_in >> street_count;

        for (int i = 0; i < street_count; i++)
        {
            file_in >> token;
            for (auto it = simulation.streets.begin();
                 it < simulation.streets.end(); it++)
            {
                if (it->name == token)
                {
                    streets.push_back(*it);
                    simulation.graph.add_street(*(it));
                    break;
                }
            }
        }
        Car car(streets);
        simulation.cars.push_back(car);
        streets[0].cars.push_back(car);
    }

    return simulation;
}
