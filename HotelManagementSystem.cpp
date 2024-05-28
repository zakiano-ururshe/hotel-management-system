#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <cctype>
#include <vector>

using namespace std;

// Base class for Person, demonstrates inheritance
class Person {
protected:
    string name;
    int id;
public:
    // Default constructor
    Person() : name(""), id(0) {}

    // Parameterized constructor
    Person(string n, int i) : name(n), id(i) {}

    // Virtual function to be overridden by derived classes
    virtual void display() const {
        cout << "Name: " << name << ", ID: " << id << endl;
    }
};

// Customer class derived from Person, demonstrates inheritance and polymorphism
class Customer : public Person {
private:
    int numOfDays;
    char typeOfRoom;
public:
    // Default constructor
    Customer() : Person(), numOfDays(0), typeOfRoom(' ') {}

    // Parameterized constructor using initializer list
    Customer(string n, int i, int days, char roomType)
        : Person(n, i), numOfDays(days), typeOfRoom(roomType) {}

    // Function to read all customers from the file and display their details
    void readCustomer() {
        ifstream file("customer.txt", ios::binary);
        if (!file) {
            cout << "File Not Found\n";
            return;
        }

        Customer c;
        cout << "Show All Hotel Booking: ";
        cout << "\n__\n\n";
        cout << "Name\tId\tType Of Room\tNumber Of Days Booked\n";
        while (file.read(reinterpret_cast<char*>(&c), sizeof(c))) {
            cout << c.name << "\t" << c.id << "\t";
            if (c.typeOfRoom == 's' || c.typeOfRoom == 'S') cout << "Single\t\t";
            else if (c.typeOfRoom == 'd' || c.typeOfRoom == 'D') cout << "Double\t\t";
            cout << c.numOfDays << "\n";
        }
        file.close();

        cout << "\n\nPress Enter to continue....!! ";
        cin.ignore();
        cin.get();
    }

    // Function to write a new customer booking to the file
    void writeCustomer() {
        ofstream file("customer.txt", ios::binary | ios::app);
        if (!file) {
            cout << "File could not be opened\n";
            return;
        }

        char ch;
        do {
            Customer c;
            cout << "Add a New Booking: ";
            cout << "\n_\n\n";
            cout << "Enter Your Name: "; cin >> c.name;

            // Validate the customer's name
            bool valid = true;
            for (char c : c.name) {
                if (!isalpha(c)) {
                    valid = false;
                    break;
                }
            }
            if (c.name.length() < 3) valid = false;

            while (!valid) {
                valid = true;
                cout << "Enter a valid name: "; cin >> c.name;
                for (char c : c.name) {
                    if (!isalpha(c)) {
                        valid = false;
                        break;
                    }
                }
                if (c.name.length() < 3) valid = false;
            }

            cout << "Enter your Id: "; cin >> c.id;
            cout << "Do you want single room or double ? (S/D): "; cin >> c.typeOfRoom;
            // Validate the room type input
            while (c.typeOfRoom != 'S' && c.typeOfRoom != 's' && c.typeOfRoom != 'D' && c.typeOfRoom != 'd') {
                cout << "Enter a valid character: ";
                cin >> c.typeOfRoom;
            }
            cout << "Enter the number of days you will book: "; cin >> c.numOfDays;

            file.write(reinterpret_cast<const char*>(&c), sizeof(c));
            cout << "\nAdd Another Booking ? (y/press any character such (n)): ";
            cin >> ch;
            system("clear"); // On Windows, use system("cls");
        } while (ch == 'y' || ch == 'Y');
        file.close();
    }

    // Function to search for a customer booking by ID
    void searchCustomer() {
        ifstream file("customer.txt", ios::binary);
        if (!file) {
            cout << "File Not Found\n";
            return;
        }

        Customer c;
        cout << "Search about Booking by Id: ";
        cout << "\n__\n\n";
        bool found = false;
        int x;
        cout << "Enter Id: ";
        cin >> x;

        while (file.read(reinterpret_cast<char*>(&c), sizeof(c))) {
            if (x == c.id) {
                found = true;
                cout << "Name\tId\tType Of Room\tNumber Of Days Booked\n";
                cout << c.name << "\t" << c.id << "\t";
                if (c.typeOfRoom == 's' || c.typeOfRoom == 'S') cout << "Single\t\t";
                else if (c.typeOfRoom == 'd' || c.typeOfRoom == 'D') cout << "Double\t\t";
                cout << c.numOfDays << "\n";
                break;
            }
        }
        if (!found) {
            cout << "Not found\n";
        }
        file.close();

        cout << "\n\nPress Enter to continue....!! ";
        cin.ignore();
        cin.get();
    }

    // Function to update the number of days booked for a customer
    void updateCustomer() {
        fstream file("customer.txt", ios::binary | ios::in | ios::out);
        if (!file) {
            cout << "File Not Found\n";
            return;
        }

        Customer c;
        cout << "Update Booking: ";
        cout << "\n__\n\n";
        bool found = false;
        int x;
        cout << "Enter Id Of Customer to update number of days booked: "; cin >> x;

        while (file.read(reinterpret_cast<char*>(&c), sizeof(c))) {
            if (x == c.id) {
                cout << "Enter your new number of days: ";
                cin >> c.numOfDays;

                int pos = file.tellg();
                file.seekp(pos - sizeof(c), ios::beg);
                file.write(reinterpret_cast<const char*>(&c), sizeof(c));

                found = true;
                cout << "\n\nName\tId\tType Of Room\tNumber Of Days Booked\n";
                cout << c.name << "\t" << c.id << "\t";
                if (c.typeOfRoom == 's' || c.typeOfRoom == 'S') cout << "Single\t\t";
                else if (c.typeOfRoom == 'd' || c.typeOfRoom == 'D') cout << "Double\t\t";
                cout << c.numOfDays << "\n";
                break;
            }
        }
        if (!found) cout << "\nId Not Found\n";
        file.close();

        cout << "\n\nPress Enter to continue....!! ";
        cin.ignore();
        cin.get();
    }

    // Function to delete a customer booking record by ID
    void deleteRecordCustomer() {
        ifstream file("customer.txt", ios::binary);
        ofstream temp("temp.txt", ios::binary);
        if (!file) {
            cout << "File Not Found\n";
            return;
        }

        Customer c;
        cout << "Delete Booking: ";
        cout << "\n__\n\n";
        int x;
        cout << "Enter Id Of Customer To Delete: "; cin >> x;
        bool found = false;

        while (file.read(reinterpret_cast<char*>(&c), sizeof(c))) {
            if (x != c.id) {
                temp.write(reinterpret_cast<const char*>(&c), sizeof(c));
            }
            else {
                found = true;
            }
        }
        file.close();
        temp.close();
        remove("customer.txt");
        rename("temp.txt", "customer.txt");

        if (found) cout << "\nThe customer record was deleted successfully";
        else cout << "\n\nCustomer record not found\n";
        cout << "\n\nPress Enter to continue....!! ";
        cin.ignore();
        cin.get();
    }

    // Overloaded == operator to compare two customers by their ID
    bool operator==(const Customer& other) const {
        return this->id == other.id;
    }

    // Overloaded << operator to print customer details
    friend ostream& operator<<(ostream& os, const Customer& c) {
        os << "Name: " << c.name << ", ID: " << c.id << ", Room Type: "
            << c.typeOfRoom << ", Days: " << c.numOfDays;
        return os;
    }
};

// Template function to display a list of items
template <typename T>
void displayList(const vector<T>& list) {
    for (const auto& item : list) {
        cout << item << endl;
    }
}

// Room class to manage hotel rooms
class Room {
private:
    int room_no, room_price;
    char room_type, room_view;
public:
    // Default constructor
    Room() : room_no(0), room_price(0), room_type(' '), room_view(' ') {}

    // Parameterized constructor
    Room(int no, int price, char type, char view)
        : room_no(no), room_price(price), room_type(type), room_view(view) {}

    // Function to write a new room to the file
    void writeRoom() {
        ofstream file("rooms.txt", ios::binary | ios::app);
        if (!file) {
            cout << "File could not be opened\n";
            return;
        }

        char ch;
        do {
            Room r;
            cout << "Add a New Room: ";
            cout << "\n_\n\n";
            cout << "Enter Room Number: "; cin >> r.room_no;
            cout << "Enter Room Type (S-single, D-double): "; cin >> r.room_type;
            while (r.room_type != 'S' && r.room_type != 's' && r.room_type != 'D' && r.room_type != 'd') {
                cout << "Enter a valid character: ";
                cin >> r.room_type;
            }
            cout << "Enter Room View (S-sea, P-pool, G-garden): "; cin >> r.room_view;
            while (r.room_view != 'S' && r.room_view != 's' && r.room_view != 'P' && r.room_view != 'p' && r.room_view != 'G' && r.room_view != 'g') {
                cout << "Enter a valid character: ";
                cin >> r.room_view;
            }
            cout << "Enter Room Price: "; cin >> r.room_price;

            file.write(reinterpret_cast<const char*>(&r), sizeof(r));
            cout << "\nAdd Another Room ? (y/press any character such (n)): ";
            cin >> ch;
            system("clear"); // On Windows, use system("cls");
        } while (ch == 'y' || ch == 'Y');
        file.close();
    }

    // Function to read all rooms from the file and display their details
    void readRoom() {
        ifstream file("rooms.txt", ios::binary);
        if (!file) {
            cout << "FILE NOT FOUND\n\n";
            return;
        }

        Room r;
        cout << "Show all Rooms: ";
        cout << "\n__\n\n";
        cout << "NUMBER\tTYPE\tVIEW\tPRICE\n\n";
        while (file.read(reinterpret_cast<char*>(&r), sizeof(r))) {
            cout << r.room_no << "\t";
            if (r.room_type == 'S' || r.room_type == 's') cout << "Single\t";
            else if (r.room_type == 'D' || r.room_type == 'd') cout << "Double\t";
            if (r.room_view == 'S' || r.room_view == 's') cout << "Sea\t";
            else if (r.room_view == 'P' || r.room_view == 'p') cout << "Pool\t";
            else if (r.room_view == 'G' || r.room_view == 'g') cout << "Garden\t";
            cout << r.room_price << "\n";
        }
        file.close();

        cout << "\n\nPress Enter to continue....!! ";
        cin.ignore();
        cin.get();
    }

    // Function to search for a room by room number
    void searchRoom() {
        ifstream file("rooms.txt", ios::binary);
        if (!file) {
            cout << "File Not Found\n";
            return;
        }

        Room r;
        cout << "Search about room: ";
        cout << "\n__\n\n";
        bool found = false;
        int x;
        cout << "Enter Room Number: ";
        cin >> x;

        while (file.read(reinterpret_cast<char*>(&r), sizeof(r))) {
            if (x == r.room_no) {
                found = true;
                cout << "NUMBER\tTYPE\tVIEW\tPRICE\n\n";
                cout << r.room_no << "\t";
                if (r.room_type == 'S' || r.room_type == 's') cout << "Single\t";
                else if (r.room_type == 'D' || r.room_type == 'd') cout << "Double\t";
                if (r.room_view == 'S' || r.room_view == 's') cout << "Sea\t";
                else if (r.room_view == 'P' || r.room_view == 'p') cout << "Pool\t";
                else if (r.room_view == 'G' || r.room_view == 'g') cout << "Garden\t";
                cout << r.room_price << "\n";
                break;
            }
        }
        if (!found) {
            cout << "Not Found\n";
        }
        file.close();

        cout << "\n\nPress Enter to continue....!! ";
        cin.ignore();
        cin.get();
    }

    // Friend function to display room details
    friend ostream& operator<<(ostream& os, const Room& r) {
        os << "Room Number: " << r.room_no << ", Type: " << r.room_type
            << ", View: " << r.room_view << ", Price: " << r.room_price;
        return os;
    }
};

// Function to display the main menu
void menu() {
    system("clear"); // On Windows, use system("cls");
    cout << "=====================================================\n";
    cout << "======================== Hotel Management System =====================\n";
    cout << "=====================================================\n";
    cout << "1. Booking Customer Room\n";
    cout << "2. Add A New Room\n";
    cout << "3. View All Rooms\n";
    cout << "4. Search About Room\n";
    cout << "5. View All Booking Customers\n";
    cout << "6. Search About Customer\n";
    cout << "7. Update Booking Information\n";
    cout << "8. Delete Booking Information\n";
    cout << "9. Exit\n";
}

int main() {
    Customer customer;
    Room room;
    vector<Customer> customerList;
    vector<Room> roomList;
    int choice;
    do {
        menu();
        cout << "Enter Choice: ";
        cin >> choice;
        switch (choice) {
        case 1:
            customer.writeCustomer();
            break;
        case 2:
            room.writeRoom();
            break;
        case 3:
            room.readRoom();
            break;
        case 4:
            room.searchRoom();
            break;
        case 5:
            customer.readCustomer();
            break;
        case 6:
            customer.searchCustomer();
            break;
        case 7:
            customer.updateCustomer();
            break;
        case 8:
            customer.deleteRecordCustomer();
            break;
        case 9:
            exit(0);
            break;
        default:
            cout << "Invalid choice!\n";
        }
    } while (choice != 9);

    return 0;
}