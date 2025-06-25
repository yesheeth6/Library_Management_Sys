#include <iostream>
#include <sqlite3.h>
#include <string>

using namespace std;

// SQLite database pointer
sqlite3* db;

// Open database connection
void openDatabase() {
    if (sqlite3_open("library.db", &db)) {
        cerr << "Error opening database: " << sqlite3_errmsg(db) << endl;
        exit(1);
    }
    cout << "Database opened successfully.\n";
}

// Close database connection
void closeDatabase() {
    sqlite3_close(db);
    cout << "Database connection closed.\n";
}

// Test creating tables
void testCreateTables() {
    string createBooksTable = 
        "CREATE TABLE IF NOT EXISTS Books ("
        "ISBN TEXT PRIMARY KEY, "
        "Title TEXT, "
        "Author TEXT, "
        "Genre TEXT, "
        "AvailableCopies INTEGER, "
        "BorrowedCount INTEGER);";

    char* errorMessage;

    if (sqlite3_exec(db, createBooksTable.c_str(), 0, 0, &errorMessage) != SQLITE_OK) {
        cerr << "Error creating Books table: " << errorMessage << endl;
        sqlite3_free(errorMessage);
    } else {
        cout << "Books table created successfully.\n";
    }
}

// Test inserting a book
void testInsertBook() {
    string sql = "INSERT INTO Books (ISBN, Title, Author, Genre, AvailableCopies, BorrowedCount) VALUES ('12345', 'Test Book', 'John Doe', 'Fiction', 10, 0);";
    char* errorMessage;

    if (sqlite3_exec(db, sql.c_str(), 0, 0, &errorMessage) != SQLITE_OK) {
        cerr << "Error inserting book: " << errorMessage << endl;
        sqlite3_free(errorMessage);
    } else {
        cout << "Book inserted successfully.\n";
    }
}

// Test querying the Books table
void testQueryBooks() {
    string sql = "SELECT * FROM Books;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0) == SQLITE_OK) {
        cout << "\n=== Books in Database ===\n";
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            string isbn = (const char*)sqlite3_column_text(stmt, 0);
            string title = (const char*)sqlite3_column_text(stmt, 1);
            string author = (const char*)sqlite3_column_text(stmt, 2);
            string genre = (const char*)sqlite3_column_text(stmt, 3);
            int availableCopies = sqlite3_column_int(stmt, 4);
            int borrowedCount = sqlite3_column_int(stmt, 5);

            cout << "ISBN: " << isbn << ", Title: " << title
                 << ", Author: " << author << ", Genre: " << genre
                 << ", Available Copies: " << availableCopies
                 << ", Times Borrowed: " << borrowedCount << endl;
        }
        sqlite3_finalize(stmt);
    } else {
        cerr << "Error querying books: " << sqlite3_errmsg(db) << endl;
    }
}

// Test deleting a book
void testDeleteBook() {
    string sql = "DELETE FROM Books WHERE ISBN = '12345';";
    char* errorMessage;

    if (sqlite3_exec(db, sql.c_str(), 0, 0, &errorMessage) != SQLITE_OK) {
        cerr << "Error deleting book: " << errorMessage << endl;
        sqlite3_free(errorMessage);
    } else {
        cout << "Book deleted successfully.\n";
    }
}

// Main function
int main() {
    // Open database connection
    openDatabase();

    // Test database functionality
    testCreateTables();
    testInsertBook();
    testQueryBooks();
    testDeleteBook();
    testQueryBooks();

    // Close database connection
    closeDatabase();
    return 0;
}
