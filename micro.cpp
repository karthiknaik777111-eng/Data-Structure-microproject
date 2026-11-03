#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <ctime>
#include <cctype>
#include <cstdlib>
using namespace std;

const int ZONES = 4, MAX_USERS = 100, HASH_SIZE = 31, QUEUE_SIZE = 20;

string now() {
    time_t t = time(nullptr);
    char buf[20];
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", localtime(&t));
    return buf;
}

string ask(string prompt) {
    string s;
    cout << prompt;
    getline(cin, s);
    return s;
}

int askNumber(string prompt) {
    string s = ask(prompt);
    return (!s.empty() && isdigit(s[0])) ? atoi(s.c_str()) : -1;
}

string zoneName(int z) { return string(1, 'A' + z); }

int zoneNumber(string s) {
    if (s.size() != 1) return -1;
    int z = toupper(s[0]) - 'A';
    return (z >= 0 && z < ZONES) ? z : -1;
}

class Credential {
    string nfc, fingerprint;
public:
    Credential(string n = "", string f = "") : nfc(n), fingerprint(f) {}
    string getNfc() { return nfc; }
    string getFingerprint() { return fingerprint; }
    bool matches(string f) { return f == fingerprint; }
};

class User {
    string id, name, role;
    Credential cred;
    bool allowed[ZONES] = {};
public:
    User(string i, string n, string r, Credential c, string bits = "0000")
        : id(i), name(n), role(r), cred(c) {
        for (int z = 0; z < ZONES && z < (int)bits.size(); z++) allowed[z] = (bits[z] == '1');
    }
    string getId() { return id; }
    string getName() { return name; }
    string getRole() { return role; }
    Credential getCred() { return cred; }
    bool canEnter(int z) { return allowed[z]; }
    void setAccess(int z, bool value) { allowed[z] = value; }
    string bits() {
        string s;
        for (int z = 0; z < ZONES; z++) s += allowed[z] ? '1' : '0';
        return s;
    }
    string zonesList() {
        string s;
        for (int z = 0; z < ZONES; z++) if (allowed[z]) s += zoneName(z) + " ";
        return s.empty() ? "none" : s;
    }
};

class Zone {
public:
    string name;
    bool locked = false;
};

class HashTable {
    struct Node { string key; User* user; Node* next; };
    Node* table[HASH_SIZE] = {};
    int hash(string key) {
        int h = 0;
        for (char c : key) h = (h * 31 + (unsigned char)c) % HASH_SIZE;
        return h;
    }
public:
    void insert(string key, User* user) {
        int i = hash(key);
        table[i] = new Node{key, user, table[i]};
    }
    User* find(string key) {
        for (Node* n = table[hash(key)]; n; n = n->next)
            if (n->key == key) return n->user;
        return nullptr;
    }
};

struct Request { string nfc, fingerprint; int zone; };

class RequestQueue {
    Request items[QUEUE_SIZE];
    int front = 0, count = 0;
public:
    bool empty() { return count == 0; }
    bool full() { return count == QUEUE_SIZE; }
    int size() { return count; }
    void enqueue(Request r) {
        items[(front + count) % QUEUE_SIZE] = r;
        count++;
    }
    Request dequeue() {
        Request r = items[front];
        front = (front + 1) % QUEUE_SIZE;
        count--;
        return r;
    }
};

struct Action { string text; char type; int zone; User* user; };

class ActionStack {
    struct Node { Action action; Node* next; };
    Node* top = nullptr;
public:
    void push(Action a) { top = new Node{a, top}; }
    bool pop(Action& a) {
        if (!top) return false;
        Node* n = top;
        a = n->action;
        top = n->next;
        delete n;
        return true;
    }
    void show() {
        if (!top) cout << "  No actions yet.\n";
        for (Node* n = top; n; n = n->next) cout << "  " << n->action.text << "\n";
    }
};

struct LogEntry { string time, userId, zone, result, reason; };

class AuditLogger {
    struct Node { LogEntry entry; Node* next; };
    Node *head = nullptr, *tail = nullptr;
    int count = 0;

    void add(LogEntry e) {
        Node* n = new Node{e, nullptr};
        if (tail) tail->next = n; else head = n;
        tail = n;
        count++;
    }
    void print(LogEntry e) {
        cout << "  " << e.time << " | " << e.userId << " | Zone " << e.zone << " | " << e.result;
        if (!e.reason.empty()) cout << " (" << e.reason << ")";
        cout << "\n";
    }
public:
    void load() {
        ifstream in("access_log.txt");
        string line;
        while (getline(in, line)) {
            if (line.empty()) continue;
            stringstream ss(line);
            LogEntry e;
            getline(ss, e.time, '|');
            getline(ss, e.userId, '|');
            getline(ss, e.zone, '|');
            getline(ss, e.result, '|');
            getline(ss, e.reason);
            add(e);
        }
    }
    void log(string userId, string zone, string result, string reason = "") {
        LogEntry e{now(), userId, zone, result, reason};
        add(e);
        ofstream out("access_log.txt", ios::app);
        out << e.time << '|' << e.userId << '|' << e.zone << '|' << e.result << '|' << e.reason << '\n';
    }
    void showAll() {
        if (!head) cout << "  Log is empty.\n";
        for (Node* n = head; n; n = n->next) print(n->entry);
    }
    void searchUser(string userId) {
        bool found = false;
        for (Node* n = head; n; n = n->next)
            if (n->entry.userId == userId) { print(n->entry); found = true; }
        if (!found) cout << "  No records for " << userId << "\n";
    }
    void showSortedByUser() {
        if (!head) { cout << "  Log is empty.\n"; return; }
        LogEntry* arr = new LogEntry[count];
        int i = 0;
        for (Node* n = head; n; n = n->next) arr[i++] = n->entry;
        for (int j = 1; j < count; j++) {
            LogEntry key = arr[j];
            int k = j - 1;
            while (k >= 0 && arr[k].userId > key.userId) { arr[k + 1] = arr[k]; k--; }
            arr[k + 1] = key;
        }
        for (int j = 0; j < count; j++) print(arr[j]);
        delete[] arr;
    }
};

class AccessController {
    User* users[MAX_USERS];
    int userCount = 0;
    Zone zones[ZONES];
    HashTable byNfc;
    RequestQueue queue;
    ActionStack actions;
    AuditLogger logger;

    User* findUser(string id) {
        int low = 0, high = userCount - 1;
        while (low <= high) {
            int mid = (low + high) / 2;
            if (users[mid]->getId() == id) return users[mid];
            if (users[mid]->getId() < id) low = mid + 1;
            else high = mid - 1;
        }
        return nullptr;
    }

    bool addUser(User* u) {
        if (userCount == MAX_USERS || findUser(u->getId()) || byNfc.find(u->getCred().getNfc())) {
            delete u;
            return false;
        }
        int i = userCount - 1;
        while (i >= 0 && users[i]->getId() > u->getId()) { users[i + 1] = users[i]; i--; }
        users[i + 1] = u;
        userCount++;
        byNfc.insert(u->getCred().getNfc(), u);
        return true;
    }

    bool deny(string userId, string zone, string reason) {
        logger.log(userId, zone, "DENIED", reason);
        cout << "  ACCESS DENIED - " << reason << "\n";
        return false;
    }

    bool checkAccess(string nfc, string fingerprint, int z) {
        string zone = zoneName(z);
        User* u = byNfc.find(nfc);
        if (!u) return deny("UNKNOWN", zone, "Invalid NFC card");
        if (!u->getCred().matches(fingerprint)) return deny(u->getId(), zone, "Fingerprint mismatch");
        if (zones[z].locked) return deny(u->getId(), zone, "Zone under lockdown");
        if (!u->canEnter(z)) return deny(u->getId(), zone, "No permission");
        logger.log(u->getId(), zone, "GRANTED");
        cout << "  ACCESS GRANTED - Welcome, " << u->getName() << "\n";
        return true;
    }

    void printUser(User* u) {
        cout << "  " << u->getId() << " | " << u->getName() << " | " << u->getRole()
             << " | " << u->getCred().getNfc() << " | Zones: " << u->zonesList() << "\n";
    }

public:
    AccessController() {
        zones[0].name = "General Research";
        zones[1].name = "Chemical/Biological Research";
        zones[2].name = "High-Security Area";
        zones[3].name = "Equipment Room";
    }

    void load() {
        logger.load();
        ifstream in("users.txt");
        string line;
        while (getline(in, line)) {
            if (line.empty()) continue;
            stringstream ss(line);
            string id, name, role, nfc, fp, bits;
            getline(ss, id, '|');
            getline(ss, name, '|');
            getline(ss, role, '|');
            getline(ss, nfc, '|');
            getline(ss, fp, '|');
            getline(ss, bits);
            addUser(new User(id, name, role, Credential(nfc, fp), bits));
        }
        if (userCount == 0) {
            addUser(new User("U101", "Rahul", "Researcher", Credential("NFC1025", "FP-RAHUL-01"), "1100"));
            addUser(new User("U102", "Anita", "Student", Credential("NFC1026", "FP-ANITA-02"), "1000"));
            addUser(new User("U103", "Dr. Sharma", "Senior Scientist", Credential("NFC1027", "FP-SHARMA-03"), "1111"));
        }
    }

    void save() {
        ofstream out("users.txt");
        for (int i = 0; i < userCount; i++) {
            User* u = users[i];
            out << u->getId() << '|' << u->getName() << '|' << u->getRole() << '|'
                << u->getCred().getNfc() << '|' << u->getCred().getFingerprint() << '|' << u->bits() << '\n';
        }
    }

    void registerUser() {
        string id = ask("  User ID: ");
        string name = ask("  Name: ");
        string role = ask("  Role: ");
        string nfc = ask("  NFC card ID: ");
        string fp = ask("  Fingerprint ID: ");
        if (id.empty() || name.empty() || nfc.empty() || fp.empty() ||
            !addUser(new User(id, name, role, Credential(nfc, fp)))) {
            cout << "  Registration failed (empty field or duplicate ID/NFC).\n";
            return;
        }
        logger.log(id, "-", "ADMIN", "User registered");
        cout << "  User registered. Grant zone permissions from the menu.\n";
    }

    void showUsers() {
        for (int i = 0; i < userCount; i++) printUser(users[i]);
    }

    void searchUser() {
        User* u = findUser(ask("  User ID: "));
        if (u) printUser(u); else cout << "  User not found.\n";
    }

    void showZones() {
        for (int z = 0; z < ZONES; z++)
            cout << "  Zone " << zoneName(z) << " | " << zones[z].name << " | "
                 << (zones[z].locked ? "LOCKDOWN" : "Open") << "\n";
    }

    void requestAccess() {
        string nfc = ask("  Scan NFC card ID: ");
        string fp = ask("  Fingerprint ID: ");
        int z = zoneNumber(ask("  Zone (A-D): "));
        if (z < 0) { cout << "  Invalid zone.\n"; return; }
        checkAccess(nfc, fp, z);
    }

    void queueRequest() {
        if (queue.full()) { cout << "  Queue is full.\n"; return; }
        Request r;
        r.nfc = ask("  Scan NFC card ID: ");
        r.fingerprint = ask("  Fingerprint ID: ");
        r.zone = zoneNumber(ask("  Zone (A-D): "));
        if (r.zone < 0) { cout << "  Invalid zone.\n"; return; }
        queue.enqueue(r);
        cout << "  Request queued. Waiting: " << queue.size() << "\n";
    }

    void processQueue() {
        if (queue.empty()) { cout << "  Queue is empty.\n"; return; }
        int n = 1;
        while (!queue.empty()) {
            Request r = queue.dequeue();
            cout << "  Request " << n++ << " (" << r.nfc << ", Zone " << zoneName(r.zone) << ")\n  ";
            checkAccess(r.nfc, r.fingerprint, r.zone);
        }
    }

    void changePermission(bool grant) {
        User* u = findUser(ask("  User ID: "));
        int z = zoneNumber(ask("  Zone (A-D): "));
        if (!u || z < 0) { cout << "  User not found or invalid zone.\n"; return; }
        u->setAccess(z, grant);
        string text = string(grant ? "Granted " : "Revoked ") + u->getName() + (grant ? " access to Zone " : " access from Zone ") + zoneName(z);
        actions.push({text, grant ? 'G' : 'R', z, u});
        logger.log(u->getId(), zoneName(z), "ADMIN", grant ? "Permission granted" : "Permission revoked");
        cout << "  " << text << "\n";
    }

    void setLockdown(bool on) {
        int z = zoneNumber(ask("  Zone (A-D): "));
        if (z < 0) { cout << "  Invalid zone.\n"; return; }
        zones[z].locked = on;
        string text = string(on ? "Lockdown activated on Zone " : "Lockdown removed on Zone ") + zoneName(z);
        actions.push({text, on ? 'L' : 'U', z, nullptr});
        logger.log("ADMIN", zoneName(z), "ADMIN", on ? "Lockdown activated" : "Lockdown removed");
        cout << "  " << text << "\n";
    }

    void undoLastAction() {
        Action a;
        if (!actions.pop(a)) { cout << "  Nothing to undo.\n"; return; }
        if (a.type == 'L') zones[a.zone].locked = false;
        else if (a.type == 'U') zones[a.zone].locked = true;
        else if (a.type == 'G') a.user->setAccess(a.zone, false);
        else a.user->setAccess(a.zone, true);
        cout << "  Undone: " << a.text << "\n";
    }

    void showActions() { actions.show(); }
    void showLogs() { logger.showAll(); }
    void searchLogs() { logger.searchUser(ask("  User ID: ")); }
    void showSortedLogs() { logger.showSortedByUser(); }
};

void printMenu() {
    cout << "\n=== LAB ACCESS VERIFICATION ===\n"
            " 1. Register user\n"
            " 2. Request zone access\n"
            " 3. Add request to queue\n"
            " 4. Process queued requests\n"
            " 5. View users\n"
            " 6. Find user by ID\n"
            " 7. View zones\n"
            " 8. Grant permission\n"
            " 9. Revoke permission\n"
            "10. Activate lockdown\n"
            "11. Remove lockdown\n"
            "12. Undo last admin action\n"
            "13. View admin actions\n"
            "14. View access logs\n"
            "15. Search logs by user ID\n"
            "16. View logs sorted by user ID\n"
            " 0. Save and exit\n";
}

int main() {
    AccessController lab;
    lab.load();
    while (true) {
        printMenu();
        int choice = askNumber("Enter choice: ");
        if (cin.eof()) break;
        cout << "\n";
        switch (choice) {
            case 1: lab.registerUser(); break;
            case 2: lab.requestAccess(); break;
            case 3: lab.queueRequest(); break;
            case 4: lab.processQueue(); break;
            case 5: lab.showUsers(); break;
            case 6: lab.searchUser(); break;
            case 7: lab.showZones(); break;
            case 8: lab.changePermission(true); break;
            case 9: lab.changePermission(false); break;
            case 10: lab.setLockdown(true); break;
            case 11: lab.setLockdown(false); break;
            case 12: lab.undoLastAction(); break;
            case 13: lab.showActions(); break;
            case 14: lab.showLogs(); break;
            case 15: lab.searchLogs(); break;
            case 16: lab.showSortedLogs(); break;
            case 0: lab.save(); cout << "  Data saved. Goodbye.\n"; return 0;
            default: cout << "  Invalid choice.\n";
        }
    }
    lab.save();
    return 0;
}