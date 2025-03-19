#include <bits/stdc++.h>
using namespace std;

struct Person {
    int id;
    vector<vector<int>> preference; // Handles ties
    queue<int> remaining;
    int engaged_to = -1;
};

vector<Person> men, women;
map<int, int> women_index;

bool prefers(int woman, int new_man, int current_man) {
    for (auto group : women[woman].preference) {
        if (find(group.begin(), group.end(), new_man) != group.end()) return true;
        if (find(group.begin(), group.end(), current_man) != group.end()) return false;
    }
    return false;
}

void strong1(int n) {
    queue<int> free_men;
    for (int i = 0; i < n; i++) free_men.push(i);

    while (!free_men.empty()) {
        int m = free_men.front(); free_men.pop();
        while (!men[m].remaining.empty()) {
            int w = men[m].remaining.front(); men[m].remaining.pop();
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

void strong2(int n) {
    strong1(n);
    set<int> Z;
    for (int i = 0; i < n; i++) {
        if (men[i].engaged_to == -1) {
            Z.insert(i);
        }
    }
    if (!Z.empty()) {
        cout << "No Strongly Stable Matching Exists\n";
        return;
    }

    for (int w = 0; w < n; w++) {
        int current_man = women[w].engaged_to;
        for (auto group : women[w].preference) {
            for (int better_man : group) {
                if (better_man == current_man) break;
                int better_man_partner = men[better_man].engaged_to;
                if (better_man_partner != -1 && prefers(better_man, w, better_man_partner)) {
                    cout << "No Strongly Stable Matching Exists\n";
                    return;
                }
            }
        }
    }

    cout << "Strongly Stable Matching Exists:\n";
    for (int i = 0; i < n; i++) {
        cout << "Man " << i+1 << " is matched with Woman " << men[i].engaged_to+1 << "\n";
    }
}

void test_case_incomplete_prefs() {
    int n = 3;
    men = {
        {0, {{0}, {1}}}, // M1 prefers W1 > W2 (W3 not ranked)
        {1, {{1}, {2}}}, // M2 prefers W2 > W3 (W1 not ranked)
        {2, {{2}, {0}}}  // M3 prefers W3 > W1 (W2 not ranked)
    };
    women = {
        {0, {{0}, {2}}}, // W1 prefers M1 > M3 (M2 not ranked)
        {1, {{1}, {0}}}, // W2 prefers M2 > M1 (M3 not ranked)
        {2, {{2}, {1}}}  // W3 prefers M3 > M2 (M1 not ranked)
    };

    for (int i = 0; i < n; i++) {
        for (auto group : men[i].preference)
            for (int w : group)
                men[i].remaining.push(w);
        women_index[women[i].id] = i;
    }

    cout << "\nRunning Test Case: Incomplete Preferences\n";
    strong2(n);
}

void test_case_ties() {
    int n = 3;
    men = {
        {0, {{0, 1}, {2}}}, // M1 prefers (W1, W2) > W3
        {1, {{1}, {0, 2}}}, // M2 prefers W2 > (W1, W3)
        {2, {{2}, {0, 1}}}  // M3 prefers W3 > (W1, W2)
    };
    women = {
        {0, {{0, 1}, {2}}}, // W1 prefers (M1, M2) > M3
        {1, {{1}, {0, 2}}}, // W2 prefers M2 > (M1, M3)
        {2, {{2}, {0, 1}}}  // W3 prefers M3 > (M1, M2)
    };

    for (int i = 0; i < n; i++) {
        for (auto group : men[i].preference)
            for (int w : group)
                men[i].remaining.push(w);
        women_index[women[i].id] = i;
    }

    cout << "\nRunning Test Case: Preferences with Ties\n";
    strong2(n);
}

int main() {
    test_case_incomplete_prefs();
    test_case_ties();
    return 0;
}
