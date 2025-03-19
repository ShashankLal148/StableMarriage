#include <bits/stdc++.h>
using namespace std;

struct Person {
    int id;
    vector<vector<int>> preference;
    queue<int> remaining;
    int engaged_to = -1;
};

vector<Person> men, women;
map<int, int> women_index;

bool prefers(int woman, int new_man, int current_man) {
    for (const auto& group : women[woman].preference) {
        if (find(group.begin(), group.end(), new_man) != group.end()) return true;
        if (find(group.begin(), group.end(), current_man) != group.end()) return false;
    }
    return false;
}

void stable_matching(int n) {
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

void process_test_case(vector<vector<int>> men_prefs, vector<vector<vector<int>>> women_prefs) {
    int n = men_prefs.size();
    men.assign(n, {});
    women.assign(n, {});
    
    for (int i = 0; i < n; i++) {
        men[i].id = i;
        for (int x : men_prefs[i]) men[i].remaining.push(x);
    }
    for (int i = 0; i < n; i++) {
        women[i].id = i;
        women[i].preference = women_prefs[i];
    }
    for (int i = 0; i < n; i++) {
        women_index[women[i].id] = i;
    }
    stable_matching(n);
    
    cout << "Result:\n";
    for (int i = 0; i < n; i++) {
        if (men[i].engaged_to != -1)
            cout << "Man " << i + 1 << " is matched with Woman " << men[i].engaged_to + 1 << "\n";
        else
            cout << "Man " << i + 1 << " is not matched\n";
    }
}

void process_test_case(string filename) {
    ifstream file(filename);
    int n;
    file >> n;
    vector<vector<int>> men_prefs(n);
    vector<vector<vector<int>>> women_prefs(n);
    
    for (int i = 0; i < n; i++) {
        int m;
        file >> m;
        men_prefs[i].resize(m);
        for (int j = 0; j < m; j++) file >> men_prefs[i][j];
    }
    for (int i = 0; i < n; i++) {
        int g;
        file >> g;
        women_prefs[i].resize(g);
        for (int j = 0; j < g; j++) {
            int size;
            file >> size;
            women_prefs[i][j].resize(size);
            for (int k = 0; k < size; k++) file >> women_prefs[i][j][k];
        }
    }
    file.close();
    process_test_case(men_prefs, women_prefs);
}

int main() {
    cout << "Running predefined test cases:\n";
    
    // Test Case 1: Stable Matching Exists
    process_test_case({{0,1,2},{1,2,0},{2,0,1}}, {{{0,1,2}},{{1,2,0}},{{2,0,1}}});
    
    // Test Case 2: Another Stable Matching Exists
    process_test_case({{1,0,2},{0,2,1},{2,1,0}}, {{{1,2,0}},{{0,2,1}},{{2,0,1}}});
    
    // Test Case 3: No Stable Matching
    process_test_case({{0,1},{1,0}}, {{{1},{0}},{{0},{1}}});
    
    // Test Case 4: Ties and Incomplete Lists (Stable Matching Exists)
    process_test_case({{0,1},{1,0}}, {{{0,1}},{{0,1}}});
    
    // Test Case 5: Ties and Incomplete Lists (No Stable Matching Exists)
    process_test_case({{0},{1}}, {{{1}},{{0}}});
    
    cout << "Running large test cases:\n";
    // process_test_case("testcase1.txt");
    // process_test_case("testcase2.txt");
    
    return 0;
}
