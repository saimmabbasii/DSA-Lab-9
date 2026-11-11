/*
================================================================================
Name: Muhammad Saim Abbasi
CMS: 544102
Section: BSAI-2A
Open Ended Lab - Student Help Desk Application
================================================================================

DESIGN EXPLANATION and SUITABILITY:
1. Primary Storage (Student Records): Binary Search Tree (BST)
   - Why chosen: A help desk performs frequent lookups by roll number. A BST offers
     an efficient average search time of O(log n) and average insertion of O(log n).
     Duplicate detection is seamlessly integrated into insertion during tree traversal.
     Furthermore, an in-order traversal (Left -> Root -> Right) retrieves all records
     in ascending order of roll numbers in O(n) without requiring an auxiliary sort.
   - Why not an array: An unsorted array requires O(n) search and O(n) duplicate check.
     A sorted array allows O(log n) binary search but suffers from costly O(n) element 
     shifting during insertion and has a fixed or expensive reallocation capacity.
   - Why not a linked list: Lacks random access; searching and duplicate validation 
     always take O(n) linear scans.

2. Additional Feature: Help Requests Queue (Singly Linked List)
   - Why chosen: Student service requests strictly follow a First-In, First-Out (FIFO)
     policy to ensure fairness. By maintaining explicit 'front' and 'rear' pointers, 
     both enqueue (adding a request) and dequeue (serving the earliest request) 
     operate in strict O(1) time complexity.
   - Boundary checks implemented: Rejection of unregistered student roll numbers, 
     empty queue handling, duplicate registration prevention in BST, and full dynamic 
     memory deallocation upon program termination.

WORST-CASE TIME COMPLEXITIES:
- Student Record Insertion: O(n) [Occurs when the BST degenerates into a skewed tree/linked list]
- Student Record Search:    O(n) [Occurs when the BST degenerates into a skewed tree/linked list]
- Display All Records:      O(n) [Visits every node exactly once]
- Queue Enqueue:            O(1) [Pointer insertion at rear]
- Queue Dequeue:            O(1) [Pointer removal from front]
================================================================================
*/

#include <iostream>
#include <string>

// Node representing a student record in the Binary Search Tree
struct StudentNode {
    int rollNo;
    std::string name;
    StudentNode* left;
    StudentNode* right;

    StudentNode(int r, const std::string& n) 
        : rollNo(r), name(n), left(nullptr), right(nullptr) {}
};

// Node representing an entry in the FIFO Help Request Queue
struct QueueNode {
    int rollNo;
    QueueNode* next;

    QueueNode(int r) : rollNo(r), next(nullptr) {}
};

// Binary Search Tree class to manage student records
class StudentBST {
private:
    StudentNode* root;

    // Helper for inserting a node; handles duplicate rejection
    StudentNode* insertHelper(StudentNode* node, int rollNo, const std::string& name, bool& isInserted) {
        if (node == nullptr) {
            isInserted = true;
            return new StudentNode(rollNo, name);
        }

        if (rollNo < node->rollNo) {
            node->left = insertHelper(node->left, rollNo, name, isInserted);
        } else if (rollNo > node->rollNo) {
            node->right = insertHelper(node->right, rollNo, name, isInserted);
        } else {
            // Duplicate roll number detected
            isInserted = false;
        }
        return node;
    }

    // Helper for searching a student record
    StudentNode* searchHelper(StudentNode* node, int rollNo) const {
        if (node == nullptr || node->rollNo == rollNo) {
            return node;
        }
        if (rollNo < node->rollNo) {
            return searchHelper(node->left, rollNo);
        }
        return searchHelper(node->right, rollNo);
    }

    // In-order traversal helper to display records sorted by roll number
    void inOrderHelper(StudentNode* node) const {
        if (node != nullptr) {
            inOrderHelper(node->left);
            std::cout << "  Roll No: " << node->rollNo << " | Name: " << node->name << "\n";
            inOrderHelper(node->right);
        }
    }

    // Post-order traversal helper for clean dynamic memory deallocation
    void destroyTree(StudentNode* node) {
        if (node != nullptr) {
            destroyTree(node->left);
            destroyTree(node->right);
            delete node;
        }
    }

public:
    StudentBST() : root(nullptr) {}

    ~StudentBST() {
        destroyTree(root);
        root = nullptr;
    }

    // Worst-Case Time Complexity: O(n) (Skewed tree), Average-Case: O(log n)
    bool addStudent(int rollNo, const std::string& name) {
        bool isInserted = false;
        root = insertHelper(root, rollNo, name, isInserted);
        return isInserted;
    }

    // Worst-Case Time Complexity: O(n) (Skewed tree), Average-Case: O(log n)
    StudentNode* searchStudent(int rollNo) const {
        return searchHelper(root, rollNo);
    }

    // Worst-Case Time Complexity: O(n)
    void displayAll() const {
        if (root == nullptr) {
            std::cout << "[Info] No student records found. The database is empty.\n";
            return;
        }
        std::cout << "--- Registered Student Records (Sorted by Roll Number) ---\n";
        inOrderHelper(root);
        std::cout << "---------------------------------------------------------\n";
    }

    bool isEmpty() const {
        return root == nullptr;
    }
};

// Queue class to manage FIFO Help Requests
class RequestQueue {
private:
    QueueNode* front;
    QueueNode* rear;

public:
    RequestQueue() : front(nullptr), rear(nullptr) {}

    ~RequestQueue() {
        clearQueue();
    }

    // Worst-Case Time Complexity: O(1)
    void enqueue(int rollNo) {
        QueueNode* newNode = new QueueNode(rollNo);
        if (rear == nullptr) {
            front = rear = newNode;
        } else {
            rear->next = newNode;
            rear = newNode;
        }
    }

    // Worst-Case Time Complexity: O(1)
    bool dequeue(int& servedRollNo) {
        if (isEmpty()) {
            return false;
        }
        QueueNode* temp = front;
        servedRollNo = temp->rollNo;
        front = front->next;

        if (front == nullptr) {
            rear = nullptr;
        }
        delete temp;
        return true;
    }

    bool isEmpty() const {
        return front == nullptr;
    }

    void displayQueue() const {
        if (isEmpty()) {
            std::cout << "[Info] The help request queue is currently empty.\n";
            return;
        }
        std::cout << "Current Queue (Front -> Rear): ";
        QueueNode* curr = front;
        while (curr != nullptr) {
            std::cout << "[" << curr->rollNo << "]";
            if (curr->next != nullptr) std::cout << " -> ";
            curr = curr->next;
        }
        std::cout << "\n";
    }

    void clearQueue() {
        while (front != nullptr) {
            QueueNode* temp = front;
            front = front->next;
            delete temp;
        }
        rear = nullptr;
    }
};

int main() {
    StudentBST bst;
    RequestQueue queue;

    int choice = 0;

    std::cout << "=====================================================\n";
    std::cout << "        STUDENT HELP DESK MANAGEMENT SYSTEM         \n";
    std::cout << "=====================================================\n";

    while (true) {
        std::cout << "\n--- Main Menu ---\n";
        std::cout << "1. Add Student Record\n";
        std::cout << "2. Search Student by Roll Number\n";
        std::cout << "3. Display All Students\n";
        std::cout << "4. Submit Help Request (Enqueue)\n";
        std::cout << "5. Serve Earliest Help Request (Dequeue)\n";
        std::cout << "6. View Help Request Queue\n";
        std::cout << "7. Exit Application\n";
        std::cout << "Enter your choice (1-7): ";

        if (!(std::cin >> choice)) {
            std::cout << "[Error] Invalid input. Please enter an integer.\n";
            std::cin.clear();
            std::string discard;
            std::getline(std::cin, discard);
            continue;
        }

        switch (choice) {
            case 1: {
                int roll;
                std::string name;
                std::cout << "Enter Roll Number: ";
                std::cin >> roll;
                std::cout << "Enter Student Name: ";
                std::cin.ignore();
                std::getline(std::cin, name);

                if (bst.addStudent(roll, name)) {
                    std::cout << "[Success] Student record added successfully.\n";
                } else {
                    std::cout << "[Error] Duplicate roll number! Student with Roll No " 
                              << roll << " is already registered.\n";
                }
                break;
            }

            case 2: {
                if (bst.isEmpty()) {
                    std::cout << "[Error] Search failed. No records exist in the database.\n";
                    break;
                }
                int roll;
                std::cout << "Enter Roll Number to search: ";
                std::cin >> roll;

                StudentNode* res = bst.searchStudent(roll);
                if (res != nullptr) {
                    std::cout << "[Found] Roll No: " << res->rollNo << " | Name: " << res->name << "\n";
                } else {
                    std::cout << "[Search Failed] No student found with Roll Number " << roll << ".\n";
                }
                break;
            }

            case 3: {
                bst.displayAll();
                break;
            }

            case 4: {
                int roll;
                std::cout << "Enter Roll Number for Help Request: ";
                std::cin >> roll;

                // Validate that student is registered first
                StudentNode* student = bst.searchStudent(roll);
                if (student == nullptr) {
                    std::cout << "[Rejected] Student with Roll No " << roll 
                              << " is not registered. Request denied.\n";
                } else {
                    queue.enqueue(roll);
                    std::cout << "[Success] Help request queued for " << student->name 
                              << " (Roll No: " << roll << ").\n";
                }
                break;
            }

            case 5: {
                int servedRoll;
                if (queue.dequeue(servedRoll)) {
                    StudentNode* s = bst.searchStudent(servedRoll);
                    std::cout << "[Served] Now assisting: " 
                              << (s ? s->name : "Unknown") 
                              << " (Roll No: " << servedRoll << ")\n";
                } else {
                    std::cout << "[Notice] Cannot serve request: Queue is empty.\n";
                }
                break;
            }

            case 6: {
                queue.displayQueue();
                break;
            }

            case 7: {
                std::cout << "[Exiting] Freeing allocated memory and closing system...\n";
                return 0;
            }

            default:
                std::cout << "[Error] Invalid option selected. Please choose between 1 and 7.\n";
                break;
        }
    }

    return 0;
}