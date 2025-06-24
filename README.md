# Library Management System

A feature-rich library management system built in **C++** using **Object-Oriented Programming (OOP)** principles and Standard Template Library (STL). This project allows for efficient management of books, users, borrowing processes, and insightful analytics.

---

## 🚀 Features

- **Book Management**  
  - Add, remove, and track books in the library.  
  - Maintain records of borrowed books and available copies.  
  - Implemented a waitlist system for popular books.  

- **User Management**  
  - Different user types: **Students** and **Staff** with borrowing limits.  
  - Personalized recommendations based on borrowing history and genre preferences.  

- **Analytics Dashboard**  
  - Displays top borrowed books.  
  - Tracks borrowing trends and user activity.  

---

## 🛠️ Technologies Used

- **Programming Language**: C++  
- **Libraries/Concepts**:  
  - **STL Containers**: `map`, `vector`, `queue`  
  - **Object-Oriented Programming**: Inheritance, Polymorphism, Encapsulation  

---

## 📦 Installation and Setup

1. **Clone the Repository**  
   ```bash
   git clone https://github.com/yesheeth6/Library_Management_Sys.git
   cd Library-Management-System
## Compile the Code
Use any C++ compiler like g++:
g++ library_management.cpp -o library_management
## Run the Program
./library_management
# 📚 How to Use
## Add books to the library:
Specify book details (title, author, genre, ISBN, copies).
## Register users:
Add a student or staff member by providing their name and ID.
## Borrow and return books:
Borrow books based on availability and user type limits.
Return books to free up copies and notify waitlisted users.
## View recommendations and analytics:
Get book recommendations personalized to user preferences.
Analyze borrowing trends and popular genres.
#📝 Example Commands
## Add a Book:
Add "C++ Programming" with 5 copies:
library.addBook("C++ Programming", "Bjarne Stroustrup", "Programming", "1234", 5);
### Add a User:
Add a student named "Alice":
library.addUser(new Student("Alice", "S1"));
## Borrow a Book:
Alice borrows the book with ISBN 1234:
library.borrowBook("S1", "1234");
## View Analytics:
Display top borrowed books:
library.displayAnalytics();
