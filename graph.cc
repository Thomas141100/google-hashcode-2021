#include "graph.hh"

#include <string>
#include <vector>

/*
bool Street::operator==(const Street &other)
{
    return (this->src == other.src && this->dst == other->dst
            && this->time == other->time && this->name.compare(other->name))
}

bool Stree::operator!=(const Street &other){
    return (this->src != other->src || this->dst != other->dst
            || this->time != other->time || this->name.compare(other->name))
}
*/

Graph::Graph(int nb_intersection)
{
    this->nb_intersection = nb_intersection; // note that index = src + dst *
                                             // (num_intersection + 1)
    this->adj_mat = std::vector<int>(nb_intersection * nb_intersection, -1);
}

void Graph::add_street(Street &new_street)
{
    this->street_list.push_back(new_street);
    this->adj_mat[new_street.src + new_street.dst * (this->nb_intersection)] =
        new_street.time;
}

int Graph::street_count(int intersect_index)
{
    int street_count = 0;

    for (size_t dst = 0; dst < this->nb_intersection; dst++)
    {
        if (adj_mat[(this->nb_intersection) * dst + intersect_index] != -1)
            street_count++;
    }

    return street_count;
}

std::vector<Street> Graph::street_to_intersect(int intersect)
{
    std::vector<Street> out;

    for (Street street : street_list)
    {
        if (street.dst == intersect)
            out.push_back(street);
    }

    return out;
}
