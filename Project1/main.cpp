#include <iostream>
#include <vector>

using namespace std;


int func() {
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
        {1, 1, 0, 1, 0},
        {0, 0, 0, 1, 0},
        {0, 0, 0, 0, 0} };
    for (int q = 0; q < nodes.size(); q++) {
        vector<vector<int>> pos = nodes[q];
        cout << "pos: ";
        for (auto p : pos) {
            cout << "[" << p[0] << "," << p[1] << "] ";
        }
        cout << endl;
        for (auto& combo : combos) {
            int agent = combo[0];
            for (auto& step : steps) {
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
                if (combo.size() > 1) {
                    int agent2 = combo[1];
                    for (auto& step2 : steps) {
                        vector<vector<int>> node2;
                        for (int j = 0; j < node.size(); j++) {
                            if (j == agent2) {
                                if (-1 < (node[j][0] + step2[0]) && (node[j][0] + step2[0]) < 5 && -1 < (node[j][1] + step2[1]) && (node[j][1] + step2[1]) < 5 &&
                                    map[node[j][0] + step2[0]][node[j][1] + step2[1]] == 0 &&
                                    find(node.begin(), node.end(), vector<int>{node[j][0] + step2[0], node[j][1] + step2[1]}) == node.end()) {
                                    node2.push_back({ node[j][0] + step2[0], node[j][1] + step2[1] });
                                }
                                else {
                                    break;
                                }
                            }
                            else {
                                node2.push_back(node[j]);
                            }
                        }
                        if (combo.size() == 3) {
                            int agent3 = combo[2];
                            for (auto& step3 : steps) {
                                vector<vector<int>> node3;
                                for (int k = 0; k < node2.size(); k++) {
                                    if (k == agent3) {
                                        if (-1 < (node2[k][0] + step3[0]) && (node2[k][0] + step3[0]) < 5 && -1 < (node2[k][1] + step3[1]) && (node2[k][1] + step3[1]) < 5 &&
                                            map[node2[k][0] + step3[0]][node2[k][1] + step3[1]] == 0 &&
                                            find(node2.begin(), node2.end(), vector<int>{node2[k][0] + step3[0], node2[k][1] + step3[1]}) == node2.end()) {
                                            node3.push_back({ node2[k][0] + step3[0], node2[k][1] + step3[1] });
                                        }
                                        else {
                                            break;
                                        }
                                    }
                                    else {
                                        node3.push_back(node2[k]);
                                    }
                                }
                                if (node3.size() == 3 && find(nodes.begin(), nodes.end(), node3) == nodes.end()) {
                                    nodes.push_back(node3);
                                    if (node3 == finish) {
                                        return 0;
                                    }
                                }
                            }
                        }
                        else {
                            if (node2.size() == 3 && find(nodes.begin(), nodes.end(), node2) == nodes.end()) {
                                nodes.push_back(node2);
                                if (node2 == finish) {
                                    return 0;
                                }
                            }
                        }
                    }
                }
                else {
                    if (node.size() == 3 && find(nodes.begin(), nodes.end(), node) == nodes.end()) {
                        nodes.push_back(node);
                        if (node == finish) {
                            return 0;
                        }
                    }
                }
            }
        }
    }
    cout << "net puti blyat" << endl;
    return 1;
}

void main() {
    if (func() == 0) {
        cout << "finish" << endl;
    }
}