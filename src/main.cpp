#include "Operator.h"
#include "rsa.h"

// username == e-mail
int main() {
    fstream users;
    string filepath =
        "C:\\Users\\User\\Desktop\\DataBase App - C++\\db\\users.csv";
    openFile(filepath);
    RSA rsa;

    std::cout << "------------ Welcome! ------------" << endl;
    std::cout << "Are you an operator or an user for this app? " << endl
              << "Press: " << endl;
    std::cout << " - O -> Operator" << endl << " - U -> User" << endl;

    char id;
    std::cin >> id;

    /*
        ID: User
    */
    if (id == 'U' || id == 'u') {

        User user;
        std::cout
            << "Please press 1 and the enter key to login into your "
               "account. "
               "Don't have an accout? Press 0 followed by the enter key to "
               "create one."
            << endl;
        char answer;
        std::cin >> answer;
        if (answer == '0') {
            /*
                Configure your new account by entering your e-mail address
            */
            std::cout
                << "Please enter your e-mail address below.\n An e-mail "
                   "address may contain only letters and numbers, one '@' "
                   "symbol, '.' and '_' before the '@' symbol. Username "
                   "example: buon.giorno@gmail.com"
                << endl;

            /*
                Validate the e-mail address
            */
            string username;
            if (user.validUsername(username) == true) {
                user.setUsername(username);
            }

            /*
                Configure and validate password
            */
            std::cout << endl;
            std::cout << "Please enter a password. The password must contain "
                         "at least 8 characters, an "
                         "upper letter, a special character and a digit."
                      << endl;

            string passwordCopy;
            if (user.validPassword(passwordCopy) == true) {
                std::cout
                    << endl
                    << "For confirmation, please re-enter your password: ";
                if (user.checkPassword(passwordCopy) == true) {
                    user.setPassword(passwordCopy);
                    std::cout << endl << "Password set!";
                } else {
                    std::cout << "Passwords do not match! One attempt left."
                              << endl;
                    std::cout << "Press 1 to re-enter or any key to exit."
                              << endl;
                    std::cin >> answer;
                    if (answer == '1') {
                        std::cout << "For confirmation, please re-enter your "
                                     "password: ";
                        if (user.checkPassword(passwordCopy) == true) {
                            user.setPassword(passwordCopy);
                            std::cout << "Password set!";
                        }
                    } else {
                        exitProgram();
                    }
                }
            } else {
                exitProgram();
            }

            /*
                Print new user to the db file
            */
            ofstream fout(filepath, ios::app);
            fout << user.getUsername() << "," << user.getPassword() << endl;
            std::cout << endl << "Login successful!" << endl;
            fout.close();

            /*
                Encrypt password and print to file
            */
            vector<int> passwd = rsa.encoder(user.getPassword());
            string result;
            for (int i : passwd) {
                result += to_string(i);
            }
            filepath = "C:\\Users\\User\\Desktop\\DataBase App - "
                       "C++\\db\\encryptedPasswords.csv";
            openFile(filepath);
            ofstream f(filepath, ios::app);
            f << endl << user.getUsername() << "," << result << endl;
            f.close();

        } else if (answer == '1') {
            /*
                Existent account - searching for the e-mail address
            */
            string enteredUsername;
            while (1) {
                std::cout << "Username (e-mail): ";

                std::cin >> enteredUsername;
                if (user.verifyUser(enteredUsername) == false) {
                    std::cout << "Username not found." << endl;
                    std::cout
                        << "Press 1 to re-enter username or any key to exit."
                        << endl;
                    std::cin >> answer;
                    if (answer != '1') {
                        exitProgram();
                    }
                } else {
                    user.setUsername(enteredUsername);
                    break;
                }
            }

            /*
                Password matching verification
            */
            string enteredPassword;
            while (1) {
                std::cout << "Password: ";
                enteredPassword = "";
                user.setPasswordwithStars(enteredPassword);
                if (user.verifyPassword(enteredPassword, enteredUsername) ==
                    false) {
                    std::cout << "\nIncorrect password.\n";
                    std::cout
                        << "Press 1 to re-enter password or any key to exit.\n";
                    std::cin >> answer;
                    if (answer != '1') {
                        exitProgram();
                    }
                } else {
                    user.setPassword(enteredPassword);
                    break;
                }
            }
            std::cout << endl << "Login successful!" << endl;
        } else {
            exitProgram();
        }

        /*
            Display available trips from 'booking.csv'
        */
        filepath = "C:\\Users\\User\\Desktop\\DataBase App - "
                   "C++\\input\\trips.csv";
        int n = countLines(filepath);
        Trip trip[n];
        displayAvailableTrips(n, trip);

        /*
            Book reservations and print to database file
        */
        std::cout << "\nPress '1' to book a reservation or enter any key "
                     "to exit: ";
        std::cin >> answer;
        if (answer == '1') {
            filepath = "C:\\Users\\User\\Desktop\\DataBase App - "
                       "C++\\db\\booking.csv";
            openFile(filepath);
            user.bookTrip(filepath, trip);
        } else {
            exitProgram();
        }

    }
    /*
        ID: Operator
    */
    else if (id == 'O' || id == 'o') {
        Operator worker;
        std::cout << "Please press 1 and the enter key to login into your "
                     "account. "
                     "Don't have an acount? Press 0 followed by the enter "
                     "key to create one."
                  << endl;
        char answer;
        std::cin >> answer;
        if (answer == '0') {
            /*
                Configure new account by e-mail address
            */
            std::cout
                << endl
                << "Please enter your e-mail address below.\nAn e-mail "
                   "address may contain only letters and numbers, one '@' "
                   "symbol, '.' and '_' before the '@' symbol. Username "
                   "example: buon.giorno@gmail.com"
                << endl;

            string username;
            if (worker.validUsername(username) == true) {
                worker.setUsername(username);
            } else {
                exitProgram();
            }

            std::cout << "Please enter a password. The password must contain "
                         "at least 8 characters, an "
                         "upper letter, a special character and a digit."
                      << endl;

            string passwordCopy;
            worker.validPassword(passwordCopy);
            std::cout << "For confirmation, please re-enter your password: ";
            if (worker.checkPassword(passwordCopy) == true) {
                worker.setPassword(passwordCopy);
                std::cout << endl << "Password set!";
            } else {
                std::cout << " One attempt left." << endl;
                std::cout << "Press 1 to re-enter or any key to exit." << endl;
                std::cin >> answer;
                if (answer == '1') {
                    std::cout << "For confirmation, please re-enter your "
                                 "password: ";
                    if (worker.checkPassword(passwordCopy) == true) {
                        worker.setPassword(passwordCopy);
                        std::cout << "\nPassword set!\n";
                        /*
                            Print new operator account details to data file
                        */
                        ofstream fout(filepath, ios::app);
                        fout << worker.getUsername() << ","
                             << worker.getPassword() << endl;
                        worker.login();
                    } else {
                        exitProgram();
                    }
                } else {
                    exitProgram();
                }
            }

            vector<int> passwd = rsa.encoder(worker.getPassword());
            string result;
            for (int i : passwd) {
                result += to_string(i);
            }
            filepath = "C:\\Users\\User\\Desktop\\DataBase App - "
                       "C++\\db\\encryptedPasswords.csv";
            openFile(filepath);
            ofstream f(filepath, ios::app);
            f << endl << worker.getUsername() << "," << result << endl;
            f.close();

        } else if (answer == '1') {
            openFile(filepath);
            /*
                Search for existent operator account
            */
            string enteredUsername;
            while (1) {
                std::cout << "Username (e-mail): ";

                std::cin >> enteredUsername;
                if (worker.verifyUser(enteredUsername) == false) {
                    std::cout << "Username not found." << endl;
                    std::cout
                        << "Press 1 to re-enter username or any key to exit."
                        << endl;
                    std::cin >> answer;
                    if (answer == '1') {
                        exitProgram();
                    }
                } else {
                    worker.setUsername(enteredUsername);
                    break;
                }
            }

            string enteredPassword;
            while (1) {
                std::cout << "Password: ";
                enteredPassword = "";
                worker.setPasswordwithStars(enteredPassword);
                if (worker.verifyPassword(enteredPassword, enteredUsername) ==
                    false) {
                    std::cout << endl << "Incorrect password." << endl;
                    std::cout
                        << "Press 1 to re-enter password or any key to exit."
                        << endl;
                    std::cin >> answer;
                    if (answer == '1') {
                        exitProgram();
                    }
                } else {
                    worker.setPassword(enteredPassword);
                    break;
                }
            }
            std::cout << endl << "Login successful!" << endl;
        } else {
            exitProgram();
        }
        /*
            Add or delete a trip form data file
        */
        filepath = "C:\\Users\\User\\Desktop\\DataBase App - "
                   "C++\\input\\trips.csv";
        int n = countLines(filepath);
        Trip trip[n];
        displayAvailableTrips(n, trip);
        worker.editTripFile(filepath, n, trip);
    } else {
        exitProgram();
    }

    return 0;
}
