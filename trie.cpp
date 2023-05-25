#include <iostream>
#include <unordered_map>
using namespace std;

// Префиксный узел
struct TrieNode {
    unordered_map<char, TrieNode*> children;
    bool isEndOfWord;
};

// Префиксный класс
class Trie {
private:
    TrieNode* root;

public:
    Trie() {
        root = new TrieNode();
    }

    // Вставка
    void insert(const string& word) {
        TrieNode* current = root;
        for (char c : word) {
            if (current->children.find(c) == current->children.end()) {
                current->children[c] = new TrieNode();
            }
            current = current->children[c];
        }
        current->isEndOfWord = true;
    }

    // Автодополнение префикса
    void autocomplete(const string& prefix) {
        TrieNode* current = root;
        for (char c : prefix) {
            if (current->children.find(c) == current->children.end()) {
                cout << "No matching words found." << endl;
                return;
            }
            current = current->children[c];
        }
        autocompleteUtil(current, prefix);
    }

private:
    // рекурсия для автозаполнения
    void autocompleteUtil(TrieNode* node, const string& prefix) {
        if (node->isEndOfWord) {
            cout << prefix << endl;
        }
        for (const auto& kvp : node->children) {
            autocompleteUtil(kvp.second, prefix + kvp.first);
        }
    }
};

int main() {
    
    Trie trie;

    // Вставим слова в трие
    trie.insert("apple");
    trie.insert("application");
    trie.insert("banana");
    trie.insert("bat");
    trie.insert("cat");
    trie.insert("car");

    // Цикл автозаполнения
    while (true) {
        string prefix;
        cout << "Enter a prefix: ";
        cin >> prefix;
        cout << "Autocomplete results:" << endl;
        trie.autocomplete(prefix);
    }

    return 0;
}
