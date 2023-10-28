#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <Windows.h>

using namespace std;

ofstream file;
vector<vector<int>> steps = { {-1, 0}, {1, 0}, {0, -1}, {0, 1} };
vector<vector<int>> combos = {
    {0}, {1}, {2},
    {0, 1}, {0, 2}, {1, 0}, {1, 2}, {2, 0}, {2, 1},
    {0, 1, 2}, {0, 2, 1}, {1, 0, 2}, {1, 2, 0}, {2, 0, 1}, {2, 1, 0} };
vector<vector<vector<int>>> nodes = { {{0, 0}, {0, 1}, {0, 2}} };
vector<vector<int>> finish = { {3, 0}, {0, 4}, {4, 2} };
vector<vector<int>> map = {
    {0, 0, 0, 1, 0},
    {0, 0, 0, 1, 0},
    {1, 1, 1, 1, 0},
    {0, 0, 0, 1, 0},
    {0, 0, 0, 0, 0} };

string node2string(vector<vector<int>> node) {
    string node_s;
    for (auto& a : node) {
        node_s += "[" + to_string(a[0]) + "," + to_string(a[1]) + "]";
    }
    node_s = '"' + node_s + '"';
    return node_s;
}

vector<vector<int>> move_1_agent(int agent, vector<int> step ,vector<vector<int>> pos) {
    vector<vector<int>> node;
    for (int i = 0; i < pos.size(); i++) {
        if (i == agent) {
            if (-1 < (pos[i][0] + step[0]) && (pos[i][0] + step[0]) < 5 && -1 < (pos[i][1] + step[1]) && (pos[i][1] + step[1]) < 5 &&
                map[pos[i][0] + step[0]][pos[i][1] + step[1]] == 0 &&
                find(pos.begin(), pos.end(), vector<int>{pos[i][0] + step[0], pos[i][1] + step[1]}) == pos.end()) {
                node.push_back({ pos[i][0] + step[0], pos[i][1] + step[1] });
            }
            else {
                break;
            }
        }
        else {
            node.push_back(pos[i]);
        }
    }
    return node;
}

int func() {
    for (int q = 0; q < nodes.size(); q++) {
        vector<vector<int>> pos = nodes[q];
        cout << "pos: ";
        for (auto& p : pos) {
            cout << "[" << p[0] << "," << p[1] << "] ";
        }
        cout << endl;
        string mother = node2string(pos);
        for (auto& combo : combos) {
            int agent = combo[0];
            for (auto& step : steps) {
                vector<vector<int>> node = move_1_agent(agent, step, pos);
                if (combo.size() > 1) {
                    int agent2 = combo[1];
                    for (auto& step2 : steps) {
                        vector<vector<int>> node2 = move_1_agent(agent2, step2, node);
                        if (combo.size() == 3) {
                            int agent3 = combo[2];
                            for (auto& step3 : steps) {
                                vector<vector<int>> node3 = move_1_agent(agent3, step3, node2);
                                if (node3.size() == 3 && find(nodes.begin(), nodes.end(), node3) == nodes.end()) {
                                    nodes.push_back(node3);
                                    string branch = mother + " -> " + node2string(node3);
                                    file << branch << endl;
                                    if (node3 == finish) {return 0;}
                                }
                            }
                        }
                        else {
                            if (node2.size() == 3 && find(nodes.begin(), nodes.end(), node2) == nodes.end()) {
                                nodes.push_back(node2);
                                string branch = mother + " -> " + node2string(node2);
                                file << branch << endl;
                                if (node2 == finish) {return 0;}
                            }}
                    }
                }
                else {
                    if (node.size() == 3 && find(nodes.begin(), nodes.end(), node) == nodes.end()) {
                        nodes.push_back(node);
                        string branch = mother + " -> " + node2string(node);
                        file << branch << endl;
                        if (node == finish) {return 0;}
                    }}
            }
        }
    }
    return 1;
}

void main() {
    file.open("forgraph.dot");
    file << "digraph tree{" << endl;
    if (func() == 0) {
        cout << "finish" << endl;
    }
    else {
        cout << "path does not exist" << endl;
    }
    file << "}" << endl;
    file.close();
    system("dot -Tpdf -O forgraph.dot");
}