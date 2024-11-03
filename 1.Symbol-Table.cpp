#include <iostream>
#include <string>
#include <list>
#include <vector>

using namespace std;

struct SymbolTableEntry {
    string name;
    string type;
    string size;
    string dimension;
    string line_of_code;
    string address;
};

class SymbolTable {
    int table_size;
    vector<list<SymbolTableEntry>> table; 

public:
    SymbolTable(int size) {
        table_size = size;
        table.resize(size);
    }
    
    int getHashKey(string name) {
        int hash_value = 0;
        for (char ch : name) {
            hash_value += ch;
        }
        return hash_value % table_size;
    }
    
    void insert(string name, string type, string size, string dimension, string line_of_code, string address) {
        int hash_key = getHashKey(name);

        for (auto &entry : table[hash_key]) {
            if (entry.name == name) {
                cout << "Symbol '" << name << "' already exists. Use update to modify it.\n";
                return;
            }
        }
        SymbolTableEntry new_entry = { name, type, size, dimension, line_of_code, address };

        table[hash_key].push_back(new_entry);
        cout << "Symbol '" << name << "' inserted.\n";
    }

    SymbolTableEntry* search(string name) {
        int hash_key = getHashKey(name);

        for (auto &entry : table[hash_key]) {
            if (entry.name == name) {
                return &entry;
            }
        }
        cout << "Symbol '" << name << "' not found.\n";
        return nullptr;
    }

    void deleteSymbol(string name) {
        int hash_key = getHashKey(name);

        for (auto it = table[hash_key].begin(); it != table[hash_key].end(); ++it) {
            if (it->name == name) {
                table[hash_key].erase(it);
                cout << "Symbol '" << name << "' deleted.\n";
                return;
            }
        }
        cout << "Symbol '" << name << "' not found.\n";
    }

    void update(string name, string type = "", string size = "", string dimension = "", string line_of_code = "", string address = "") {
        int hash_key = getHashKey(name);
        bool symbolFound = false;  

        for (auto &entry : table[hash_key]) {
            if (entry.name == name) {
                symbolFound = true;  
                if (!type.empty()) entry.type = type;
                if (!size.empty()) entry.size = size;
                if (!dimension.empty()) entry.dimension = dimension;
                if (!line_of_code.empty()) entry.line_of_code = line_of_code;
                if (!address.empty()) entry.address = address;

                cout << "Symbol '" << name << "' updated.\n";
                return;  
            }
        }

        if (!symbolFound) {
            cout << "Symbol '" << name << "' does not exist. Please check the name.\n";
        }
    }

    void show() {
        cout << "Symbol Table Contents:\n";
        for (int i = 0; i < table_size; i++) {
            if (!table[i].empty()) {
                cout << "Index " << i << ":\n";
                for (auto &entry : table[i]) {
                    cout << "Name: " << entry.name << ", Type: " << entry.type << ", Size: " << entry.size
                         << ", Dimension: " << entry.dimension << ", Line of Code: " << entry.line_of_code
                         << ", Address: " << entry.address << "\n";
                }
            }
        }
    }

   
    void displayHashKey(string name) {
        int hash_key = getHashKey(name);
        cout << "Hash key for symbol '" << name << "' is: " << hash_key << "\n";
    }
};

int main() {
    int table_size;
    cout << "Enter symbol table size: ";
    cin >> table_size;

    SymbolTable symbolTable(table_size);
    int choice;
    string name, type, size, dimension, line_of_code, address;

    do {
        cout << "\nOperations:\n";
        cout << "1. Insert\n";
        cout << "2. Search\n";
        cout << "3. Update\n";
        cout << "4. Delete\n";
        cout << "5. Show Symbol Table\n";
        cout << "6. Get Hash Key\n";  
        cout << "7. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            cout << "Enter symbol name: ";
            cin >> name;
            cout << "Enter symbol type: ";
            cin >> type;
            cout << "Enter symbol size: ";
            cin >> size;
            cout << "Enter symbol dimension: ";
            cin >> dimension;
            cout << "Enter symbol line of code: ";
            cin >> line_of_code;
            cout << "Enter symbol address: ";
            cin >> address;
            symbolTable.insert(name, type, size, dimension, line_of_code, address);
            break;
        case 2:
            cout << "Enter symbol name to search: ";
            cin >> name;
            if (SymbolTableEntry* symbol = symbolTable.search(name)) {
                cout << "Symbol found: Name: " << symbol->name << ", Type: " << symbol->type << "\n";
            }
            break;
        case 3: {
            cout << "Enter symbol name to update: ";
            cin >> name;

            cout << "Enter new type (or press 0 to skip): ";
            cin >> type;
            if (type == "0") type = "";  

            cout << "Enter new size (or press 0 to skip): ";
            cin >> size;
            if (size == "0") size = ""; 
            cout << "Enter new dimension (or press 0 to skip): ";
            cin >> dimension;
            if (dimension == "0") dimension = "";  
            cout << "Enter new line of code (or press 0 to skip): ";
            cin >> line_of_code;
            if (line_of_code == "0") line_of_code = "";  
            cout << "Enter new address (or press 0 to skip): ";
            cin >> address;
            if (address == "0") address = "";  

            symbolTable.update(name, type, size, dimension, line_of_code, address);
            break;
        }
        case 4:
            cout << "Enter symbol name to delete: ";
            cin >> name;
            symbolTable.deleteSymbol(name);
            break;
        case 5:
            symbolTable.show();
            break;
        case 6:
            cout << "Enter symbol name to get hash key: ";  
            cin >> name;
            symbolTable.displayHashKey(name);
            break;
        case 7:
            cout << "Exiting...\n";
            break;
        default:
            cout << "Invalid choice! Please try again.\n";
        }
    } while (choice != 7);

    return 0;
}
