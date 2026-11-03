"# Data-Structure-microproject" 
# Lab Access Verification System

## 📌 Project Overview

The **Lab Access Verification System** is a C++ Data Structures microproject designed to manage and verify access to different laboratory zones.

The system verifies users using **NFC card ID and fingerprint ID**, checks their permissions, handles access requests, manages security lockdowns, records access activities, and provides an option to undo the latest administrative action.

The project demonstrates how different **Data Structures and Algorithms** can be combined to solve a real-world security and access-control problem.

---

## 🎯 Problem Statement

In laboratories and research facilities, different users may have different access permissions. Manual access management can be slow and difficult to track.

This project provides a computerized system that:

- Identifies users using NFC card IDs.
- Verifies fingerprint IDs.
- Checks permission for specific laboratory zones.
- Handles multiple access requests using a queue.
- Records access activities in an audit log.
- Allows administrators to grant or revoke permissions.
- Supports emergency lockdown of zones.
- Allows the last administrative action to be undone.

---

## 💡 Objectives

- Implement a practical application of Data Structures.
- Provide fast user identification using a Hash Table.
- Manage access requests using a Queue.
- Maintain administrative actions using a Stack.
- Store and search audit logs using a Linked List.
- Search users efficiently using Binary Search.
- Sort access logs using Insertion Sort.
- Store user and access information using files.

---

## 🧩 Data Structures and Algorithms Used

### 1. Hash Table

A Hash Table is used to find users using their **NFC card ID**.

```text
NFC Card ID → Hash Function → User
```

This provides fast average-case lookup for access verification.

---

### 2. Queue

A **Queue** is used to store pending access requests.

It follows:

```text
FIFO – First In, First Out
```

When multiple users request access, their requests are processed in the same order in which they were received.

---

### 3. Stack

A **Stack** is used to store administrative actions such as:

- Grant permission
- Revoke permission
- Activate lockdown
- Remove lockdown

It follows:

```text
LIFO – Last In, First Out
```

Therefore, the most recent administrative action can be undone first.

---

### 4. Linked List

A Linked List is used to maintain the access audit log.

Each log contains information such as:

- Time
- User ID
- Zone
- Result
- Reason

This allows the system to maintain a history of access attempts.

---

### 5. Binary Search

Users are maintained in sorted order according to their User ID.

Binary Search is used to find a user efficiently by their ID.

```text
Low → Middle → High
```

---

### 6. Insertion Sort

Insertion Sort is used to display the access logs sorted according to User ID.

---

## 🔐 Access Verification

When a user requests access, the system checks:

1. Whether the NFC card exists.
2. Whether the fingerprint matches.
3. Whether the requested zone is under lockdown.
4. Whether the user has permission for that zone.

If all conditions are satisfied:

```text
ACCESS GRANTED
```

Otherwise:

```text
ACCESS DENIED
```

The system also records the reason for denial.

---

## 🏢 Laboratory Zones

The system contains four zones:

| Zone | Name |
|---|---|
| A | General Research |
| B | Chemical/Biological Research |
| C | High-Security Area |
| D | Equipment Room |

Each user can have different permissions for these zones.

---

## ⚙️ Main Features

### 👤 User Registration

Administrators can register a new user with:

- User ID
- Name
- Role
- NFC Card ID
- Fingerprint ID

The system prevents duplicate User IDs and NFC IDs.

### 🔑 Zone Access Request

Users provide:

- NFC Card ID
- Fingerprint ID
- Zone

The system verifies the credentials and permissions.

### 📋 Request Queue

Access requests can be placed into a queue and processed later.

### 👥 View Users

Displays all registered users and their permitted zones.

### 🔎 Find User

Searches for a user using their User ID.

### 🚨 Lockdown

Administrators can activate or remove lockdown for a particular zone.

When a zone is under lockdown, access is denied even if the user normally has permission.

### ↩️ Undo Last Action

The system uses a Stack to undo the latest administrative action.

### 📝 Audit Logs

Every access attempt and important administrative action is recorded.

The log contains:

```text
Time | User ID | Zone | Result | Reason
```

### 🔍 Search Logs

Access logs can be searched using a User ID.

### 📊 Sorted Logs

Logs can be displayed sorted by User ID using Insertion Sort.

---

## 🖥️ Menu Options

The program provides the following menu:

```text
=== LAB ACCESS VERIFICATION ===

1. Register user
2. Request zone access
3. Add request to queue
4. Process queued requests
5. View users
6. Find user by ID
7. View zones
8. Grant permission
9. Revoke permission
10. Activate lockdown
11. Remove lockdown
12. Undo last admin action
13. View admin actions
14. View access logs
15. Search logs by user ID
16. View logs sorted by user ID
0. Save and exit
```

---

## 💾 File Handling

The project uses files to preserve information between program executions.

### `users.txt`

Stores registered user information such as:

```text
User ID
Name
Role
NFC ID
Fingerprint ID
Zone Permissions
```

### `access_log.txt`

Stores access and administrative activity logs.

When the program starts, existing information is loaded from these files.

When the program exits using option `0`, the current user data is saved.

---

## 🛠️ Technologies Used

- **Language:** C++
- **Concept:** Data Structures and Algorithms
- **Compiler:** GCC / MinGW
- **IDE:** Visual Studio Code
- **File Handling:** C++ File Streams
- **Version Control:** Git & GitHub

---

## ▶️ How to Run

### Step 1: Clone the Repository

```bash
git clone https://github.com/karthiknaik777111-eng/Data-Structure-microproject.git
```

### Step 2: Open the Project

Open the project folder in **Visual Studio Code**.

### Step 3: Compile

Using g++:

```bash
g++ micro.cpp -o micro
```

### Step 4: Run

On Windows:

```bash
micro.exe
```

Or:

```bash
.\micro.exe
```

---

## 📂 Project Structure

```text
Data-Structure-microproject/
│
├── micro.cpp
├── README.md
├── users.txt
└── access_log.txt
```

`users.txt` and `access_log.txt` are generated/used by the program for persistent data.

---

## 📈 Data Structure Mapping

| Requirement | Data Structure / Algorithm |
|---|---|
| Find user by NFC | Hash Table |
| Manage pending requests | Queue |
| Undo latest action | Stack |
| Store audit logs | Linked List |
| Find user by ID | Binary Search |
| Sort logs by User ID | Insertion Sort |
| Store persistent data | File Handling |

---

## 🌟 Advantages

- Fast user identification.
- Supports multiple laboratory zones.
- Provides permission-based access.
- Maintains an access history.
- Supports emergency lockdown.
- Provides undo functionality for administrative actions.
- Demonstrates multiple Data Structures in one real-world application.

---

## 🔮 Future Enhancements

The current project is a software simulation. It can be extended in the future by integrating:

- Real NFC card readers.
- Real fingerprint sensors.
- RFID-based access control.
- Automatic door locking systems.
- Graphical User Interface.
- Database connectivity.
- Network-based monitoring.
- Real-time security notifications.

---

## 👥 Team Members

Add your team members below:

- **Karthik M.S** – Team Member
- **Prasan** – Team Member
- **shashank** – Team Member
- **Tarun** – Team Member
- **Sadashiv mali** – Team Member
---- **Jevan** – Team Member

## 📚 Conclusion

The **Lab Access Verification System** demonstrates the practical application of Data Structures and Algorithms in a real-world laboratory security scenario.

By combining **Hash Table, Queue, Stack, Linked List, Binary Search, Insertion Sort, and File Handling**, the project provides an organized approach to user authentication, access control, request management, security lockdown, administrative actions, and audit logging.

This project shows how fundamental Data Structures can be integrated together to build a practical and functional software solution.