// @I'M AFTAB
// this may contain errors (maybe logical);
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>
#include <fstream>
#include <sstream>

using namespace std;



class UserManager {
private:
    string username;
    string password;
    
public:
    UserManager() {
        loadCredentials();
    }
    
    void loadCredentials() {
        ifstream file("users.txt");
        if (file.is_open()) {
            getline(file, username);
            getline(file, password);
            file.close();
        } else {
            username = "admin";
            password = "password";
        }
    }
    
    void saveCredentials() {
        ofstream file("users.txt");
        if (file.is_open()) {
            file << username << endl;
            file << password << endl;
            file.close();
        }
    }
    
    bool login() {
        string inputUser, inputPass;
        cout << "=== Expense Tracker Login Page ===" << endl;
        cout << "Username: ";
        cin >> inputUser;
        cout << "Password: ";
        cin >> inputPass;
        
        if (inputUser == username && inputPass == password) {
            cout << "Login successful!" << endl;
            return true;
        } else {
            cout << "Invalid credentials!" << endl;
            return false;
        }
    }
    
    void changePassword() {
        string oldPass, newPass;
        cout << "Enter current password: ";
        cin >> oldPass;
        
        if (oldPass == password) {
            cout << "Enter new password: ";
            cin >> newPass;
            password = newPass;
            saveCredentials();
            cout << "Password changed successfully!" << endl;
        } else {
            cout << "Incorrect current password!" << endl;
        }
    }
};

class ExpenseTracker {
private:
    vector<Expense> expenses;
    int nextId;
    const string dataFile = "expenses.txt";
    
    void loadExpenses() {
        ifstream file(dataFile);
        if (file.is_open()) {
            expenses.clear();
            string line;
            while (getline(file, line)) {
                stringstream ss(line);
                Expense exp;
                string amountStr;
                
                getline(ss, line, ',');
                exp.id = stoi(line);
                getline(ss, exp.category, ',');
                getline(ss, exp.description, ',');
                getline(ss, amountStr, ',');
                exp.amount = stod(amountStr);
                getline(ss, exp.date);
                
                expenses.push_back(exp);
            }
            file.close();
            
            if (!expenses.empty()) {
                nextId = expenses.back().id + 1;
            } else {
                nextId = 1;
            }
        }
    }
    
    void saveExpenses() {
        ofstream file(dataFile);
        if (file.is_open()) {
            for (const auto& exp : expenses) {
                file << exp.id << "," << exp.category << "," 
                     << exp.description << "," << exp.amount 
                     << "," << exp.date << endl;
            }
            file.close();
        }
    }
    
public:
    ExpenseTracker() : nextId(1) {
        loadExpenses();
    }
    
    void addExpense() {
        Expense exp;
        exp.id = nextId++;
        
        cout << "Enter category: ";
        cin.ignore();
        getline(cin, exp.category);
        
        cout << "Enter description: ";
        getline(cin, exp.description);
        
        cout << "Enter amount: ";
        cin >> exp.amount;
        
        cout << "Enter date (YYYY-MM-DD) form: ";
        cin >> exp.date;
        
        expenses.push_back(exp);
        saveExpenses();
        cout << "Expense added successfully! ID: " << exp.id << endl;
    }
    
    void deleteExpense() {
        int id;
        cout << "Enter expense ID to delete: ";
        cin >> id;
        
        auto it = find_if(expenses.begin(), expenses.end(), 
                         [id](const Expense& exp) { return exp.id == id; });
        
        if (it != expenses.end()) {
            expenses.erase(it);
            saveExpenses();
            cout << "Expense deleted successfully!" << endl;
        } else {
            cout << "Expense not found!" << endl;
        }
    }
    
    void editExpense() {
        int id;
        cout << "Enter expense ID to edit: ";
        cin >> id;
        
        auto it = find_if(expenses.begin(), expenses.end(), 
                         [id](const Expense& exp) { return exp.id == id; });
        
        if (it != expenses.end()) {
            cout << "Enter new category (current: " << it->category << "): ";
            cin.ignore();
            getline(cin, it->category);
            
            cout << "Enter new description (current: " << it->description << "): ";
            getline(cin, it->description);
            
            cout << "Enter new amount (current: " << it->amount << "): ";
            cin >> it->amount;
            
            cout << "Enter new date (current: " << it->date << "): ";
            cin >> it->date;
            
            saveExpenses();
            cout << "Expense updated successfully!" << endl;
        } else {
            cout << "Expense not found!" << endl;
        }
    }
    
    void replaceExpense() {
        int id;
        cout << "Enter expense ID to replace: ";
        cin >> id;
        
        auto it = find_if(expenses.begin(), expenses.end(), 
                         [id](const Expense& exp) { return exp.id == id; });
        
        if (it != expenses.end()) {
            Expense newExp;
            newExp.id = id;
            
            cout << "Enter new category: ";
            cin.ignore();
            getline(cin, newExp.category);
            
            cout << "Enter new description: ";
            getline(cin, newExp.description);
            
            cout << "Enter new amount: ";
            cin >> newExp.amount;
            
            cout << "Enter new date: ";
            cin >> newExp.date;
            
            *it = newExp;
            saveExpenses();
            cout << "Expense replaced successfully!" << endl;
        } else {
            cout << "Expense not found!" << endl;
        }
    }
    
    void displayAllExpenses() {
        if (expenses.empty()) {
            cout << "No expenses found!" << endl;
            return;
        }
        
        cout << "\n=== All Expenses ===" << endl;
        cout << setw(5) << "ID" << setw(15) << "Category" << setw(20) 
             << "Description" << setw(10) << "Amount" << setw(12) << "Date" << endl;
        cout << string(62, '-') << endl;
        
        double total = 0;
        for (const auto& exp : expenses) {
            cout << setw(5) << exp.id << setw(15) << exp.category 
                 << setw(20) << exp.description << setw(10) << exp.amount 
                 << setw(12) << exp.date << endl;
            total += exp.amount;
        }
        
        cout << string(62, '-') << endl;
        cout << "Total Expenses: $" << total << endl;
    }
    
    void removeAllExpenses() {
        char confirm;
        cout << "Are you sure you want to delete ALL expenses? (y/n): ";
        cin >> confirm;
        
        if (confirm == 'y' || confirm == 'Y') {
            expenses.clear();
            nextId = 1;
            saveExpenses();
            cout << "All expenses removed!" << endl;
        } else {
            cout << "Operation cancelled." << endl;
        }
    }
    
    void searchExpenses() {
        string keyword;
        cout << "Enter search keyword (category/description): ";
        cin.ignore();
        getline(cin, keyword);
        
        vector<Expense> results;
        for (const auto& exp : expenses) {
            if (exp.category.find(keyword) != string::npos || 
                exp.description.find(keyword) != string::npos) {
                results.push_back(exp);
            }
        }
        
        if (results.empty()) {
            cout << "No matching expenses found!" << endl;
            return;
        }
        
        cout << "\n=== Search Results ===" << endl;
        cout << setw(5) << "ID" << setw(15) << "Category" << setw(20) 
             << "Description" << setw(10) << "Amount" << setw(12) << "Date" << endl;
        cout << string(62, '-') << endl;
        
        for (const auto& exp : results) {
            cout << setw(5) << exp.id << setw(15) << exp.category 
                 << setw(20) << exp.description << setw(10) << exp.amount 
                 << setw(12) << exp.date << endl;
        }
    }
};

int main() {
    UserManager userManager;
    
    if (!userManager.login()) {
        return 0;
    }
    
    ExpenseTracker tracker;
    int choice;
    
    do {
        cout << "\n=== Expense Tracker Menu ===" << endl;
        cout << "1. Add Expense" << endl;
        cout << "2. Delete Expense" << endl;
        cout << "3. Edit Expense" << endl;
        cout << "4. Replace Expense" << endl;
        cout << "5. View All Expenses" << endl;
        cout << "6. Remove All Expenses" << endl;
        cout << "7. Search Expenses" << endl;
        cout << "8. Change Password" << endl;
        cout << "9. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;
        
        switch (choice) {
            case 1:
                tracker.addExpense();
                break;
            case 2:
                tracker.deleteExpense();
                break;
            case 3:
                tracker.editExpense();
                break;
            case 4:
                tracker.replaceExpense();
                break;
            case 5:
                tracker.displayAllExpenses();
                break;
            case 6:
                tracker.removeAllExpenses();
                break;
            case 7:
                tracker.searchExpenses();
                break;
            case 8:
                userManager.changePassword();
                break;
            case 9:
                cout << "Goodbye!" << endl;
                break;
            default:
                cout << "Invalid choice!" << endl;
        }
    } while (choice != 9);
    
    return 0;

}
