# Library Management System

## Description

The Library Management System is a comprehensive application designed to manage books and users within a library. It uses a CSV file for bulk data import and stores records in a **SQLite database** for seamless management. This project implements core functionalities such as adding books, managing users, and borrowing books while maintaining a robust log of transactions.

Key features include:
- **Database Integration**: Powered by SQLite for efficient data storage and retrieval.
- **CSV Data Import**: Load large datasets of books into the system with ease.
- **User Management**: Supports adding and maintaining user profiles.
- **Transaction Logs**: Automatically records book borrow actions in the database.

This project is written in **C++** and utilizes the **SQLite C++ interface** for database interactions. It is well-suited for library systems in schools, colleges, or small organizations.

---

## Features

1. **Book Management**
   - Add individual books to the library database.
   - Import books from a large CSV dataset.
   - View all available books, along with metadata (ISBN, title, author, genre, etc.).

2. **User Management**
   - Add library users with unique IDs.
   - Maintain user profiles and types (e.g., student, staff).

3. **Borrow and Return System**
   - Borrow books while updating the database in real time.
   - Automatically reduce available copies and track borrowing count.
   - Log every borrowing action in the transactions table.

4. **CSV Integration**
   - Use a CSV file (`large_library_dataset.csv`) to load a collection of books in bulk.

---

## Prerequisites

Before running the project, ensure the following software and dependencies are installed:

1. **SQLite3**
   - `sqlite3.dll` (included in the project directory).
   - SQLite C++ libraries.
   
2. **Compiler**
   - GCC or MinGW (if on Windows).
   - Ensure SQLite libraries are linked during compilation.

3. **Development Environment**
   - Visual Studio Code or any C++ IDE with support for SQLite.

---

## How to Run

1. Clone or download the repository.
2. Open the terminal in the project directory.
3. Compile the program using the following command:
   ```bash
   g++ -o library_system lib_m_sys.cpp -lsqlite3
## Execute the compiled program:
./library_system
## Project Directory Structure
.vscode/                  # VS Code settings folder
output/                   # Folder for compiled executables
large_library_dataset.csv # Sample dataset for books
lib_m_sys.cpp             # Main C++ source file
library.db                # SQLite database file (generated after the first run)
sqlite3.dll               # SQLite dynamic-link library
## Future Enhancements
Add a feature for book return and fine calculation.
Enhance user authentication with login support.
Implement a graphical user interface (GUI).
Provide advanced search and filter options for books.
