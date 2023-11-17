#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <cmath>
#include <limits>

struct Car {
    std::string make;
    std::string model;
    int year;
    double dailyRate;
    bool rented;

    Car(const std::string& carMake, const std::string& carModel, int carYear, double rate)
            : make(carMake), model(carModel), year(carYear), dailyRate(rate), rented(false) {}

    double calculateRentalCost(int days) const {
        return dailyRate * days;
    }
};

struct Hotel {
    std::string hotelName;
    double latitude;
    double longitude;
    int roomNumber;

    Hotel(const std::string& name, double lat, double lon)
            : hotelName(name), latitude(lat), longitude(lon), roomNumber(0) {}

    double getRoomCost() const {
        return 300.0;
    }

    double calculateDistance(double latitudeDiff, double longitudeDiff) const {
        return sqrt(latitudeDiff * latitudeDiff + longitudeDiff * longitudeDiff);
    }

    static int findClosestHotel(const Hotel hotels[], int numHotels, double userLatitude, double userLongitude) {
        int closestHotelIndex = 0;
        double closestDistance = hotels[0].calculateDistance(userLatitude - hotels[0].latitude, userLongitude - hotels[0].longitude);

        for (int i = 1; i < numHotels; ++i) {
            double distance = hotels[i].calculateDistance(userLatitude - hotels[i].latitude, userLongitude - hotels[i].longitude);
            if (distance < closestDistance) {
                closestDistance = distance;
                closestHotelIndex = i;
            }
        }

        return closestHotelIndex;
    }
};

struct Customer {
    std::string firstName;
    std::string lastName;
    std::string contactInfo;
    int roomNumber;
    int days;
};

void rentCar(Car& car, std::ofstream& file) {
    Customer customer;
    std::cout << "Enter first name: ";
    std::cin >> customer.firstName;

    std::cout << "Enter last name: ";
    std::cin >> customer.lastName;

    std::cout << "Enter contact info: ";
    std::cin >> customer.contactInfo;

    std::cout << "Enter number of days: ";
    std::cin >> customer.days;

    car.rented = true;

    double totalCost = car.calculateRentalCost(customer.days);

    file << std::left << std::setw(20) << customer.firstName + " " + customer.lastName
         << std::setw(20) << customer.contactInfo
         << std::setw(20) << car.make + " " + car.model
         << std::setw(10) << car.year
         << std::setw(10) << customer.days
         << std::setw(15) << totalCost << std::endl;
}

void rentRoom(Hotel& hotel, std::ofstream& file) {
    Customer customer;
    std::cout << "Enter first name: ";
    std::cin >> customer.firstName;

    std::cout << "Enter last name: ";
    std::cin >> customer.lastName;

    std::cout << "Enter contact info: ";
    std::cin >> customer.contactInfo;

    std::cout << "Enter number of days: ";
    std::cin >> customer.days;

    hotel.roomNumber++;

    double totalCost = hotel.getRoomCost() * customer.days;

    file << std::left << std::setw(20) << customer.firstName + " " + customer.lastName
         << std::setw(20) << customer.contactInfo
         << std::setw(20) << hotel.hotelName
         << std::setw(10) << hotel.roomNumber
         << std::setw(10) << customer.days
         << std::setw(15) << totalCost << std::endl;
}

int main() {
    std::ofstream hotelFile("hotel_records.txt");
    hotelFile.close();
    const int numHotels = 3;
    Hotel hotels[numHotels] = {
            {"Hotel 1", 37.7749, -122.4194},
            {"Hotel 2", 34.0522, -118.2437},
            {"Hotel 3", 40.7128, -74.0060}
    };

    const int numCars = 3;
    Car cars[numCars] = {
            {"Toyota", "Camry", 2022, 50.0},
            {"Honda", "Accord", 2022, 55.0},
            {"Ford", "Mustang", 2022, 70.0}
    };

    double userLatitude, userLongitude;
    std::cout << "Enter your current latitude: ";
    std::cin >> userLatitude;

    std::cout << "Enter your current longitude: ";
    std::cin >> userLongitude;

    int closestHotelIndex = Hotel::findClosestHotel(hotels, numHotels, userLatitude, userLongitude);
    Hotel& closestHotel = hotels[closestHotelIndex];

    int option;
    std::cout << "Choose an option:\n";
    std::cout << "1. Rent a car\n";
    std::cout << "2. Rent a room\n";
    std::cin >> option;

    if (option == 1) {
        int carChoice;
        std::cout << "Available Cars:\n";
        for (int i = 0; i < numCars; ++i) {
            std::cout << i + 1 << ". " << cars[i].make << " " << cars[i].model << " (" << cars[i].year << ")\n";
        }
        std::cout << "Select a car (1-" << numCars << "): ";
        std::cin >> carChoice;

        if (carChoice >= 1 && carChoice <= numCars && !cars[carChoice - 1].rented) {
            std::ofstream carFile("rental_records.txt", std::ios::app);
            rentCar(cars[carChoice - 1], carFile);

        } else {
            std::cout << "Invalid selection or car is already rented.\n";
        }
    } else if (option == 2) {
        std::ofstream file("hotel_records.txt", std::ios::app);

        char anotherRoom;
        do {
            rentRoom(closestHotel, file);

            std::cout << "Do you want to rent another room? (y/n): ";
            std::cin >> anotherRoom;
        } while (anotherRoom == 'y' || anotherRoom == 'Y');
    } else {
        std::cout << "Invalid option\n";
    }

    std::ifstream carFile("rental_records.txt");
    std::ifstream roomFile("hotel_records.txt");
    std::string line;

    std::cout << "\n***** Car Rental Records *****\n";
    while (std::getline(carFile, line)) {
        std::cout << line << std::endl;
    }

    std::cout << "\n***** Hotel Records *****\n";
    while (std::getline(roomFile, line)) {
        std::cout << line << std::endl;
    }

    return 0;
}
