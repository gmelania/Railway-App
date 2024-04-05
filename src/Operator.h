#include "User.h"

class Operator : public User {
  public:
    /*
      Append a new trip to the data file
    */
    void printNewTripToFile(string filepath) {
        try {
            ofstream fout(filepath, ios::app);
            fout << this->bookedTrip.start << ","
                 << this->bookedTrip.destination << "," << this->bookedTrip.date
                 << "," << this->bookedTrip.departure << endl;
            std::cout << "The file has been updated!";
            fout.close();
        } catch (const ofstream::failure &e) {
            cerr << "Error writing to file: " << e.what() << endl;
            // throw;
        }
    }

    void updateOriginalFile(string originalFilepath, string copyFilepath) {
        try {
            /*
                Replace trips from the original file using trunc mode
                to overwrite existing content
            */
            openFile(copyFilepath);
            ifstream fin(copyFilepath);
            ofstream fout(originalFilepath, ios::trunc);

            std::string line;
            while (getline(fin, line)) {
                fout << line << std::endl;
            }

            fout.close();
            fin.close();
        } catch (const exception &e) {
            cerr << "An error occurred: " << e.what() << endl;
        } catch (...) {
            cerr << "An unexpected error occurred." << endl;
        }
    }

    void deleteTrip(int &lineIndex, string filepath, string filepathCopy) {
        try {
            openFile(filepath);
            ifstream fin(filepath);
            /*
                Store the trips in a vector of strings
            */

            vector<string> lines;
            string line;
            while (getline(fin, line))
                lines.push_back(line);
            fin.close();

            if (lineIndex > lines.size() || lineIndex < 1) {
                cout << "Line index not found in file." << endl;
                cout << "File has " << lines.size() << " lines from 1 to "
                     << lines.size() << "." << endl;
                return;
            }

            /*
                Copy the trips to a new file avoiding the one to be deleted
            */
            openFile(filepathCopy);
            ofstream fout(filepathCopy);
            for (int i = 0; i < lines.size(); i++)
                if (i != lineIndex)
                    fout << lines[i] << endl;
            fout.close();

            std::cout << "Trip deleted successfully." << endl;

        } catch (const std::exception &e) {
            cerr << "An error occurred: " << e.what() << endl;
        } catch (...) {
            cerr << "An unexpected error occurred." << endl;
        }
    }

    bool deleteTripFromFile(string filepath, int &n, int tripIndex,
                            Trip trip[]) {
        try {
            ifstream fin(filepath);

            // string line;
            for (int i = 0; i < n; i++) {
                if (i == tripIndex - 1) {
                    std::cout << "Start: " << trip[i].start
                              << "\nDestination: " << trip[i].destination
                              << "\nDate: " << trip[i].date
                              << "\nDeparture time: " << trip[i].departure
                              << endl;
                    std::cout << "Confirm deletion? (y/n) ";

                    char answer;
                    cin >> answer;
                    if (answer == 'y') {
                        /*
                            Update original file
                        */
                        string filepathCopy =
                            "C:\\Users\\User\\Desktop\\DataBase App - "
                            "C++\\input\\tripsCopy.csv";
                        deleteTrip(i, filepath, filepathCopy);
                        n--;
                        updateOriginalFile(filepath, filepathCopy);

                        return true;
                    }
                }
            }
            std::cout << "Trip not found for deletion." << endl;
            return false;
        } catch (const ifstream::failure &e) {
            cerr << "Error reading file: " << e.what() << endl;
            throw;
        } catch (...) {
            cerr << "An unexpected error occurred during trip deletion."
                 << endl;
            throw;
        }
    }

    void editTripFile(string filepath, int &n, Trip trip[]) {
        try {
            openFile(filepath);
            std::cout << "Options: " << endl;
            std::cout << "- 1 -> Add a trip." << endl
                      << "- 2 -> Remove a trip." << endl;

            char answer;
            cin >> answer;
            if (answer == '1') {
                std::cout << "Required details:\n";
                /*
                    Add new trip to data file
                */
                std::cout << "Enter start: ";
                cin >> this->bookedTrip.start;
                std::cout << "Enter destination: ";
                cin >> this->bookedTrip.destination;

                string date;
                if (validDate(date, trip, 0, 'o') == -1) {
                    throw invalid_argument("Process terminated.");
                } else {
                    this->bookedTrip.date = date;
                }

                string time;
                if (validDeparture(time, trip, 0, 'o') == -1) {
                    throw invalid_argument("Process terminated.");
                } else {
                    this->bookedTrip.departure = time;
                }

                std::cout << "New trip details:\n";
                std::cout << this->bookedTrip.start << " -> "
                          << this->bookedTrip.destination << " - "
                          << this->bookedTrip.date << ", "
                          << this->bookedTrip.departure << "\n";
                cout << "Confirm details? (y/n) ";
                cin >> answer;
                if (answer == 'y' || answer == 'Y') {
                    printNewTripToFile(filepath);
                    n++;
                } else {
                    throw invalid_argument("Details discarded.");
                }
            } else if (answer == '2') {
                /*
                    Delete trip from data file
                */
                displayAvailableTrips(n, trip);
                std::cout << "Enter the number of the trip to be deleted: ";
                int tripIndex;
                cin >> tripIndex;
                if (!deleteTripFromFile(filepath, n, tripIndex, trip)) {
                    throw invalid_argument("Trip deletion has been canceled.");
                }
            }

        } catch (const exception &e) {
            cerr << "An error occurred: " << e.what() << endl;
        } catch (...) {
            cerr << "An unexpected error occurred." << endl;
        }
    }
};