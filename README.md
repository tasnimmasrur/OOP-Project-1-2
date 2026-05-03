 🚀 Intelligent User Management & Recommendation Engine (C++)

 🌟 Overview

This project is a highly modular, console-based User Management System built using modern Object-Oriented Programming (OOP) principles in C++.

It combines:

* 🔐 Secure authentication
* 👥 Dynamic user handling (Normal & VIP)
* 🎯 Rule-based recommendation engine
* 📊 Real-time system analytics

Designed as an academic project, but structured with **real-world system design practices**.

 🧠 Core Highlights

 🔐 Authentication & Security

* Secure login system with credentials
* Recovery PIN-based password reset
* Session management (login/logout)
* Protected operations (admin-only access)

 👥 Advanced User System

* Supports Normal and VIP users
* Interest-based profiling
* Dynamic interest management

| Feature       | Normal User | VIP User  |
| ------------- | ----------- | --------- |
| Max Interests | 3           | Unlimited |
| Logging       | ❌           | ✅        |
| Priority      | Normal      | High      |

🎯 Rule-Based Recommendation Engine

A lightweight AI-inspired system that generates recommendations using condition-based rules.

Logic Flow:

User Interest → Rule Match → Recommendation Output

✔ Easily extendable
✔ Decoupled design using polymorphism

 🔍 Smart Discovery Features

* 🔎 Search users by name (case-insensitive)
* 🎯 Filter users by interest
* 🤝 Match users based on shared interests (scoring system)

 📊 Built-in Analytics Dashboard

* Total users (Normal vs VIP)
* Average interests per user
* Most active user
* Top trending interest
* Interest ranking (Top 3)

 ⚙️ System Utilities

* Import users from external file
* Auto-save system (persistent storage)
* OOP Demonstration Module:

  * Friend function
  * Operator overloading (`<<`, `==`, `>`)
  * `this` pointer usage

🏗️ System Architecture

 🔹 Design Patterns Used

* Singleton Pattern → Centralized Engine controller
* Factory Pattern → Rule creation
* Template Programming → Generic storage (`Storage<T>`)

 🔹 Class Structure

Engine (Singleton)
│
├── User (Base Class)
│   ├── PremiumUser
│   │   └── VIPUser (with logging)
│
├── Rule (Abstract)
│   └── BasicRule
│
└── Storage<T> (Template Container)

📂 Project Structure

├── main.cpp
├── Engine.cpp / Engine.h
├── User.cpp / User.h
├── Rule.cpp / Rule.h
├── Storage.h
├── Exception.h
├── Logger.h
├── Makefile
└── README.md


 ⚙️ Build & Run

 🔧 Requirements

* C++11 or higher
* g++ compiler
* Windows environment (for console UI)

 🛠️ Compile

```bash
make

▶️ Run

bash
./oop4.exe

 💾 Data Persistence

The system automatically stores and retrieves data using text files:

| File               | Purpose              |
| ------------------ | -------------------- |
| `auth_data.txt`    | User credentials     |
| `rules_data.txt`   | Recommendation rules |
| `normal_users.txt` | Normal users         |
| `vip_users.txt`    | VIP users            |
| `interest_*.txt`   | Interest grouping    |

✔ Fully automated save system after each operation

 🧪 OOP Concepts Demonstrated

* ✔ Inheritance & Multi-level Inheritance
* ✔ Polymorphism (virtual functions)
* ✔ Encapsulation
* ✔ Operator Overloading
* ✔ Friend Functions
* ✔ Templates (Generic Programming)
* ✔ Singleton Design Pattern

⚠️ Limitations

* Uses raw pointers (manual memory management)
* Platform-dependent (`windows.h`)
* No database integration (file-based only)
* Custom exception class (not derived from `std::exception`)

 🔮 Future Enhancements

* 🔄 Replace raw pointers with smart pointers (`unique_ptr`)
* 🌐 Cross-platform support (remove Windows dependency)
* 🗄️ Database integration (SQLite / MySQL)
* 🤖 Upgrade recommendation system (ML-based)
* 🎨 GUI version (Qt / Web-based dashboard)


 Author :

Tasnim Masrur
C++ | OOP | System Design Enthusiast

 Final Note

This project demonstrates how fundamental OOP concepts can be used to build a **scalable, modular, and intelligent system**—bridging academic knowledge with real-world application design.


⭐ If you find this project useful, consider giving it a star!
