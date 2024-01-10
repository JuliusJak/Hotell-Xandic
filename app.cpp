#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>


using std::string;
using std::cin;
using std::cout;
using std::vector;
using std::ifstream;
using std::istringstream;
using std::endl;
using std::ofstream;


struct Reservation {
    string reservationDetails;
};

struct Person {
    string username;
    string password;
    vector<Reservation> reservations;
};


bool authentication(vector<Person>& people, Person& authenticatedUser) {
    string username;
    string password;

    cout << "Enter username: ";
    cin >> username;
    cout << "Enter password: ";
    cin >> password;

    ifstream file("users/userList.csv");
    string line;

    while (getline(file, line)) {
        istringstream iss(line);
        string storedUsername, storedPassword;
        getline(iss, storedUsername, ',');
        getline(iss, storedPassword, ',');

        if (username == storedUsername && password == storedPassword) {
            authenticatedUser.username = username;
            authenticatedUser.password = password;
            return true;
        }
    }

    cout << "Invalid username or password." << endl;
    return false;
}

void currentlyLoggedIn(const Person& user) {
    cout << "Currently logged in as: " << user.username << endl;
}

void showReservations(const Person& user) {
//should only show current reservations that has the tag: expired=false
    cout << "Showing reservations for: " << user.username << endl << endl;

    ifstream reservationsFile("reservations/" + user.username + "_reservations.csv");
    string line;

    getline(reservationsFile, line);

    if (reservationsFile.eof()) {
        cout << "Found no current reservations for: " << user.username << endl << endl;
        reservationsFile.close();
        return;
    }

    while (getline(reservationsFile, line)) {
        cout << "Reservation: " << line << endl;
    }

    cout << endl;

    reservationsFile.close();
}

void createReservationFile(const Person& user) {
    string filename = "reservations/" + user.username + "_reservations.csv";

    ifstream fileCheck(filename);
    if (fileCheck.good()) {
        cout << "Reservation file for " << user.username << " already exists." << endl;
        return;
    }

    ofstream reservationsFile(filename);

    reservationsFile << "Check in date,Check out date,Number of guests,Room type,Room number,expired" << '\n';


    for (const Reservation& reservation : user.reservations) {
        reservationsFile << reservation.reservationDetails << '\n';
    }

    reservationsFile.close();
    cout << "Reservation file created for " << user.username << "." << endl;
}

void newReservation(Person& user) {
//add a head/tag that says expired and is true or false
//it's false by default when creating a new reservation but becomes true when it's deleted
    string checkInDate;
    string checkOutDate;
    string numGuests;
    string roomType;
    string reservationDetails;

    cout << "Enter reservation details:" << endl;
    cout << "Checkin date: "; cin >> checkInDate;
    cout << "Checkout date: "; cin >> checkOutDate;
    cout << "Number of guests: "; cin >> numGuests;
    cout << "Room type: "; cin >> roomType;
    
    reservationDetails = checkInDate +","+ checkOutDate +","+ numGuests +","+ roomType;

    Reservation newReservation;
    newReservation.reservationDetails = reservationDetails;

    user.reservations.push_back(newReservation);

    ofstream reservationsFile("reservations/" + user.username + "_reservations.csv", std::ios::app);

    reservationsFile << newReservation.reservationDetails << '\n';

    reservationsFile.close();

    cout << "Reservation added successfully!" << endl;
}


void showReservationHistory(){
//should only show expired/deleted reservations that has the tag: active=true
}

void deleteReservation(){

}

void deleteAccount(){
//ask "are you sure?"
}

int main() {
    vector<Person> people;
    Person authenticatedUser;

    if (!authentication(people, authenticatedUser)) {
        return 0;
    }

    createReservationFile(authenticatedUser);

    currentlyLoggedIn(authenticatedUser);

    int choice;
    do {
        cout << "\nOptions Menu:\n"
             << "1. Make a new reservation\n"
             << "2. View current reservatoins\n"
             << "3. View reservation history\n"
             << "4. Delete a reservation\n"
             << "5. Delete account\n"
             << "0. Logout\n"
             << "Enter your choice (0-5): ";
        cin >> choice;

        switch (choice) {
            case 1: newReservation(authenticatedUser); break;
            case 2: showReservations(authenticatedUser); break;
            case 3: showReservationHistory(); break;
            case 4: deleteReservation(); break;
            case 5: deleteAccount(); return 0;
            case 0: cout << "Logging out...\n"; break;
            default: cout << "Invalid choice. Please enter a number between 0 and 4.\n";
        }
    } while (choice != 0);


    return 0;
}