#include <iostream>
#include <vector>
#include <sqlite3.h> // Include SQLite library

// Function to write 2D array contents to SQLite database at index i
void writeToSQLite(int i, const std::vector<std::vector<int>>& arr, sqlite3* db) {
    char* errMsg;
    std::string query = "INSERT INTO table_name VALUES (" + std::to_string(i) + ", ";
    query += "(";

    for (size_t j = 0; j < arr[i].size(); ++j) {
        query += std::to_string(arr[i][j]);
        if (j != arr[i].size() - 1)
            query += ", ";
    }
    query += "); ";

    int rc = sqlite3_exec(db, query.c_str(), NULL, 0, &errMsg);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    }
}

// Function to get user input for array elements and call writeToSQLite function
void getUserInputAndWriteToSQLite(const std::string& sqlFilename) {
    int n;
    std::cout << "Enter the size of the square matrix (n): ";
    std::cin >> n;

    std::vector<std::vector<int>> array(n, std::vector<int>(n));
    std::cout << "Enter " << n*n << " elements to fill the matrix: ";
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            std::cin >> array[i][j];
        }
    }

    int index;
    std::cout << "Enter the index where you want to write into the SQLite database: ";
    std::cin >> index;

    sqlite3* db;
    int rc = sqlite3_open(sqlFilename.c_str(), &db);
    if (rc) {
        std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return;
    }

    // Write array content to SQLite database at specified index
    writeToSQLite(index, array, db);

    sqlite3_close(db);
    std::cout << "Data successfully written to SQLite database at index " << index << std::endl;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <database_file>" << std::endl;
        return 1;
    }

    std::string sqlFilename = argv[1];

    // Call function to get user input and write to SQLite database
    getUserInputAndWriteToSQLite(sqlFilename);

    return 0;
}
