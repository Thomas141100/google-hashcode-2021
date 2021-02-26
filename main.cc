#include <fstream>
#include <iostream>
#include <map>
#include <string>

#include "graph.hh"
#include "parser.hh"

void update(Simulation simulation,
            std::map<std::string, std::vector<int>> traffic_light, int time)
{
    for (auto street : simulation.streets)
    {
        Car car = street.cars[0];
        if (traffic_light.find(street.name)->second[time] == 1)
        {
            car.streets.erase(car.streets.begin());
            car.availability = car.streets[0].time + 1;
        }
    }
    for (auto c : simulation.cars)
    {
        c.availability--;
    }
}

void freq_calc(int intersect,
               std::map<std::string, std::vector<int>> traffic_light,
               std::ofstream &file_out, Graph graph, Simulation simulation)
{
    file_out << intersect << '\n';
    std::vector<Street> streets = graph.street_to_intersect(intersect);
    int nb_street = streets.size();
    file_out << nb_street << '\n';
    if (nb_street == 1)
        file_out << streets[0].name << ' ' << simulation.duration << '\n';

    for (auto street : streets)
    {
        int sum_passage = 0;
        std::vector<int> traffic = traffic_light.find(street.name)->second;
        for (int i = 0; i < simulation.duration; i++)
        {
            sum_passage += traffic[i];
        }
        file_out << street.name << ' ' << sum_passage << '\n';
    }
}

Street *max_intersect(int intersect_index, Simulation simulation)
{
    Street *max_street = nullptr;
    int max_time = 0;

    std::vector<Street> street_list =
        simulation.graph.street_to_intersect(intersect_index);

    if (street_list.size() == 0)
        return max_street;

    for (size_t index = 0; index < street_list.size(); index++)
    {
        int m = 0;
        int i = 0;

        for (Car car : street_list[index].cars)
        {
            if (car.availability > 0)
                continue;
            m = std::max(street_list[index].time + i, m);
            i++;
        }
        if (max_time < m)
        {
            max_street = &street_list[index];
            max_time = m;
        }
    }

    return max_street;
}

int main(int argc, char *argv[])
{
    if (argc < 2)
        return 0;

    std::cout << "Start parsing" << std::endl;
    Simulation simulation = parser(argv[1]);
    std::cout << "Finish parsing" << std::endl;

    std::cout << "Start insert" << std::endl;
    std::map<std::string, std::vector<int>> allume;
    for (int i = 0; i < simulation.street_count; i++)
    {
        std::vector<int> base_vec(simulation.duration, 0);
        allume.insert({ simulation.streets[i].name, base_vec });
    }

    std::cout << "Finish insert" << std::endl;

    int temps = 0;
    while (temps < simulation.duration)
    {
        std::cout << "Temps: " << temps << std::endl;
        for (int i = 0; i < simulation.intersection_count; i++)
        {
            std::cout << "Intersect count: " << i << std::endl;
            Street *street = max_intersect(i, simulation);
            if (street == nullptr)
                continue;
            allume[street->name][temps] = 1;
        }
        update(simulation, allume, temps);
        temps++;
    }

    std::ofstream file_out;
    file_out.open("result");
    for (int j = 0; j < simulation.intersection_count; j++)
    {
        freq_calc(j, allume, file_out, simulation.graph, simulation);
    }
    file_out.close();
    return 0;
}
