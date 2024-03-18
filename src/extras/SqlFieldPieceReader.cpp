// #include <iostream>
// #include <vector>
// #include <sqlite3.h>

// // Function to return the 2D array at a specified index from SQL file
// std::vector<std::vector<int>> getArrayAtIndexFromSQL(const std::string& filename, int index) {
//     std::vector<std::vector<int>> result;

//     // Open SQLite database
//     sqlite3* db;
//     if (sqlite3_open(filename.c_str(), &db) != SQLITE_OK) {
//         std::cerr << "Error opening database: " << sqlite3_errmsg(db) << std::endl;
//         sqlite3_close(db);
//         return result;
//     }

//     // Prepare SQL statement
//     sqlite3_stmt* stmt;
//     const char* sql = "SELECT * FROM table_name WHERE index_column = ?";
//     if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
//         std::cerr << "Error preparing statement: " << sqlite3_errmsg(db) << std::endl;
//         sqlite3_close(db);
//         return result;
//     }

//     // Bind index parameter
//     if (sqlite3_bind_int(stmt, 1, index) != SQLITE_OK) {
//         std::cerr << "Error binding parameter: " << sqlite3_errmsg(db) << std::endl;
//         sqlite3_finalize(stmt);
//         sqlite3_close(db);
//         return result;
//     }

//     // Execute SQL statement
//     while (sqlite3_step(stmt) == SQLITE_ROW) {
//         int numColumns = sqlite3_column_count(stmt);
//         std::vector<int> row;
//         for (int i = 1; i < numColumns; ++i) { // Skip index column
//             row.push_back(sqlite3_column_int(stmt, i));
//         }
//         result.push_back(row);
//     }

//     // Finalize statement and close database
//     sqlite3_finalize(stmt);
//     sqlite3_close(db);

//     return result;
// }

// int main() {
//     // SQL file name
// std::string sqlFilename = "C:/Users/tomer/Documents/school/cpProject/DB/FieldPieces.sql";

//     // Get index from user
//     int index;
//     std::cout << "Enter index: ";
//     std::cin >> index;

//     // Retrieve the 2D array at the specified index from SQL file
//     std::vector<std::vector<int>> result = getArrayAtIndexFromSQL(sqlFilename, index);

//     // Print the retrieved 2D array
//     if (!result.empty()) {
//         std::cout << "2D Array at index " << index << ":" << std::endl;
//         for (const auto& row : result) {
//             for (int element : row) {
//                 std::cout << element << " ";
//             }
//             std::cout << std::endl;
//         }
//     } else {
//         std::cout << "No data found for index " << index << std::endl;
//     }

//     return 0;
// }
