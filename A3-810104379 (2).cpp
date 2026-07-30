#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <map>
#define NotDelivered 0
#define Delivered 1
#define Transit 2
using namespace std;
struct Truck
{
    int id;
    int capacity;
    string loc = "Tehran";
    bool in_transit = false;
    string destination;
};
struct City
{
    string name;
    double distance;
    double sumoutgoingscores = 0;
    double sumincomingscores = 0;
    double score = 0;
};
struct Order
{
    string origin_city;
    string destination_city;
    int weight;
    int score = 0;
    int status = NotDelivered;
    int id = 0;
    int truck_id = -1;
    int cost = 0;
};
vector<Truck> readTrucks(const string &filename);
vector<City> readCities(const string &filename);
void add_order(int &numberoforder, vector<Order> &orders, Order new_order);
void status_order(int order_id, const vector<Order> &orders);
void get_order(int truck_id, string destination_city, vector<Order> &orders, vector<Truck> &trucks, int numberoforder);
void sort_orders(vector<Order *> &valid_orders);
void print_loaded(int truck_id, vector<int> &loaded_ids);
void deliver_order(int truck_id, vector<Order> &orders, vector<Truck> &trucks);
void print_deliver(vector<int> delivered_ids,int truck_id,bool found);
string calculated_city_score(vector<City> &cities, vector<Order> &orders);
int calculated_truck_score(vector<Truck> &trucks, vector<Order> &orders, vector<City> &cities);
void recommend_city_truck(vector<Order> &orders, vector<Truck> &trucks, vector<City> &cities);
int calculate_the_cost(vector<Order> &orders, vector<City> &cities);
void financial_report(vector<Order> &orders, vector<City> &cities);
void run_program(vector<Order> &orders, vector<Truck> &trucks, vector<City> &cities);
vector<Truck> readTrucks(const string &filename)
{
    vector<Truck> trucks;
    ifstream file(filename);
    if (!file.is_open())
    {
        cerr << "Error: failed to open file " << filename << endl;
        return trucks;
    }
    string line;
    getline(file, line);

    while (getline(file, line))
    {
        if (line.empty())
            continue;
        stringstream ss(line);
        string cell;
        vector<string> row;
        while (getline(ss, cell, ','))
        {
            row.push_back(cell);
        }
        if (row.size() >= 2)
        {
            Truck t;
            t.id = stoi(row[0]);
            t.capacity = stoi(row[1]);
            trucks.push_back(t);
        }
    }
    file.close();
    return trucks;
}
vector<City> readCities(const string &filename)
{
    vector<City> cities;
    ifstream file(filename);
    if (!file.is_open())
    {
        cerr << "Error: failed to open file " << filename << endl;
        return cities;
    }
    string line;
    getline(file, line);
    while (getline(file, line))
    {
        if (line.empty())
            continue;

        stringstream ss(line);
        string cell;
        vector<string> row;
        while (getline(ss, cell, ','))
        {
            row.push_back(cell);
        }
        if (row.size() >= 2)
        {
            City c;
            c.name = row[0];
            c.distance = stoi(row[1]);
            cities.push_back(c);
        }
    }
    file.close();
    return cities;
}
void add_order(int &numberoforder, vector<Order> &orders, Order new_order)
{
    numberoforder++;
    new_order.id = numberoforder;
    orders.push_back(new_order);
    cout << "Order " << numberoforder << " added" << endl;
}
void status_order(int order_id, const vector<Order> &orders)
{
    bool found = false;
    for (auto &order : orders)
    {
        if (order.id == order_id)
        {
            found = true;
            switch (order.status)
            {
            case NotDelivered:
                cout << "Order " << order_id << " is currently in warehouse in " << order.origin_city << endl;
                break;
            case Delivered:
                cout << "Order " << order_id << " is delivered to " << order.destination_city << endl;
                break;
            default:
                cout << "Order " << order_id << " is in transit to " << order.destination_city << endl;
                break;
            }
            break;
        }
    }
    if (!found)
    {
        const string Search = "Order not found";
        cout << Search << endl;
    }
}
void get_order(int truck_id, string destination_city, vector<Order> &orders, vector<Truck> &trucks, int numberoforder)
{
    bool found = false;
    int truck_capacity = 0;
    vector<Order *> valid_orders;
    Truck *truck = nullptr;
    for (auto &t : trucks)
    {
        if (t.id == truck_id)
        {
            truck = &t;
            break;
        }
    }
    string origin = truck->loc;
    for (auto &order : orders)
    {
        if (destination_city == order.destination_city && order.status == NotDelivered && order.origin_city == origin)
        {
            order.score = order.weight + (numberoforder - order.id) * 5;
            valid_orders.push_back(&order);
        }
    }
    for (int j = 0; j < trucks.size(); j++)
    {
        if (trucks[j].id == truck_id && !valid_orders.empty())
        {
            truck_capacity = trucks[j].capacity;
            trucks[j].in_transit = true;
            trucks[j].destination = destination_city;
            found = true;
            break;
        }
    }
    sort_orders(valid_orders);
    vector<int> loaded_ids;
    for (int k = 0; k < valid_orders.size(); k++)
    {
        if (truck_capacity >= valid_orders[k]->weight)
        {
            truck_capacity -= valid_orders[k]->weight;
            valid_orders[k]->status = Transit;
            valid_orders[k]->truck_id = truck_id;
            loaded_ids.push_back(valid_orders[k]->id);
        }
    }
    print_loaded(truck_id, loaded_ids);
}
void sort_orders(vector<Order *> &valid_orders)
{
    sort(valid_orders.begin(), valid_orders.end(),
         [](Order *a, Order *b)
         {
             if (a->score == b->score)
                 return a->id < b->id;
             return a->score > b->score;
         });
}
void print_loaded(int truck_id, vector<int> &loaded_ids)
{
    if (loaded_ids.empty())
    {
        cout << "No order could be loaded" << endl;
        return;
    }

    sort(loaded_ids.begin(), loaded_ids.end());

    cout << "Truck " << truck_id << " loaded with orders: ";
    for (size_t i = 0; i < loaded_ids.size(); i++)
    {
        cout << loaded_ids[i];
        if (i < loaded_ids.size() - 1)
            cout << " ";
    }
    cout << endl;
}
void deliver_order(int truck_id, vector<Order> &orders, vector<Truck> &trucks)
{
    bool found = false;
    string destination = "";
    vector<int> delivered_ids;
    for (auto &order : orders)
    {
        if (truck_id == order.truck_id && order.status == Transit)
        {
            found = true;
            order.status = Delivered;
            order.truck_id = -1;
            delivered_ids.push_back(order.id);
            destination = order.destination_city;
        }
    }
    if (!delivered_ids.empty())
    {
        for (auto &truck : trucks)
        {
            if (truck.id == truck_id)
            {
                truck.loc = destination;
                truck.in_transit = false;
                truck.destination = "";
                break;
            }
        }
    }
    print_deliver(delivered_ids, truck_id, found);
}
void print_deliver(vector<int> delivered_ids,int truck_id,bool found){
    if (!delivered_ids.empty())
    {
        sort(delivered_ids.begin(), delivered_ids.end());
        cout << "Truck " << truck_id << " delivered orders: ";
        for (int i = 0; i < delivered_ids.size(); i++)
        {
            cout << delivered_ids[i];
            if (i < delivered_ids.size() - 1)
            {
                cout << " ";
            }
        }
        cout << endl;
    }
    if (!found)
    {
        cout << "No orders to deliver in truck " << truck_id << endl;
    }
}
string calculated_city_score(vector<City> &cities, vector<Order> &orders)
{
    map<string, double> city;
    for (auto &city : cities)
    {
        city.sumoutgoingscores = 0;
        city.sumincomingscores = 0;
        city.score = 0;
    }
    for (int i = 0; i < cities.size(); i++)
    {
        for (int j = 0; j < orders.size(); j++)
        {
            if (orders[j].status == NotDelivered && orders[j].origin_city == cities[i].name && orders[j].destination_city == "Tehran")
            {
                int largest_id = orders.empty() ? 0 : orders.back().id;
                int order_score = orders[j].weight + (largest_id - orders[j].id) * 5;
                cities[i].sumoutgoingscores += order_score;
            }
        }
    }
    for (int i = 0; i < cities.size(); i++)
    {
        for (int j = 0; j < orders.size(); j++)
        {
            if (orders[j].status == NotDelivered && orders[j].destination_city == cities[i].name && orders[j].origin_city == "Tehran")
            {
                int largest_id = orders.empty() ? 0 : orders.back().id;
                int order_score = orders[j].weight + (largest_id - orders[j].id) * 5;
                cities[i].sumincomingscores += order_score;
            }
        }
    }
    for (int i = 0; i < cities.size(); i++)
    {
        if (cities[i].name == "Tehran")
        {
            continue;
        }
        cities[i].score = (double)(cities[i].sumincomingscores + cities[i].sumoutgoingscores) / (double)cities[i].distance;
        city[cities[i].name] = cities[i].score;
    }
    vector<pair<string, double>> sorted_cities(city.begin(), city.end());
    sort(sorted_cities.begin(), sorted_cities.end(), [](const pair<string, double> &a, const pair<string, double> &b)
         {
        if (a.second != b.second) {
            return a.second > b.second; 
        }
        return a.first < b.first; });
    return sorted_cities[0].first;
}
int calculated_truck_score(vector<Truck> &trucks, vector<Order> &orders, vector<City> &cities)
{
    vector<vector<int>> truck_data;
    string destination = calculated_city_score(cities, orders);
    int total_weight = 0;
    for (const auto &order : orders)
    {
        if (order.status == NotDelivered && order.origin_city == "Tehran" && order.destination_city == destination)
        {
            total_weight += order.weight;
        }
    }
    int best_truck_id = -1;
    int best_remaining = 1e9;
    for (const auto &truck : trucks)
    {
        if (truck.loc == "Tehran" && truck.capacity >= total_weight && truck.in_transit == false)
        {
            int remaining = truck.capacity - total_weight;
            if (remaining < best_remaining || remaining == best_remaining && truck.id < best_truck_id)
            {
                best_remaining = remaining;
                best_truck_id = truck.id;
            }
        }
    }
    return best_truck_id;
}
void recommend_city_truck(vector<Order> &orders, vector<Truck> &trucks, vector<City> &cities)
{
    string recommend_city = calculated_city_score(cities, orders);
    int recommend_truck = calculated_truck_score(trucks, orders, cities);
    cout << "Recommended city: " << recommend_city << endl
         << "Recommended truck: " << recommend_truck << endl;
}
int calculate_the_cost(vector<Order> &orders, vector<City> &cities)
{
    int sum = 0;
    for (int i = 0; i < orders.size(); i++)
    {
        if (orders[i].status == Delivered)
        {
            if (orders[i].destination_city == "Tehran")
            {
                for (int j = 0; j < cities.size(); j++)
                {
                    if (orders[i].origin_city == cities[j].name)
                    {
                        orders[i].cost = cities[j].distance * orders[i].weight;
                        sum += orders[i].cost;
                    }
                }
            }
            else
            {
                for (int j = 0; j < cities.size(); j++)
                {
                    if (orders[i].destination_city == cities[j].name)
                    {
                        orders[i].cost = cities[j].distance * orders[i].weight;
                        sum += orders[i].cost;
                    }
                }
            }
        }
    }
    return sum;
}
void financial_report(vector<Order> &orders, vector<City> &cities)
{
    int sum = calculate_the_cost(orders, cities);

    cout << "Total income: " << sum << endl;
    cout << "Delivered orders:" << endl;

    for (const auto &order : orders)
    {
        if (order.status == Delivered && order.cost > 0)
        {
            cout << order.id << " " << order.cost << endl;
        }
    }
}
void run_program(vector<Order> &orders, vector<Truck> &trucks, vector<City> &cities)
{
    const string AddOrder = "add_order";
    const string StatusOrder = "track";
    const string GetOrder = "load";
    const string Deliver = "deliver";
    const string Recommend = "recommend";
    const string FinancialReport = "financial_report";
    string input;
    Order new_order;
    int numberoforder = 0;
    int truck_id;
    string destination_city;
    while (cin >> input)
    {
        if (input == AddOrder)
        {
            cin >> new_order.origin_city >> new_order.destination_city >> new_order.weight;
            add_order(numberoforder, orders, new_order);
        }
        else if (input == StatusOrder)
        {
            cin >> new_order.id;
            status_order(new_order.id, orders);
        }
        else if (input == GetOrder)
        {
            cin >> truck_id >> destination_city;
            get_order(truck_id, destination_city, orders, trucks, numberoforder);
        }
        else if (input == Deliver)
        {
            cin >> truck_id;
            deliver_order(truck_id, orders, trucks);
        }
        else if (input == Recommend)
        {
            recommend_city_truck(orders, trucks, cities);
        }
        else if (input == FinancialReport)
        {
            financial_report(orders, cities);
        }
    }
}
int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        cerr << "Usage: " << argv[0] << " <trucks.csv> <cities.csv>" << endl;
        return 1;
    }
    vector<Truck> trucks = readTrucks(argv[1]);
    vector<City> cities = readCities(argv[2]);
    vector<Order> orders;
    run_program(orders, trucks, cities);
    return 0;
}
