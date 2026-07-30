# Cargo Transportation Management System

A command-line cargo transportation management system developed individually in C++ as a university programming project.

## Overview

This project manages cargo transportation between Tehran and several surrounding cities.

Tehran is considered the central city, and every other city is directly connected only to Tehran. The system manages trucks, cargo orders, transportation status, truck loading, delivery, route recommendations, and financial reports.

## Features

- Loading truck information from a CSV file
- Loading city information and distances from a CSV file
- Registering new cargo orders
- Assigning a unique ID to each order
- Tracking the current status of every order
- Managing warehouse, in-transit, and delivered orders
- Loading suitable orders into trucks
- Prioritizing older and heavier orders
- Checking truck capacity during loading
- Updating truck locations after delivery
- Recommending the best destination city
- Recommending the most suitable truck
- Calculating transportation costs
- Generating financial reports for delivered orders
- Command-line input processing
- Modular and top-down program design

## Transportation Model

Tehran is the central point of the transportation network.

Each surrounding city has one direct route to Tehran, and there are no direct routes between the surrounding cities.

All trucks are initially located in Tehran. After delivering their cargo, trucks remain in the destination city.

## Input Files

The program receives two CSV file paths as command-line arguments:

1. Trucks file
2. Cities file

Example trucks file:

```csv
truck_id,capacity
1,100
2,150
3,200
4,50
```

Example cities file:

```csv
city_name,distance
Arak,500
Isfahan,700
```

## Main Commands

### Add a new order

```text
add_order <origin_city> <destination_city> <weight>
```

### Track an order

```text
track <order_id>
```

### Load a truck

```text
load <truck_id> <destination_city>
```

### Deliver orders

```text
deliver <truck_id>
```

### Recommend a city and truck

```text
recommend
```

### Generate a financial report

```text
financial_report
```

## Order Status

Each order can have one of the following statuses:

- In warehouse
- In transit
- Delivered

## Loading Priority

Orders are prioritized using their weight and registration order.

Older and heavier orders receive a higher priority. Orders are loaded only when their weight does not exceed the remaining capacity of the selected truck.

## Financial Report

The transportation cost of an order is calculated using:

```text
cost = distance × weight
```

Only delivered orders are included in the total company income.

## Run

```bash
./UTTruck <path-to-trucks.csv> <path-to-cities.csv>
```

Example:

```bash
./UTTruck trucks.csv cities.csv
```

## Programming Concepts

- C++ programming
- Top-down design
- Modular programming
- Structures and custom data types
- File handling
- CSV processing
- Sorting and prioritization
- Capacity management
- Command-line interfaces
- State management
- Input processing
- Financial calculations

## Technologies

- C++
- Visual Studio Code
- Command-line interface
- CSV files

## Author

**Tara Jahanshahi**

Computer Engineering Student  
University of Tehran
