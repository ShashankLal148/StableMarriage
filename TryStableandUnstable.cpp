#include <bits/stdc++.h>
using namespace std;

struct Person {
    int id;
    vector<int> preference;
    deque<int> remaining;
    int engaged_to = -1;
};

vector<Person> men, women;
map<int, int> women_index;

bool prefers(int woman, int new_man, int current_man) {
    for (int m : women[woman].preference) {
        if (m == new_man) return true;
        if (m == current_man) return false;
    }
    return false;
}

// STRONG1 Algorithm
void strong1(int n) {
    queue<int> free_men;
    for (int i = 0; i < n; i++) 
        if (!men[i].preference.empty()) free_men.push(i);

    while (!free_men.empty()) {
        int m = free_men.front(); free_men.pop();
        while (!men[m].remaining.empty()) {
            int w = men[m].remaining.front(); men[m].remaining.pop_front();
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

// STRONG2 Algorithm
void strong2(int n) {
    strong1(n);

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
        int w = *U.begin(); U.erase(U.begin());
        int m = women[w].engaged_to;
        if (m != -1) {
            women[w].engaged_to = -1;
            men[m].engaged_to = -1;
        }
        while (!women[w].remaining.empty()) {
            int last_man = women[w].remaining.back(); women[w].remaining.pop_back();
            if (men[last_man].engaged_to == w) {
                men[last_man].engaged_to = -1;
            }
        }
    }
    
    bool found_matching = false;
    for (int i = 0; i < n; i++) {
        if (men[i].engaged_to != -1) {
            found_matching = true;
            break;
        }
    }

    if (!found_matching) {
        cout << "No Strongly Stable Matching Exists\n";
        return;
    }

    cout << "Strongly Stable Matching Exists:\n";
    for (int i = 0; i < n; i++) {
        if (men[i].engaged_to != -1)
            cout << "Man " << i+1 << " is matched with Woman " << men[i].engaged_to+1 << "\n";
        else
            cout << "Man " << i+1 << " remains unmatched\n";
    }
}

// Function to Run All Test Cases
void runTestCase(string name, vector<Person> men_input, vector<Person> women_input, int n) {
    cout << "Running Test Case: " << name << "\n";
    
    men = men_input;
    women = women_input;

    for (int i = 0; i < n; i++) {
        for (int w : men[i].preference) 
            men[i].remaining.push_back(w);
        women_index[women[i].id] = i;
    }

    strong2(n);
    cout << "----------------------------------------\n";
}

// Main Function
int main() {
    int n = 3; // Number of men and women in most cases

    // Test Case 1: Normal Case (Stable Matching Exists)
    runTestCase("Normal Case", 
        {
            {0, {0, 1, 2}}, 
            {1, {1, 0, 2}}, 
            {2, {2, 0, 1}}
        },
        {
            {0, {1, 0, 2}}, 
            {1, {2, 0, 1}}, 
            {2, {0, 1, 2}}
        }, 
        n
    );

    // Test Case 2: Another Stable Matching Exists
    runTestCase("Another Stable Matching Exists", 
        {
            {0, {0, 1}}, 
            {1, {1, 0}}, 
            {2, {1, 2, 0}}
        },
        {
            {0, {0, 2, 1}}, 
            {1, {1, 2, 0}}, 
            {2, {2, 0, 1}}
        }, 
        n
    );

    // Test Case 3: No Stable Matching Exists
    runTestCase("No Stable Matching Exists", 
        {
            {0, {1, 2}}, 
            {1, {2, 0}}, 
            {2, {0, 1}}
        },
        {
            {0, {2, 1}}, 
            {1, {0, 2}}, 
            {2, {1, 0}}
        }, 
        n
    );

    // Test Case 4: Ties and Incomplete Lists (Stable Matching Exists)
    runTestCase("Preferences with Ties and Incomplete Lists", 
        {
            {0, {0, 1}},    
            {1, {1}},       
            {2, {}}         
        },
        {
            {0, {0}},       
            {1, {1, 0}},    
            {2, {}}
        }, 
        n
    );

    // Test Case 5: Ties and Incomplete Lists (No Stable Matching Exists)
    runTestCase("Ties and Incomplete Lists - No Matching Exists", 
        {
            {0, {1}},    
            {1, {0}},    
            {2, {}}      
        },
        {
            {0, {1}},    
            {1, {0}},    
            {2, {}}      
        }, 
        n
    );

    return 0;
}
