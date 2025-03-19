#include <bits/stdc++.h>
using namespace std;

// Constants
const int INF = 1e9;

// Data structures
struct Person {
    int id;
    vector<int> preference;
    queue<int> remaining;
    int engaged_to = -1;
    bool proposed = false;
};

vector<Person> men, women;
map<int, int> women_index; // Maps woman ID to index in women[]

// Function to check if a woman prefers a new proposal over her current partner
bool prefers(int woman, int new_man, int current_man) {
    for (int m : women[woman].preference) {
        if (m == new_man) return true;
        if (m == current_man) return false;
    }
    return false;
}

// STRONG1 Algorithm Implementation (Stable Matching)
void strong1(int n) {
    queue<int> free_men;
    for (int i = 0; i < n; i++) free_men.push(i);

    while (!free_men.empty()) {
        int m = free_men.front();
        free_men.pop();
        while (!men[m].remaining.empty()) {
            int w = men[m].remaining.front();
            men[m].remaining.pop();
            if (women[w].engaged_to == -1) {
                women[w].engaged_to = m;
                men[m].engaged_to = w;
                break;
            } else {
                int m0 = women[w].engaged_to;
                if (prefers(w, m, m0)) {
                    women[w].engaged_to = m;
                    men[m].engaged_to = w;
                    men[m0].engaged_to = -1;
                    free_men.push(m0);
                    break;
                }
            }
        }
    }
}

// STRONG2 Algorithm Implementation
void strong2(int n) {
    strong1(n); // Start with STRONG1 matching

    set<int> Z;
    vector<int> critical_set(n, 0);
    for (int i = 0; i < n; i++) {
        if (men[i].engaged_to == -1) {
            Z.insert(i);
            critical_set[i] = 1;
        }
    }
    
    set<int> U;
    for (int m : Z) {
        if (men[m].engaged_to != -1) U.insert(men[m].engaged_to);
    }
    
    while (!U.empty()) {
        int w = *U.begin();
        U.erase(U.begin());
        int m = women[w].engaged_to;
        if (m != -1) {
            women[w].engaged_to = -1;
            men[m].engaged_to = -1;
        }
    }
    
    for (int i = 0; i < n; i++) {
        if (men[i].engaged_to == -1) {
            cout << "No Strongly Stable Matching Exists\n";
            return;
        }
    }
    cout << "Strongly Stable Matching Exists:\n";
    for (int i = 0; i < n; i++) {
        cout << "Man " << i+1 << " is matched with Woman " << men[i].engaged_to+1 << "\n";
    }
}

// Test function
void test() {
    int n = 3;
    // men = {{0, {0, 1, 2}}, {1, {1, 2, 0}}, {2, {2, 0, 1}}};
    // women = {{0, {0, 1, 2}}, {1, {1, 2, 0}}, {2, {2, 0, 1}}};
    // men = {{0, {1, 2, 0}}, {1, {2, 0, 1}}, {2, {0, 1, 2}}};
    // women = {{0, {2, 1, 0}}, {1, {0, 2, 1}}, {2, {1, 0, 2}}};
//     men = {{0, {0, 1, 2}}, {1, {0, 2, 1}}, {2, {0, 1, 2}}};
// women = {{0, {1, 0, 2}}, {1, {2, 0, 1}}, {2, {0, 1, 2}}};
men = {
    {0, {0, 1, 2}}, // M1 prefers W1 > W2 > W3
    {1, {1, 2, 0}}, // M2 prefers W2 > W3 > W1
    {2, {2, 0, 1}}  // M3 prefers W3 > W1 > W2
};
women = {
    {0, {1, 0, 2}}, // W1 prefers M2 > M1 > M3 (Cycle created)
    {1, {2, 1, 0}}, // W2 prefers M3 > M2 > M1 (Cycle created)
    {2, {0, 2, 1}}  // W3 prefers M1 > M3 > M2 (Cycle created)
};
    for (int i = 0; i < n; i++) {
        men[i].id = i;
        women[i].id = i;
        for (int w : men[i].preference) men[i].remaining.push(w);
        women_index[women[i].id] = i;
    }
    strong2(n);
}

int main() {
    test();
    return 0;
}
