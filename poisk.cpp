#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <unordered_set>

using namespace std;

class Graph {
    vector<vector<string>> adj= 
        {   {"Oleg"},
            {"Nikita"},
            {"Nastya"},
            {"Vanya"},
            {"Zhenya"}
        };	
	
public:
	Graph() {}
	
    string name(int i) {return adj[i][0];}

    void addEdge(int u, int v) {
        adj[u].push_back(adj[v][0]);
        adj[v].push_back(adj[u][0]);
    }

    vector<pair<string, string>> findAcquaintances(string src, int i) {
        vector<pair<string, string>> acquaintances;

        // обход в ширину с максимальной глубиной 3
        queue<string> q;
        q.push(src);
        unordered_set<string> visited;
        visited.insert(src);

        while (!q.empty()) {
            string current = q.front();
            q.pop();

            // Просмотр текущих соседей узла
            for (string neighbor : adj[i]) {
                // Если сосед ранее не просматривался
                if (visited.find(neighbor) == visited.end()) {
                    // Добавим его в знакомых
                    acquaintances.push_back(make_pair(src, neighbor));

                    // Пометим соседа как посещенного
                    visited.insert(neighbor);

                    // Запушим соседа в очередь для следующего раза
                    q.push(neighbor);
                }
            }
        }

        return acquaintances;
    }
};

int main() {

    Graph graph;

    graph.addEdge(0, 1);
    graph.addEdge(0, 2);
    graph.addEdge(1, 3);
    graph.addEdge(2, 4);
    graph.addEdge(3, 4);

    // Поиск знакомых
    for (int i = 0; i < 5; i++) {
        cout << "Знакомство для " << graph.name(i) << ":\n";
        vector<pair<string, string>> acquaintances = graph.findAcquaintances(graph.name(i), i);
        for (const auto& acquaintance : acquaintances) {
            cout << acquaintance.first << " - " << acquaintance.second << "\n";
        }
        cout << "\n";
    }

    return 0;
}
