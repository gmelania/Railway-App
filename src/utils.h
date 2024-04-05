#include "lib.h"

struct Trip {
    string start, destination;
    string date;
    string departure;
};

void openFile(string filepath) {
    try {
        fstream file;
        file.open(filepath);
        if (!file.is_open()) {
            throw runtime_error("File \"" + filepath +
                                "\" could not be opened.");
        }
    } catch (const exception &e) {
        cerr << "Exception caught: " << e.what() << endl;
    }
}

int countLines(string filepath) {
    try {
        openFile(filepath);
        ifstream fin(filepath);

        string line;
        int counter = 0; // variable to count lines
        while (getline(fin, line)) {
            counter++;
        }
        fin.close();
        return counter;
    } catch (const exception &e) {
        cerr << "Exception caught: " << e.what() << endl;
        return -1;
    }
}

void displayAvailableTrips(int n, Trip trip[]) {
    string filepath = "C:\\Users\\User\\Desktop\\DataBase App - "
                      "C++\\input\\trips.csv";

    openFile(filepath);
    ifstream fin(filepath);
    std::cout << "Available trips:\n";

    for (int i = 0; i < n; i++) {
        std::cout << "\nTrip " << i + 1 << ": ";
        getline(fin, trip[i].start, ',');
        getline(fin, trip[i].destination, ',');
        getline(fin, trip[i].date, ',');
        getline(fin, trip[i].departure, '\n');
        std::cout << trip[i].start << " " << trip[i].destination << " "
                  << trip[i].date << " " << trip[i].departure << "\n";
    }
    fin.close();
}

void exitProgram() {
    cout << "\rExiting application   \rExiting application";
    for (int i = 0; i < 3; i++) {
        sleep(1);
        cout << ".";
    }
    sleep(1);
    std::exit(0);
}