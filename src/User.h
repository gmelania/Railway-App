#include "lib.h"
#include "utils.h"

class User {
  protected:
    string username;
    string password;
    Trip bookedTrip; // every user can book one trip per session

  public:
    User() {}

    void setUsername(string &user) { this->username = user; }
    string getUsername() { return this->username; }

    void setPassword(string &finalPassword) { this->password = finalPassword; }
    string getPassword() { return this->password; }

    /*
        Asterisk input password
    */
    void setPasswordwithStars(string &passwordCopy) {
        try {
            char c;
            while (true) {
                c = _getch();
                if (c == 13) // Enter key
                    break;
                else if (c == 8) // Backspace key
                {
                    if (!passwordCopy.empty()) {
                        std::cout << "\b \b";
                        passwordCopy.pop_back();
                    }
                } else {
                    passwordCopy.push_back(c);
                    std::cout << '*';
                }
            }
        } catch (const exception &e) {
            cerr << "Exception caught: " << e.what() << endl;
        }
    }

    bool checkPassword(string enteredPassword) {
        try {
            string attempt;
            setPasswordwithStars(attempt);
            if (enteredPassword == "esc")
                throw 505;

            if (enteredPassword == attempt) {
                return true;
            } else {
                throw invalid_argument("Incorrect password.");
            }
        } catch (const exception &ex) {
            cerr << "\nException caught: " << ex.what();
            return false;
        } catch (...) {
            exitProgram();
        }
    }

    /*
        Existent user - verification
    */
    bool verifyUser(const std::string &username) {
        try {
            string filepath = "C:\\Users\\User\\Desktop\\DataBase App - "
                              "C++\\db\\users.csv";
            openFile(filepath);
            ifstream fin(filepath);

            string line;
            while (getline(fin, line)) {
                string user = line.substr(0, line.find(','));

                if (user == username) {
                    fin.close();
                    return true;
                }
            }
            fin.close();
            return false;

        } catch (const exception &ex) {
            cerr << "Exception caught: " << ex.what() << endl;
            return false;
        }
    }

    /*
        E-mail verification
    */
    bool validUsername(string &username) {
        try {
            std::cout << "Enter e-mail: ";
            cin >> username;

            if (username == "esc")
                exitProgram();

            regex regexEmail(
                R"([a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,})");

            if (!regex_match(username, regexEmail)) {
                throw invalid_argument("Invalid e-mail address. ");
            }

            /*
                Search the input e-mail in the existent users
            */
            if (verifyUser(username)) {
                std::cout << "This username is already taken.\n";
                std::cout << "Press 1 to re-enter or any key to exit: ";
                char answer;
                cin >> answer;
                if (answer == '1') {
                    return validUsername(username); // while remake
                }
                return false;
            }
            return true;
        } catch (const invalid_argument &ex) {
            cerr << ex.what() << "Please try again.\n";
            return validUsername(username);
        } catch (const exception &ex) {
            cerr << "Exception caught: " << ex.what() << std::endl;
            return false;
        }
    }

    /*
        Password validition
    */
    bool validPassword(string &passwd) {
        try {
            std::cout << "Enter password: ";
            cin >> passwd;

            if (passwd == "esc")
                throw 505;

            regex regexPassword(
                "(?=.*\\d)(?=.*[a-z])(?=.*[A-Z])(?=.*[a-zA-Z]).{8,}");

            if (!regex_match(passwd, regexPassword)) {
                throw invalid_argument("Weak password. ");
            }

            return true;
        } catch (const invalid_argument &ex) {
            cerr << ex.what() << "Please try again.\n";
            validPassword(passwd);
        } catch (const exception &ex) {
            cerr << "Exception caught: " << ex.what() << endl;
            return false;
        } catch (...) {
            exitProgram();
        }
        return false;
    }

    /*
        Verify if the introduced password is correct
    */
    bool verifyPassword(const string &password, const string &username) {
        try {
            string filepath = "C:\\Users\\User\\Desktop\\DataBase App - "
                              "C++\\db\\users.csv";
            openFile(filepath);
            ifstream fin(filepath);
            string line;
            while (getline(fin, line)) {
                string user = line.substr(0, line.find(','));

                line.erase(0, line.find(',') + 1);
                string pass = line.substr(0, line.find(','));

                if (user == username && pass == password) {
                    fin.close();
                    return true;
                }
            }
            fin.close();
            return false;

        } catch (const exception &ex) {
            cerr << "Exception caught: " << ex.what() << endl;
            return false;
        }
    }

    void login() {
        string enteredUsername;
        bool answer;
        std::cout << "Username (e-mail): ";

        cin >> enteredUsername;
        if (verifyUser(enteredUsername) == false) {
            std::cout << "Username not found." << endl;
            std::cout << "Press 1 to re-enter username or any key to exit."
                      << endl;
            cin >> answer;
            if (answer == 1) {
                login();
            } else {
                exitProgram();
            }
        } else {
            setUsername(enteredUsername);
        }

        /*
            Password matching verification
        */
        string enteredPassword;
        while (1) {
            std::cout << "Password: ";
            enteredPassword = "";
            setPasswordwithStars(enteredPassword);
            if (verifyPassword(enteredPassword, enteredUsername) == false) {
                std::cout << "\nIncorrect password.\n";
                std::cout
                    << "Press 1 to re-enter password or any key to exit.\n";
                cin >> answer;
                if (answer == '1') {
                    exitProgram();
                }
            } else {
                setPassword(enteredPassword);
                break;
            }
        }
        std::cout << endl << "Login successful!" << endl;
    }

    /*
        Check existence of a trip start and destination
    */
    int validWay(string &start, string &destination, Trip trip[],
                 int &tripIndex) {
        char answer;
        try {
            std::cout << "Enter start: ";
            cin >> start;
            std::cout << "Enter destination: ";
            cin >> destination;

            // checking cities
            bool ok = false;
            for (int i = 0; i < 5; i++) {
                if (start == trip[i].start &&
                    destination == trip[i].destination) {
                    ok = true;
                    tripIndex = i;
                    break;
                }
            }

            if (!ok) {
                throw invalid_argument("\nUnavailable trip.");
            }
            return 1;
        } catch (const invalid_argument &ex) {
            std::cout << "Exception caught: " << ex.what()
                      << "\nPress 1 to re-enter or any key to exit: ";
            cin >> answer;
            if (answer == '1') {
                return validWay(start, destination, trip, tripIndex);
            }
        }
        exitProgram();
        return -1;
    }

    /*
        Date validation
    */
    int validDate(string &date, Trip trip[], int tripIndex, char call) {
        try {
            char answer;
            std::cout << "Enter the date (yyyy-mm-dd): ";
            cin >> date;

            regex dateRegex(R"(\d{4}-\d{2}-\d{2})");
            if (!regex_match(date, dateRegex)) {
                throw invalid_argument("Invalid date format.");
            }

            string year = date.substr(0, 4);
            string month = date.substr(5, 2);
            string day = date.substr(8, 2);
            int y = stoi(year);
            int m = stoi(month);
            int d = stoi(day);

            if ((m < 1 || m > 12) || (d < 1 || d > 31)) {
                throw invalid_argument("Invalid date components.");
            }

            /*
                Check if the date is in the past
            */
            time_t currentTime = time(nullptr);
            struct tm tm = *localtime(&currentTime);
            int year_, month_, day_;
            sscanf(date.c_str(), "%d-%d-%d", &year_, &month_, &day_);
            tm.tm_year = year_ - 1900;
            tm.tm_mon = month_ - 1;
            tm.tm_mday = day_;
            time_t selectedTime = mktime(&tm);

            if (selectedTime < currentTime) {
                throw invalid_argument("Entered date is in the past.");
            }

            // check if the call is from a user
            if (call == 'u' || call == 'U')
                dateExistsCheck(date, trip, tripIndex);
            return 1;
        } catch (const exception &ex) {
            cerr << "Exception caught: " << ex.what() << endl;
            std::cout << "Press 1 to re-enter or any key to exit: ";
            char answer;
            cin >> answer;
            if (answer == '1') {
                return validDate(date, trip, tripIndex, call);
            } else {
                return -1;
            }
        }
    }

    void dateExistsCheck(string &date, Trip trip[], int tripIndex) {
        // check if the date is correct for the existent trip
        if (date != trip[tripIndex].date) {
            throw invalid_argument(
                "Unavailable trip date.\nOnly availabile date is: " +
                trip[tripIndex].date);
        }
    }

    /*
        Time validation
    */
    bool timeCheck(int h, int min) {
        return (((h >= 0) && (h < 24)) && ((min >= 0) && (min < 60)));
    }

    int validDeparture(string &departure, Trip trip[], int tripIndex,
                       char call) {
        try {
            char answer;
            std::cout << "Please enter the departure time (hh:mm): ";
            cin >> departure;

            regex timeRegex(R"(\d{2}:\d{2})");
            if (!regex_match(departure, timeRegex)) {
                throw invalid_argument("Invalid time format.");
            }

            // check if the time is valid
            istringstream ss(departure);
            char c;
            int h, min;

            if (!((ss >> h >> c >> min) && (c == ':') &&
                  timeCheck(h, min) == true))
                throw invalid_argument("Time format is not valid");

            // check if the call is from a user
            if (call == 'u' || call == 'U')
                timeExistsCheck(departure, trip, tripIndex);
            return 1;
        } catch (const exception &ex) {
            cerr << "Exception caught: " << ex.what();
            std::cout << "\nPress 1 to re-enter or any key to exit: ";
            char answer;
            cin >> answer;
            if (answer == '1')
                return validDeparture(departure, trip, tripIndex, call);
            return -1;
        }
    }

    void timeExistsCheck(string &departure, Trip trip[], int tripIndex) {
        // check if the departure time is correct for the existent trip
        if (departure != trip[tripIndex].departure) {
            throw invalid_argument("Unavailable departure time.");
        }
    }

    void bookTrip(string filepath, Trip trip[]) {
        try {
            /*
                Check existance of the trip in the database
            */
            bool exists = false;
            int tripIndex;
            if (validWay(this->bookedTrip.start, this->bookedTrip.destination,
                         trip, tripIndex) == 1) {
                if (validDate(this->bookedTrip.date, trip, tripIndex, 'u') ==
                    1) {
                    if (validDeparture(this->bookedTrip.departure, trip,
                                       tripIndex, 'u') == 1) {
                        exists = true;
                    }
                }
            }

            if (exists == false) {
                throw invalid_argument("Process cancelled");
            }

            /*
                Existent trip - confirm the reservation
            */
            std::cout << "Your reservation details: " << endl;
            std::cout << "Start: " << this->bookedTrip.start << endl
                      << "Destination: " << this->bookedTrip.destination
                      << endl;
            std::cout << "Date and departure time: " << this->bookedTrip.date
                      << ", " << this->bookedTrip.departure << endl;

            std::cout << "Confirm reservation? (y/n) ";
            char ans;
            cin >> ans;
            if (ans == 'y') {
                /*
                    Print the reservation details to a data file
                */
                openFile(filepath);
                ofstream fout(filepath, ios::app);
                fout << "User: " << this->getUsername() << endl
                     << " -> Reservation: " << this->bookedTrip.start << " "
                     << this->bookedTrip.destination << " "
                     << this->bookedTrip.date << " "
                     << this->bookedTrip.departure << " " << endl;

                std::cout << "Your trip has been booked!\n"
                          << "Thank you for choosing "
                             "us!\n";
            } else {
                std::cout << "The trip has been discarded.";
            }
        } catch (const exception &e) {
            std::cout << "An error occurred: " << e.what() << endl;
            cout << "Press 1 to restart process or enter any key to exit: ";
            char answer;
            cin >> answer;
            if (answer == '1')
                bookTrip(filepath, trip);
        } catch (...) {
            std::cout << "An unexpected error occurred." << endl;
        }
    }
};