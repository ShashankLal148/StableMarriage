#include <bits/stdc++.h>
using namespace std;

struct Person {
    int id;
    vector<vector<int>> preferences; // Handles ties in preferences
    set<int> proposals;
    int partner = -1;
};

vector<Person> men, women;

bool prefers(int w, int m1, int m2) {
    for (auto &group : women[w].preferences) {
        if (find(group.begin(), group.end(), m1) != group.end()) return true;
        if (find(group.begin(), group.end(), m2) != group.end()) return false;
    }
    return false;
}

void strong1(int n) {
    queue<int> freeMen;
    for (int i = 0; i < n; i++) freeMen.push(i);
    while (!freeMen.empty()) {
        int m = freeMen.front();
        freeMen.pop();
        for (auto &group : men[m].preferences) {
            for (int w : group) {
                if (men[m].proposals.count(w)) continue;
                men[m].proposals.insert(w);
                if (women[w].partner == -1) {
                    women[w].partner = m;
                    men[m].partner = w;
                    break;
                } else {
                    int m2 = women[w].partner;
                    if (!prefers(w, m2, m)) {
                        women[w].partner = m;
                        men[m].partner = w;
                        men[m2].partner = -1;
                        freeMen.push(m2);
                        break;
                    }
                }
            }
        }
    }
}

void printMatching(int n) {
    for (int i = 0; i < n; i++) {
        cout << "Man " << i << " is matched with Woman " << men[i].partner << "\n";
    }
}

void test() {
    int n = 3;
    men = {{0, {{1}, {2}}}, {1, {{0, 1}}}, {2, {{2}}}};  // Ties and incomplete lists
    women = {{0, {{1}}}, {1, {{0, 1}}}, {2, {{0, 1, 2}}}};
    
    strong1(n);
    cout << "Matching using STRONG1:\n";
    printMatching(n);
}

int main() {
    test();
    return 0;
}
