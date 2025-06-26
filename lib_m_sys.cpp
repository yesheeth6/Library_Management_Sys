#include <iostream>
#include <sqlite3.h>
#include <string>
#include <vector>
#include <algorithm>
#include <map>
#include <queue>
#include <fstream>
#include <sstream>

using namespace std;

// ================================
// SQLite Database Setup
// ================================
sqlite3* db = nullptr;

void openDatabase() {
    int rc = sqlite3_open("library.db", &db);
    if (rc) {
        cerr << "Error opening database: " << sqlite3_errmsg(db) << endl;
        exit(1);
    }
    cout << "Database opened successfully.\n";
}

void closeDatabase() {
    if (db) {
        sqlite3_close(db);
        db = nullptr;
        cout << "Database closed successfully.\n";
    }
}

void createTables() {
    const string createBooksTable = 
        "CREATE TABLE IF NOT EXISTS Books ("
        "ISBN TEXT PRIMARY KEY, "
        "Title TEXT, "
        "Author TEXT, "
        "Genre TEXT, "
        "AvailableCopies INTEGER, "
        "BorrowedCount INTEGER DEFAULT 0);";

    const string createUsersTable =
        "CREATE TABLE IF NOT EXISTS Users ("
        "UserID TEXT PRIMARY KEY, "
        "Name TEXT, "
        "UserType TEXT);";

    const string createTransactionsTable =
        "CREATE TABLE IF NOT EXISTS Transactions ("
        "TransactionID INTEGER PRIMARY KEY AUTOINCREMENT, "
        "UserID TEXT, "
        "ISBN TEXT, "
        "Action TEXT, "
        "Timestamp DATETIME DEFAULT CURRENT_TIMESTAMP, "
        "FOREIGN KEY(UserID) REFERENCES Users(UserID), "
        "FOREIGN KEY(ISBN) REFERENCES Books(ISBN));";

    char* errorMessage;

    if (sqlite3_exec(db, createBooksTable.c_str(), nullptr, nullptr, &errorMessage) != SQLITE_OK) {
        cerr << "Error creating Books table: " << errorMessage << endl;
        sqlite3_free(errorMessage);
    }

    if (sqlite3_exec(db, createUsersTable.c_str(), nullptr, nullptr, &errorMessage) != SQLITE_OK) {
        cerr << "Error creating Users table: " << errorMessage << endl;
        sqlite3_free(errorMessage);
    }

    if (sqlite3_exec(db, createTransactionsTable.c_str(), nullptr, nullptr, &errorMessage) != SQLITE_OK) {
        cerr << "Error creating Transactions table: " << errorMessage << endl;
        sqlite3_free(errorMessage);
    }

    cout << "Tables created successfully.\n";
}

// ================================
// Library Class
// ================================
class Library {
public:
    void addBook(const string& title, const string& author, const string& genre, const string& isbn, int copies);
    void addUser(const string& name, const string& userID, const string& userType);
    void borrowBook(const string& userID, const string& isbn);
    void displayBooks();
    void addBooksFromCSV(const string& filePath);
};

void Library::addBook(const string& title, const string& author, const string& genre, const string& isbn, int copies) {
    // Check if the book already exists
    const string checkSql = "SELECT COUNT(*) FROM Books WHERE ISBN = ?;";
    sqlite3_stmt* checkStmt = nullptr;

    if (sqlite3_prepare_v2(db, checkSql.c_str(), -1, &checkStmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_text(checkStmt, 1, isbn.c_str(), -1, SQLITE_STATIC);

        if (sqlite3_step(checkStmt) == SQLITE_ROW) {
            int count = sqlite3_column_int(checkStmt, 0);
            sqlite3_finalize(checkStmt);

            if (count > 0) {
                cout << "Book with ISBN " << isbn << " already exists. Skipping insertion.\n";
                return;
            }
        } else {
            cerr << "Error checking book existence: " << sqlite3_errmsg(db) << endl;
            sqlite3_finalize(checkStmt);
            return;
        }
    } else {
        cerr << "Error preparing check statement: " << sqlite3_errmsg(db) << endl;
        return;
    }

    // Insert the new book
    const string insertSql = "INSERT INTO Books (ISBN, Title, Author, Genre, AvailableCopies) VALUES (?, ?, ?, ?, ?);";
    sqlite3_stmt* insertStmt = nullptr;

    if (sqlite3_prepare_v2(db, insertSql.c_str(), -1, &insertStmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_text(insertStmt, 1, isbn.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(insertStmt, 2, title.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(insertStmt, 3, author.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(insertStmt, 4, genre.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_int(insertStmt, 5, copies);

        if (sqlite3_step(insertStmt) == SQLITE_DONE) {
            cout << "Book added successfully.\n";
        } else {
            cerr << "Error adding book: " << sqlite3_errmsg(db) << endl;
        }
        sqlite3_finalize(insertStmt);
    } else {
        cerr << "Error preparing insert statement: " << sqlite3_errmsg(db) << endl;
    }
}


void Library::addBooksFromCSV(const string& filePath) {
    ifstream file(filePath);
    if (!file.is_open()) {
        cerr << "Error: Could not open file " << filePath << endl;
        return;
    }

    string line;
    getline(file, line); // Skip the header line

    while (getline(file, line)) {
        stringstream ss(line);
        string title, author, genre, isbn, copiesStr;
        int copies;

        getline(ss, title, ',');
        getline(ss, author, ',');
        getline(ss, genre, ',');
        getline(ss, isbn, ',');
        getline(ss, copiesStr, ',');

        // Trim whitespace
        title.erase(remove_if(title.begin(), title.end(), ::isspace), title.end());
        author.erase(remove_if(author.begin(), author.end(), ::isspace), author.end());
        genre.erase(remove_if(genre.begin(), genre.end(), ::isspace), genre.end());
        isbn.erase(remove_if(isbn.begin(), isbn.end(), ::isspace), isbn.end());

        try {
            copies = stoi(copiesStr);
            addBook(title, author, genre, isbn, copies);
        } catch (const exception& e) {
            cerr << "Error processing line: " << line << " (" << e.what() << ")\n";
        }
    }

    file.close();
    cout << "Books added to the database from " << filePath << endl;
}

void Library::displayBooks() {
    const string sql = "SELECT * FROM Books;";
    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        cout << "\n=== Available Books ===\n";
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            string isbn = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
            string title = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            string author = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
            string genre = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
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

// ================================
// Main Function
// ================================
int main() {
    Library library;

    // Open database connection
    openDatabase();

    // Create tables if they don't exist
    createTables();

    // Add books from the CSV file
    library.addBooksFromCSV("large_library_dataset.csv");

    // Display all books
    library.displayBooks();

    // Close the database connection
    closeDatabase();

    return 0;
}
