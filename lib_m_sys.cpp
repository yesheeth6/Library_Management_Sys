#include <iostream>
#include <map>
#include <vector>
#include <queue>
#include <string>
#include <algorithm>
#include <set>

using namespace std;

// ================================
// Book Class with Analytics
// ================================
class Book {
public:
    string title, author, genre, ISBN;
    int availableCopies, borrowedCount;
    queue<string> waitlist;

    // Default constructor
    Book() : title(""), author(""), genre(""), ISBN(""), availableCopies(0), borrowedCount(0) {}

    // Parameterized constructor
    Book(string t, string a, string g, string isbn, int copies)
        : title(t), author(a), genre(g), ISBN(isbn), availableCopies(copies), borrowedCount(0) {}

    // Display book information
    void displayBookInfo() {
        cout << "Title: " << title << ", Author: " << author
             << ", Genre: " << genre << ", ISBN: " << ISBN
             << ", Available Copies: " << availableCopies
             << ", Times Borrowed: " << borrowedCount << endl;
    }
};

// ================================
// User Base Class with Borrowing Analytics
// ================================
class User {
public:
    string name, userID;
    vector<string> borrowedBooks;
    map<string, int> genrePreference;

    // Constructor
    User(string n, string id) : name(n), userID(id) {}

    // Virtual function for borrowing limit (to be overridden by derived classes)
    virtual int borrowingLimit() = 0;

    // Borrow a book and update genre preference
    void borrowBook(string isbn, string genre) {
        borrowedBooks.push_back(isbn);
        genrePreference[genre]++;
    }

    // Return a book
    void returnBook(string isbn) {
        auto it = find(borrowedBooks.begin(), borrowedBooks.end(), isbn);
        if (it != borrowedBooks.end()) borrowedBooks.erase(it);
    }
};

// ================================
// Derived Classes for User Types
// ================================
class Student : public User {
public:
    Student(string n, string id) : User(n, id) {}
    int borrowingLimit() override { return 5; } // Students can borrow up to 5 books
};

class Staff : public User {
public:
    Staff(string n, string id) : User(n, id) {}
    int borrowingLimit() override { return 10; } // Staff can borrow up to 10 books
};

// ================================
// Library Class with Enhanced Features
// ================================
class Library {
private:
    map<string, Book> books;      // Map of books, key: ISBN
    map<string, User*> users;    // Map of users, key: User ID

public:
    // Add a new book to the library
    void addBook(string title, string author, string genre, string isbn, int copies) {
        books[isbn] = Book(title, author, genre, isbn, copies);
    }

    // Add a new user to the library system
    void addUser(User* user) {
        users[user->userID] = user;
    }

    // Borrow a book
    void borrowBook(string userID, string isbn) {
        if (books.find(isbn) == books.end()) {
            cout << "Book not found.\n";
            return;
        }
        Book &book = books[isbn];
        User *user = users[userID];

        // Check if the book is available and user has not reached the borrowing limit
        if (book.availableCopies > 0 && user->borrowedBooks.size() < user->borrowingLimit()) {
            book.availableCopies--;
            book.borrowedCount++;
            user->borrowBook(isbn, book.genre);
            cout << "Book borrowed successfully.\n";
        } else if (book.availableCopies == 0) {
            cout << "Book not available. Adding to waitlist.\n";
            book.waitlist.push(userID);
        } else {
            cout << "Borrowing limit reached.\n";
        }
    }

    // Return a book
    void returnBook(string userID, string isbn) {
        if (books.find(isbn) == books.end()) {
            cout << "Book not found.\n";
            return;
        }
        Book &book = books[isbn];
        User *user = users[userID];

        user->returnBook(isbn);
        book.availableCopies++;

        // Notify the next user in the waitlist if the book becomes available
        if (!book.waitlist.empty()) {
            string nextUser = book.waitlist.front();
            book.waitlist.pop();
            cout << "Notifying " << nextUser << " for the available book.\n";
        }
    }

    // Recommend books to a user based on preferences and popularity
    void recommendBooks(string userID) {
        User *user = users[userID];
        cout << "Recommended Books for " << user->name << ":\n";

        vector<pair<string, int>> recommendations;
        for (auto &[isbn, book] : books) {
            int score = user->genrePreference[book.genre] + book.borrowedCount;
            recommendations.push_back({isbn, score});
        }

        // Sort recommendations by score
        sort(recommendations.begin(), recommendations.end(),
             [](auto &a, auto &b) { return a.second > b.second; });

        // Display recommended books
        for (auto &[isbn, _] : recommendations) {
            books[isbn].displayBookInfo();
        }
    }

    // Display analytics such as the top borrowed books
    void displayAnalytics() {
        cout << "\n=== Library Analytics ===\n";
        cout << "Top 5 Most Borrowed Books:\n";

        vector<pair<string, int>> bookStats;
        for (auto &[isbn, book] : books) {
            bookStats.push_back({isbn, book.borrowedCount});
        }

        // Sort by borrowing count
        sort(bookStats.begin(), bookStats.end(),
             [](auto &a, auto &b) { return a.second > b.second; });

        for (int i = 0; i < min(5, (int)bookStats.size()); i++) {
            books[bookStats[i].first].displayBookInfo();
        }
    }
};

// ================================
// Main Function for User Interaction
// ================================
int main() {
    Library library;

    // Adding Books
    library.addBook("C++ Programming", "Bjarne Stroustrup", "Programming", "1234", 5);
    library.addBook("Clean Code", "Robert C. Martin", "Software Engineering", "5678", 2);
    library.addBook("Introduction to Algorithms", "Thomas H. Cormen", "Algorithms", "91011", 3);

    // Adding Users
    library.addUser(new Student("Alice", "S1"));
    library.addUser(new Staff("Bob", "T1"));

    // Borrowing Books
    library.borrowBook("S1", "1234");
    library.borrowBook("S1", "5678");
    library.borrowBook("T1", "91011");

    // Returning Books
    library.returnBook("S1", "1234");

    // Recommendations
    library.recommendBooks("S1");

    // Analytics
    library.displayAnalytics();

    return 0;
}
