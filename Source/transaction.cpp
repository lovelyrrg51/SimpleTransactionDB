#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

struct account_balance {
    int account_id; // the ID of account
    int balance;    // the Balance of account
};

struct transfer {
    int from;       // the account to transfer from
    int to;         // the account to transfer to
    int amount;     // the amount to transfer
};

using transaction = vector<transfer>;

struct account_transaction_amount {
    int transaction_id; // the id of transaction
    int amount;         // the total amount of transaction
};

// Compare two amount for sort
bool compareAmount(account_transaction_amount v1, account_transaction_amount v2) {
    return (v1.amount < v2.amount);
}

using account_transaction = vector<account_transaction_amount>;

class TransactionDB {
    // Current Transaction ID
    int transactionID;
    // Total Users
    int totalUsers;
    // User Balance List
    vector<account_balance> balanceList;
    // User Transaction List
    vector<transaction> transactionList;
    // Applied Transaction List
    vector<size_t> appliedTransactionList;
    // User Transaction Amount List
    vector<account_transaction> accountTransactionList;
public:
    TransactionDB() {
        transactionID = 0;
        totalUsers = 0;
        balanceList = {};
        transactionList = {};
        appliedTransactionList = {};
    }
    TransactionDB(const vector<account_balance> initial_balances) {
        transactionID = 0;
        // Update User Balance List
        balanceList = initial_balances;
        // Update Total Users
        totalUsers = balanceList.size();
        // Resize Account Transaction List
        accountTransactionList.resize(totalUsers);
    }
    // Get Current Transaction ID
    int get_transaction_id() const {
        return transactionID;
    }
    // Get User Balance List
    vector<account_balance> get_balances() const {
        return balanceList;
    }
    // Get Applied Transaction List
    vector<size_t> get_applied_transactions() const {
        return appliedTransactionList;
    }
    // Push Transaction
    void push_transaction(const transaction &t) {
        vector<int> amountList(t.size() + 1, 0);
        
        // Update Transaction ID
        transactionID += 1;
        
        // Per Transfer with each Transaction
        for (auto trans : t) {
            // Update From Account Amount
            amountList[trans.from] -= trans.amount;
            // Update To Account Amount
            amountList[trans.to] += trans.amount;
        }
        
        // Update User Amount with each Transaction
        for (int i = 1; i <= totalUsers; i += 1) {
            // Only Send/Receive Users
            if (amountList[i] != 0) {
                accountTransactionList[i - 1].push_back({transactionID, amountList[i]});
            }
        }
        
        // Update Transaction List
        transactionList.push_back(t);
    }
    // Sort Amount List
    void sort_amount_list() {
        for (int i = 0; i < accountTransactionList.size(); i += 1) {
            std::sort(accountTransactionList[i].begin(), accountTransactionList[i].end(), compareAmount);
            
            for (int j = 0; j < accountTransactionList[i].size(); j += 1) {
                cout << i + 1 << " " << accountTransactionList[i][j].transaction_id << " " << accountTransactionList[i][j].amount << endl;
            }
            cout << endl;
        }
    }
    // Get Dropped Transaction List
    vector<int> get_dropped_transactionList(account_transaction accountTransaction, int *sumAmount) {
        vector<int> droppedTransactionList;
        
        for (auto accountT : accountTransaction) {
            *sumAmount -= accountT.amount;
            droppedTransactionList.push_back(accountT.transaction_id);
            
            if (*sumAmount >= 0) {
                break;
            }
        }
        
        return droppedTransactionList;
    }
    // Settle Transactions
    void settle() {
        vector<bool> droppedTransactionIDList(transactionID, false);
        
        // Sort Account Transaction Amount List
        sort_amount_list();
        
        // Per Users
        for (int i = 0; i < totalUsers; i += 1) {
            int sumAmount = 0;
            for (auto accTAmount : accountTransactionList[i]) {
                sumAmount += accTAmount.amount;
            }
            
            // Check Final Account Balance
            if (balanceList[i].balance + sumAmount < 0) {
                int finalBalance = balanceList[i].balance + sumAmount;
                
                // Get Dropped Transaction List
                vector<int> droppedTransactionList = get_dropped_transactionList(accountTransactionList[i], &finalBalance);
                
                // Update Dropped Transaction ID List
                for (auto dropId: droppedTransactionList) {
                    droppedTransactionIDList[dropId] = true;
                }
            }
        }
        
        // Update Account Balance
        for (int i = 0; i < totalUsers; i += 1) {
            int sumAmount = 0;
            for (auto accTAmount : accountTransactionList[i]) {
                if (!droppedTransactionIDList[accTAmount.transaction_id]) {
                    sumAmount += accTAmount.amount;
                }
            }
            
            balanceList[i].balance += sumAmount;
        }
        
        // Update Applied Transaction List
        for (int i = 1; i <= transactionID; i += 1) {
            if (!droppedTransactionIDList[i]) {
                appliedTransactionList.push_back(i);
            }
        }
    }
};

auto create_database(const vector<account_balance>& initial_balances) {
    TransactionDB transaction_db(initial_balances);
    return transaction_db;
}

int main()
{
    int input[100] = {3, 1, 5, 2, 10, 3, 15, 2, 1, 2, 1, 11, 2, 1, 2, 3, 3, 1, 2};
    // int input[100] = {3, 1, 5, 2, 10, 3, 15, 1, 1, 2, 1, 11};
    // int input[100] = {2, 1, 10, 2, 5, 1, 1, 1, 2, 5};
    size_t p = 0;
    
    std::cout << input[0] << std::endl;
    
    vector<account_balance> test_balances;
    for (int i = 0; i < input[0]; i += 1) {
        int account = input[p += 1];
        int balance = input[p += 1];
        
        test_balances.push_back({account, balance});
    }
    
    auto db = create_database(test_balances);
    
    int transactionCount = input[p += 1];
    vector<transaction> transactions(transactionCount);
    
    cout << "Transaction Count: " << transactionCount << endl;
    
    for (int i = 0; i < transactionCount; i += 1) {
        transaction tx;
        int transferCount = input[p += 1];

        for (int j = 0; j < transferCount; j += 1) {
            int from = input[p += 1];
            int to = input[p += 1];
            int amount = input[p += 1];

            // cout << from << " " << to << " " << amount << endl;

            tx.push_back({from, to, amount});
        }
        
        transactions.push_back(tx);
        db.push_transaction(tx);
        
        cout << "Transaction ID: " << db.get_transaction_id() << endl;
    }

    std::cout << std::endl;

    db.settle();
    
    vector<account_balance> updatedBalance = db.get_balances();
    
    for (int i = 0; i < input[0]; i += 1) {
        std::cout << updatedBalance[i].account_id << " " << updatedBalance[i].balance << std::endl;
    }

    vector<size_t> applied_transaction = db.get_applied_transactions();
    
    cout << "applied transaction: " << applied_transaction.size() << endl;
    
    for (int i = 0; i < applied_transaction.size(); i += 1) {
        std::cout << applied_transaction[i] << " ";
    }
    std::cout << std::endl;

    return 0;
}
