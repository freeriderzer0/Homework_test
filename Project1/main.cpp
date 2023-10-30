#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <Windows.h>
#include <iomanip>
#include <stdlib.h>
#include <ctime>
#include <algorithm>
#include <utility>

using namespace std;

ofstream file;
vector<vector<int>> steps = { {-1, 0}, {1, 0}, {0, -1}, {0, 1} };
vector<vector<int>> steps_pf = { {-1, 0}, {-1, 1}, {0, 1}, {1, 1}, {1, 0}, {1, -1}, {0, -1}, {-1, -1} };
vector<vector<int>> combos = {
    {0}, {1}, {2},
    {0, 1}, {0, 2}, {1, 0}, {1, 2}, {2, 0}, {2, 1},
    {0, 1, 2}, {0, 2, 1}, {1, 0, 2}, {1, 2, 0}, {2, 0, 1}, {2, 1, 0} };

vector<vector<vector<int>>> nodes;
vector<vector<vector<int>>> nodes_pf;
vector<vector<int>> finish;

vector<vector<int>> map = {
    {0, 0, 0, 1, 1, 0, 0, 0, 0, 0},
    {0, 0, 0, 1, 1, 0, 0, 1, 1, 0},
    {0, 0, 0, 1, 1, 0, 0, 1, 1, 0},
    {0, 0, 0, 1, 1, 0, 0, 1, 1, 0},
    {1, 1, 0, 1, 1, 0, 0, 1, 1, 0},
    {1, 1, 0, 1, 1, 0, 0, 1, 1, 0},
    {1, 1, 0, 1, 1, 0, 0, 1, 1, 0},
    {1, 1, 0, 1, 1, 0, 0, 1, 1, 0},
    {0, 0, 0, 1, 1, 0, 0, 1, 1, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0} };
//vector<vector<int>> map = {
//    {0, 0, 0, 0, 0} ,
//    {0, 0, 0, 0, 0},
//    {1, 1, 0, 1, 1},
//    {0, 0, 0, 0, 0},
//    {0, 0, 0, 0, 0} };

vector<string> mothers;
vector<string> daughters;

template<int txt = 7, int bg = 0>
ostream& color(ostream& text) {
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hStdOut, (WORD)((bg << 4) | txt));
    return text;
}

void printArray(vector<vector<int>> map) {
    for (int i = 0; i < map.size(); i++)
    {
        for (int j = 0; j < map[0].size(); j++) {
            if (map[i][j] == 1) { cout << setw(2) << color<4, 0> << map[i][j] << color<7, 0>; }
            else { cout << setw(2) << color<7, 0> << map[i][j] << color<7, 0>; }
        }
        cout << endl;
    }
}

string node2string(vector<vector<int>> node) {
    string node_s;
    for (auto& a : node) {
        node_s += "[" + to_string(a[0]) + "," + to_string(a[1]) + "]";
    }
    node_s = '"' + node_s + '"';
    return node_s;
}

int heuristic(int a, vector<vector<int>> sel_node, vector<int> step) {
    int h = min(abs(finish[a][0] - sel_node[a][0] - step[0]), abs(finish[a][1] - sel_node[a][1] - step[1])) + abs(abs(finish[a][0] - sel_node[a][0] - step[0]) - abs(finish[a][1] - sel_node[a][1] - step[1]));
    return h;
}

int p_f() {
    cout << "Plows potential fields." << endl;
    for (int l = 0; l < nodes_pf.size(); l++) {
        vector<vector<int>> sel_node = nodes_pf[l];
        for (int ai = 0; ai < 3; ai++) {
            vector<vector<int>> new_node;
            vector<int> a1h;
            for (auto stepp : steps_pf) {
                if (-1 < (sel_node[ai][0] + stepp[0]) && (sel_node[ai][0] + stepp[0]) < map.size() && -1 < (sel_node[ai][1] + stepp[1]) && (sel_node[ai][1] + stepp[1]) < map.size() &&
                    map[sel_node[ai][0] + stepp[0]][sel_node[ai][1] + stepp[1]] == 0 &&
                    find(sel_node.begin(), sel_node.end(), vector<int>{sel_node[ai][0] + stepp[0], sel_node[ai][1] + stepp[1]}) == sel_node.end()) {
                    a1h.push_back(heuristic(ai, sel_node, stepp));
                }
                else {
                    a1h.push_back(100);
                }
            }
            for (int p = 0; p < 3; p++) {
                if (ai == p) {
                    int ma1h = *min_element(a1h.begin(), a1h.end());
                    vector<int> ::iterator itr = find(a1h.begin(), a1h.end(), ma1h);
                    _int64 index = distance(a1h.begin(), itr);
                    vector<int> ch_st = steps_pf[index];
                    if (heuristic(ai, sel_node, { 0, 0 }) >= heuristic(ai, sel_node, ch_st)) {
                        new_node.push_back({ sel_node[p][0] + ch_st[0], sel_node[p][1] + ch_st[1] });
                    }
                    else {
                        new_node.push_back(sel_node[p]);
                    }
                }
                else {
                    new_node.push_back(sel_node[p]);
                }
            }
            sel_node = new_node;
        }
        cout << node2string(sel_node) << endl;
        if (sel_node == finish) {
            return 0;
        }
        if (sel_node != nodes_pf[nodes_pf.size() - 1]) {
            nodes_pf.push_back(sel_node);
        }
        else {
            return 1;
        }
    }
}

int check_add(vector<vector<int>> node, string mother) {
    if (node.size() == 3 && find(nodes.begin(), nodes.end(), node) == nodes.end()) {
        nodes.push_back(node);
        string branch = mother + " -> " + node2string(node);
        file << branch << endl;
        mothers.push_back(mother);
        daughters.push_back(node2string(node));
        if (node == finish) {
            string color = node2string(node) + " [color=" + '"' + "red" + '"' + "]" + "\n" + mother + " [color=" + '"' + "red" + '"' + "]";
            file << color << endl;
            return 0;
        }
    }
    return 1;
}

vector<vector<int>> move_1_agent(int agent, vector<int> step ,vector<vector<int>> pos) {
    vector<vector<int>> node;
    for (int i = 0; i < pos.size(); i++) {
        if (i == agent) {
            if (-1 < (pos[i][0] + step[0]) && (pos[i][0] + step[0]) < map.size() && -1 < (pos[i][1] + step[1]) && (pos[i][1] + step[1]) < map.size() &&
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
    cout << "Search has begun." << endl;
    for (int q = 0; q < nodes.size(); q++) {
        vector<vector<int>> pos = nodes[q];
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
                                if (check_add(node3, mother) == 0) {return 0;}
                            }
                        }
                        else {if (check_add(node2, mother) == 0) {return 0;}}
                    }
                }
                else {if (check_add(node, mother) == 0) {return 0;}}
            }
        }
    }
    return 1;
}

int main() {
    cout << "Map:" << endl;
    printArray(map);
    int sx1, sy1, sx2, sy2, sx3, sy3, tx1, ty1, tx2, ty2, tx3, ty3;
    {
        cout << "1 robot's position: ";
        cin >> sx1 >> sy1;
        cout << "1 robot's target: ";
        cin >> tx1 >> ty1;
        cout << "2 robot's position: ";
        cin >> sx2 >> sy2;
        cout << "2 robot's target: ";
        cin >> tx2 >> ty2;
        cout << "3 robot's position: ";
        cin >> sx3 >> sy3;
        cout << "3 robot's target: ";
        cin >> tx3 >> ty3;
        cout << "1)" << sx1 << " " << sy1 << " -> " << tx1 << " " << ty1 << ";  " << endl;
        cout << "2)" << sx2 << " " << sy2 << " -> " << tx2 << " " << ty2 << ";  " << endl;
        cout << "3)" << sx3 << " " << sy3 << " -> " << tx3 << " " << ty3 << ";  " << endl;
    }
    nodes = { {{sx1, sy1}, {sx2, sy2}, {sx3, sy3}} };
    nodes_pf = { {{sx1, sy1}, {sx2, sy2}, {sx3, sy3}} };
    finish = { {{tx1, ty1}, {tx2, ty2}, {tx3, ty3}} };

    double st_time = clock();
    file.open("forgraph.dot");
    file << "digraph tree{" << endl;
    if (func() == 0) {
        cout << color < 2, 0> << "Graph has been created. Path has been found." << color<7, 0> << endl;
        string path = daughters[daughters.size() - 1] + " -> " + mothers[mothers.size() - 1];
        string start = mothers[0];
        string now = mothers[mothers.size() - 1];
        while (start != now) {
            vector <string> ::iterator it = find(daughters.begin(), daughters.end(), now);
            _int64 index = distance(daughters.begin(), it);
            now = mothers[index];
            string color_p = now + " [color=" + '"' + "red" + '"' + "]";
            file << color_p << endl;
            path += " -> " + now;
        }
        cout << path << endl;
        string color_full_p = path + " [color=" + '"' + "red" + '"' + "]";
        file << color_full_p << endl;
    }
    else {
        cout << color < 4, 0> << "Graph has been created, but Path does not exist." << color<7, 0> << endl;
    }
    file << "}" << endl;
    file.close();
    system("dot -Tpdf -O forgraph.dot");
    double e_time = clock();
    cout << "Time: " << to_string((e_time - st_time)/1000) << endl;

    st_time = clock();
    if (p_f() == 0) {
        cout << color < 2, 0> << "Path has been found." << color<7, 0> << endl;
    }
    else
    {
        cout << color < 4, 0> << "Path is missing or the agent is in a local minimum." << color<7, 0> << endl;
    }
    e_time = clock();
    cout << "Time: " << to_string((e_time - st_time) / 1000) << endl;

    return 0;
}