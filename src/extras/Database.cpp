#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <sstream>

// Function to read a 2D array from the user
std::vector<std::vector<int>> read2DArray() {
    int size;
    std::cout << "Enter number of size: ";
    std::cin >> size;


    std::vector<std::vector<int>> array(size, std::vector<int>(size));
    std::cout << "Enter the elements of the array:\n";
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            std::cin >> array[i][j];
        }
    }
    return array;
}

// Function to write the database to a file
void writeDatabaseToFile(const std::map<int, std::vector<std::vector<int>>> &database, const std::string &filename) {
    std::ofstream outFile(filename);
    if (!outFile) {
        std::cerr << "Error opening file for writing: " << filename << "\n";
        return;
    }

    for (const auto &entry : database) {
        int id = entry.first;
        const auto &array = entry.second;
        int rows = array.size();
        int cols = array[0].size();

        outFile << id << " " << rows << " " << cols << "\n";
        for (const auto &row : array) {
            for (int element : row) {
                outFile << element << " ";
            }
            outFile << "\n";
        }
    }
    outFile.close();
}

// Function to read the database from a file
std::map<int, std::vector<std::vector<int>>> readDatabaseFromFile(const std::string &filename) {
    std::map<int, std::vector<std::vector<int>>> database;
    std::ifstream inFile(filename);
    if (!inFile) {
        std::cerr << "Error opening file for reading: " << filename << "\n";
        return database;
    }

    int id, rows, cols;
    while (inFile >> id >> rows >> cols) {
        std::vector<std::vector<int>> array(rows, std::vector<int>(cols));
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                inFile >> array[i][j];
            }
        }
        database[id] = array;
    }
    inFile.close();
    return database;
}

// Function to display a 2D array
void display2DArray(const std::vector<std::vector<int>> &array) {
    for (const auto &row : array) {
        for (const auto &element : row) {
            std::cout << element << " ";
        }
        std::cout << "\n";
    }
}

int main() {
    const std::string filename = "../../DB/FieldPieces.txt";
    std::map<int, std::vector<std::vector<int>>> database = readDatabaseFromFile(filename);
    
    int choice, id;
    while (true) {
        std::cout << "Menu:\n";
        std::cout << "1. Read and store a new 2D array\n";
        std::cout << "2. Display a 2D array from the database\n";
        std::cout << "3. Save and exit\n";
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        if (choice == 1) {
            std::cout << "Enter ID for the new 2D array: ";
            std::cin >> id;
            std::vector<std::vector<int>> array = read2DArray();
            database[id] = array;
        } else if (choice == 2) {
            std::cout << "Enter ID of the 2D array to display: ";
            std::cin >> id;
            if (database.find(id) != database.end()) {
                display2DArray(database[id]);
            } else {
                std::cout << "No array found with ID " << id << ".\n";
            }
        } else if (choice == 3) {
            writeDatabaseToFile(database, filename);
            break;
        } else {
            std::cout << "Invalid choice. Please try again.\n";
        }
    }

    return 0;
}
