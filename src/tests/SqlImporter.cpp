#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

// Function to create SQL file
void createSQLFile(const std::vector<std::string>& tableNames, const std::vector<std::vector<std::string>>& columnNames, const std::string& filename) {
    std::ofstream outputFile(filename);

    if (!outputFile.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }

    // Write SQL commands to the file
    for (size_t i = 0; i < tableNames.size(); ++i) {
        outputFile << "CREATE TABLE " << tableNames[i] << " (\n";

        for (size_t j = 0; j < columnNames[i].size(); ++j) {
            outputFile << "\t" << columnNames[i][j] << " "; // Assuming column definition format is provided externally
            if (j != columnNames[i].size() - 1) {
                outputFile << ",";
            }
            outputFile << "\n";
        }

        outputFile << ");\n\n";
    }

    outputFile.close();
}

// Function to read SQL file and extract table names and column names
void readSQLFile(const std::string& filename, std::vector<std::string>& tableNames, std::vector<std::vector<std::string>>& columnData) {
    std::ifstream inputFile(filename);

    if (!inputFile.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }

    std::string line;
    std::string tableName;
    std::vector<std::string> columns;

    while (std::getline(inputFile, line)) {
        std::istringstream iss(line);
        std::string word;

        if (line.find("CREATE TABLE") != std::string::npos) {
            if (!tableName.empty()) {
                tableNames.push_back(tableName);
                columnData.push_back(columns);
                columns.clear();
            }
            tableName.clear();
            iss >> word; // Skip "CREATE"
            iss >> word; // Skip "TABLE"
            iss >> tableName; // Get table name
            // Remove trailing comma if present
            if (tableName.back() == ',') {
                tableName.pop_back();
            }
        } else if (line.find("(") == std::string::npos && line.find(")") == std::string::npos && !line.empty()) {
            std::size_t pos = line.find(" ");
            if (pos != std::string::npos) {
                std::string columnName = line.substr(1, pos - 1);
                // Handle data type
                // Assuming here that data type is the second word in the line
                std::string dataType;
                iss >> dataType;
                columns.push_back(columnName + " " + dataType);
            }
        }
    }

    // Add the last table and its columns
    if (!tableName.empty()) {
        tableNames.push_back(tableName);
        columnData.push_back(columns);
    }

    inputFile.close();
}

int main() {
    // Example data
    std::vector<std::string> tableNames = {"Table1", "Table2"};
    std::vector<std::vector<std::string>> columnData = {{"Column1 INT", "Column2 VARCHAR(255)"}, {"Column3 FLOAT", "Column4 DATE"}};

    // Call the function to create SQL file
    createSQLFile(tableNames, columnData, "output.sql");

    // Call the function to read SQL file
    std::vector<std::string> readTableNames;
    std::vector<std::vector<std::string>> readColumnData;
    readSQLFile("output.sql", readTableNames, readColumnData);

    // Output read data (for testing purposes)
    for (size_t i = 0; i < readTableNames.size(); ++i) {
        std::cout << "Table Name: " << readTableNames[i] << std::endl;
        std::cout << "Columns: ";
        for (const auto& column : readColumnData[i]) {
            std::cout << column << ", ";
        }
        std::cout << std::endl;
    }

    return 0;
}
