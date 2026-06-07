#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <regex>
#include <limits>

using namespace std;

// Photography Booking software

struct Client
{
    string clientName;
    int phoneNumber;
    string appointmentDate;
    string completed;
    string completedDate;
    string url;
};

int numClients;
const string filePath = "C:/Users/Administrator/Desktop/clients_list.txt";

// Checking if the value is a valid date
bool isValidDate(const string &date)
{
    int year, month, day;
    istringstream ss(date);
    char delimiter;
    if (!(ss >> year >> delimiter >> month >> delimiter >> day))
    {
        return false; // If invalid date format
    }

    if (year < 1900 || month < 1 || month > 12 || day < 1 || day > 31)
    {
        return false;
    }

    if (month == 4 || month == 6 || month == 9 || month == 11)
    {
        return day <= 30;
    }

    return true; // Date is valid
}

bool isDateAfter(const string &date1, const string &date2)
{
    int year1, month1, day1;
    int year2, month2, day2;

    istringstream ss1(date1);
    char delimiter;
    ss1 >> year1 >> delimiter >> month1 >> delimiter >> day1;

    istringstream ss2(date2);
    ss2 >> year2 >> delimiter >> month2 >> delimiter >> day2;

    if (year1 < year2)
        return true;
    if (year1 == year2 && month1 < month2)
        return true;
    if (year1 == year2 && month1 == month2 && day1 < day2)
        return true;

    return false;
}

Client *addClients(int len)
{
    Client *clients = new Client[len];
    for (int i = 0; i < len; i++)
    {
        cout << "Enter details for client " << (i + 1) << ":" << endl;
        while (true)
        {
            cout << "Client Full Name (only alphabets and spaces): ";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            getline(cin, clients[i].clientName);

            regex name_pattern("^[A-Za-z ]+$");
            if (regex_match(clients[i].clientName, name_pattern))
            {
                break;
            }
            else
            {
                cout << "Invalid name. \nPlease enter a valid Full Name containing only alphabets and spaces." << endl;
            }
        }

        cout << "Phone Number: ";
        while (!(cin >> clients[i].phoneNumber))
        {
            cout << "Invalid input. \nPlease enter a valid phone number (10 digits starting with 09): ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        string temp_date;
        do
        {
            cout << "Appointment Date (YYYY/MM/DD): ";
            cin >> temp_date;
        } while (!isValidDate(temp_date));
        clients[i].appointmentDate = temp_date;

        while (true)
        {
            cout << "Is the task completed? (yes/no): ";
            cin >> clients[i].completed;

            if (clients[i].completed == "yes")
            {
                string tempCompletedDate;
                do
                {
                    cout << "Completed Date (YYYY/MM/DD): ";
                    cin >> tempCompletedDate;
                } while (!isValidDate(tempCompletedDate) || !isDateAfter(clients[i].appointmentDate, tempCompletedDate));
                clients[i].completedDate = tempCompletedDate;

                while (true)
                {
                    string url;
                    cout << "URL: ";
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    getline(cin, url);

                    regex url_pattern(R"(https?://(www\.)?instagram\.com/[\w\-\.]+(/.*)?)");
                    if (!regex_match(url, url_pattern))
                    {
                        cerr << "Invalid URL format." << endl;
                    }
                    else
                    {
                        clients[i].url = url;
                        break;
                    }
                }
                break;
            }
            else if (clients[i].completed == "no")
            {
                break;
            }
            else
            {
                cout << "Please Enter a valid Input" << endl;
            }
        }
        cout << endl;
    }
    return clients;
}
void clientNum()
{
    cout << "How many clients do you want to add? ";
    while (!(cin >> numClients) || numClients <= 0)
    {
        cout << "Invalid input. Please enter a positive integer: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}
int currentClientCounter(const string &fileName)
{
    ifstream file(fileName);
    int count = 0;
    string line;
    while (getline(file, line))
    {
        if (line.find("============== Client") != string::npos)
        {
            count++;
        }
    }
    return count;
}

void saveToText(Client *clients, int len)
{
    
    ofstream output_f(filePath, ios::app);
    if (output_f.is_open())
    {
        for (int i = 0; i < len; i++)
        {
            int currentClientCount = currentClientCounter(filePath);
            output_f << "============== Client " << (currentClientCount + 1) << " ==============" << endl;
            output_f << "Client Name: " << clients[i].clientName << endl;
            output_f << "Phone Number: " << clients[i].phoneNumber << endl;
            output_f << "Appointment Date: " << clients[i].appointmentDate << endl;
            output_f << "Completed: " << clients[i].completed << endl;
            if (clients[i].completed == "yes")
            {
                output_f << "Completed Date: " << clients[i].completedDate << endl;
                output_f << "URL: " << clients[i].url << endl;
            }
            output_f << endl;
        }
        output_f.close();
        cout << "Client saved successfully!" << endl;
    }
    else
    {
        cout << "Unable to open file for writing." << endl;
    }
}
void displayText(const string &fileName)
{
    ifstream file(fileName);
    if (file.is_open())
    {
        string text;
        while (getline(file, text))
        {
            cout << text << endl;
        }
        file.close();
    }
    else
    {
        cout << "Unable to open the file." << endl;
    }
}
void editClient(const string &fileName)
{
    fstream file(fileName, ios::in | ios::out);
    if (!file.is_open())
    {
        cout << "Unable to open the file." << endl;
        return;
    }

    string line, updatedData = "";
    int clientNumber, currentClient = 0;
    displayText(fileName);
    cout << "Enter the client number to edit: ";
    cin >> clientNumber;

    while (getline(file, line))
    {
        if (line.find("============== Client ") != string::npos)
        {
            currentClient++;
        }

        if (currentClient == clientNumber)
        {
            string clientName, phoneNumber, appointmentDate, completed, completedDate, url;

            // Parse existing client data
            while (getline(file, line) && line.find("============== Client ") == string::npos)
            {
                if (line.find("Client Name:") != string::npos)
                    clientName = line.substr(line.find(":") + 2);
                else if (line.find("Phone Number:") != string::npos)
                    phoneNumber = line.substr(line.find(":") + 2);
                else if (line.find("Appointment Date:") != string::npos)
                    appointmentDate = line.substr(line.find(":") + 2);
                else if (line.find("Completed:") != string::npos)
                    completed = line.substr(line.find(":") + 2);
                else if (line.find("Completed Date:") != string::npos)
                    completedDate = line.substr(line.find(":") + 2);
                else if (line.find("URL:") != string::npos)
                    url = line.substr(line.find(":") + 2);
            }

            // asking the user for each field to update
            string input;
            cout << "Current Client Name: " << clientName << ".\nEnter new name (or press Enter to keep current): ";
            cin.ignore();
            getline(cin, input);
            if (!input.empty())
                clientName = input;

            cout << "Current Phone Number: " << phoneNumber << ".\nEnter new phone number (or press Enter to keep current): ";
            getline(cin, input);
            if (!input.empty())
                phoneNumber = input;

            cout << "Current Appointment Date: " << appointmentDate << ".\nEnter new date (or press Enter to keep current): ";
            getline(cin, input);
            if (!input.empty() && isValidDate(input))
                appointmentDate = input;

            cout << "Is the task completed? Current status: " << completed << ".\nEnter 'yes' or 'no' (or press Enter to keep current): ";
            getline(cin, input);
            if (!input.empty())
            {
                completed = input;
                if (completed == "yes")
                {
                    cout << "Enter Completed Date (YYYY/MM/DD): ";
                    getline(cin, input);
                    if (!input.empty() && isValidDate(input) && isDateAfter(appointmentDate, input))
                        completedDate = input;

                    cout << "Enter URL: ";
                    getline(cin, input);
                    if (!input.empty())
                        url = input;
                }
                else
                {
                    completedDate = "";
                    url = "";
                }
            }

            // Rebuild the updated client data
            stringstream clientInfo;
            clientInfo << "============== Client " << clientNumber << " ==============\n"
                       << "Client Name: " << clientName << "\n"
                       << "Phone Number: " << phoneNumber << "\n"
                       << "Appointment Date: " << appointmentDate << "\n"
                       << "Completed: " << completed << "\n";
            if (completed == "yes")
            {
                clientInfo << "Completed Date: " << completedDate << "\n"
                           << "URL: " << url << "\n";
            }
            clientInfo << endl;

            updatedData += clientInfo.str();
        }
        else
        {
            updatedData += line + "\n";
        }
    }

    file.close();

    // Write updated data back to the file
    ofstream outFile(fileName, ios::trunc);
    if (outFile.is_open())
    {
        outFile << updatedData;
        outFile.close();
        cout << "Client updated successfully!" << endl;
    }
    else
    {
        cout << "Unable to save the updated data." << endl;
    }
}
void removeClient(const string &fileName)
{
    fstream file(fileName, ios::in | ios::out);
    if (!file.is_open())
    {
        cout << "Unable to open the file." << endl;
        return;
    }

    string line, updatedData = "";
    int clientNumber, currentClient = 0;
    cout << "Enter the client number to remove: ";
    cin >> clientNumber;

    while (getline(file, line))
    {
        if (line.find("============== Client ") != string::npos)
        {
            currentClient++;
        }

        if (currentClient == clientNumber)
        {
            // Skip lines for the client to be removed
            while (getline(file, line) && line.find("============== Client ") == string::npos)
            {
                // Do nothing (skip lines)
            }
        }
        else
        {
            updatedData += line + "\n";
        }
    }

    file.close();

    // Write updated data back to the file
    ofstream outFile(fileName, ios::trunc);
    if (outFile.is_open())
    {
        outFile << updatedData;
        outFile.close();
        cout << "Client removed successfully!" << endl;
    }
    else
    {
        cout << "Unable to save the updated data." << endl;
    }
}
int main()
{
    Client *clients = nullptr; // Declaring clients as null pointer
    int choice;
    while (choice != 5)
    {
        cout << "============== Welcome to the Photography Booking Software ==============" << endl;
        cout << "What do you want to do? (1-4)" << endl;
        cout << "1. Add Client" << endl;
        cout << "2. Remove Client" << endl;
        cout << "3. Edit Client" << endl;
        cout << "4. Display client list" << endl;
        cout << "5. Exit" << endl;

        cin >> choice;
        while (choice < 1 || choice > 5)
        {
            cout << "Invalid input. Please enter a number between 1 and 5: ";
            cin >> choice;
        }

        switch (choice)
        {
        case 1:
            clientNum();
            clients = addClients(numClients);
            saveToText(clients, numClients);
            delete[] clients;
            break;
        case 2:
            removeClient(filePath);
            break;
        case 3:
            editClient(filePath);
            break;
        case 4:
        {
            displayText(filePath);
            break;
        }
        case 5:
            break;
        default:
            cout << "Invalid input." << endl;
            break;
        }
    }

    return 0;
}
