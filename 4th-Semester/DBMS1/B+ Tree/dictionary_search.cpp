#include<bits/stdc++.h>
using namespace std;
#define endl          '\n'
#define all(v)        v.begin(), v.end()
#define rep(i, s, n)  for(int i = s; i <= n; i++)
#define rev(i, n, s)  for(int i = n; i >= s; i--)
typedef long long ll;
typedef long double ld;
typedef pair<int, int> pii;
typedef vector<vector<int>> vii;

/*****Properties of B+ Tree*****/

 
/*****Helper Functions*****/
const int M = 3;      // order of B+ Tree

struct node {
    int size;
    bool leaf;
    node **pointer;
    node *parent, *next;
    string *word, *meaning;

    node(int M) {
        size = 0;
        leaf = true;
        parent = next = NULL;
        word = new string[M];
        meaning = new string[M];
        pointer = new node*[M+1];
        for(int i = 0; i <= M; i++)
            pointer[i] = NULL;
    }
};

node *root = new node(M);

// insert key and internal node to parent
void insert_to_parent(node *parent, string word, node *right) {
    int i = parent->size - 1;
    while(i >= 0) {
        if(parent->word[i] <= word)
            break;
        else {
            parent->word[i+1] = parent->word[i];
            parent->pointer[i+2] = parent->pointer[i+1];
        }
        i -= 1;
    }
    parent->word[i+1] = word;
    parent->pointer[i+2] = right;
    parent->size += 1;
}

// find leaf node that stores the key "word"
node *find_leaf_node(string word) {
    node *tmp_root = root;
    while(!tmp_root->leaf) {
        int i = 0;
        for(; i < tmp_root->size; i++) {
            if(word < tmp_root->word[i]) 
                break;
        }
        tmp_root = tmp_root->pointer[i];
    }
    return tmp_root;
}

// update internal node after inserting leaf node, if needed
void insert_internal_node(node *parent, string word, node *left, node *right) {
    // new root should create
    if(parent == NULL) {
        parent = new node(M);
        parent->word[0] = word;
        parent->pointer[0] = left;
        parent->pointer[1] = right;
        parent->size = 1;
        parent->leaf = false;
        root = parent;
        left->parent = parent;
        right->parent = parent;
        return;
    }

    right->parent = parent;
    insert_to_parent(parent, word, right);
    if(parent->size == M) {
        node *new_node = new node(M);
        new_node->leaf = false;

        int i, j = 0;
        for(i = parent->size-(M-1)/2; i < M; i++, j++) {
            new_node->word[j] = parent->word[i];
            if(j == 0) {
                new_node->pointer[0] = parent->pointer[i];
                new_node->pointer[0]->parent = new_node;
            }
            new_node->pointer[j+1] = parent->pointer[i+1];
            new_node->pointer[j+1]->parent = new_node;
        }
        
        parent->size -= ((M-1)/2 + 1);
        new_node->size = (M-1)/2;
        insert_internal_node(parent->parent, parent->word[parent->size], parent, new_node);
    }
}

// insert data at leaf node
void insert_leaf_node(string word, string meaning) {
    node *leaf = find_leaf_node(word);

    int i = leaf->size - 1;
    while(i >= 0) {
        if(leaf->word[i] <= word)
            break;
        else {
            leaf->word[i+1] = leaf->word[i];
            leaf->meaning[i+1] = leaf->meaning[i]; 
        }
        i -= 1;
    }

    leaf->word[i+1] = word;
    leaf->meaning[i+1] = meaning;
    leaf->size += 1;

    if(leaf->size == M) {
        node *new_leaf = new node(M);

        int i, j = 0;
        for(i = leaf->size - M/2; i < M; i++, j++) {
            new_leaf->word[j] = leaf->word[i];
            new_leaf->meaning[j] = leaf->meaning[i];
        }

        leaf->size -= M/2;
        new_leaf->size = M/2;
        new_leaf->next = leaf->next;
        leaf->next = new_leaf;
        insert_internal_node(leaf->parent, new_leaf->word[0], leaf, new_leaf);
    }
}

void insert(string word, string meaning) {
    insert_leaf_node(word, meaning);
}

string search(string word) {
    node *leaf = find_leaf_node(word);
    for(int i = 0; i < leaf->size; i++) {
        if(word == leaf->word[i])
            return leaf->meaning[i];
    }
    return "not found!";
}

/*****main Function*****/
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);

    assert(M >= 3);
    root = new node(M);

    // freopen("input.txt", "r", stdin);
    // freopen("output.txt", "w", stdout);

    int n; 
    cin >> n;

    vector<string> s;
    while(n--) {
        string a, b;
        cin >> a >> b;
        insert(a, b);
        s.push_back(a);
    }

    for(string st: s)
        cout << st << " -> " << search(st) << endl;
    

    // freopen("input.txt", "r", stdin);


    // int cnt = 100000;
    // vector<string> words;
    // for(int i = 0; i < cnt; i++) {
    //     string word; 
    //     cin >> word;
    //     words.push_back(word);
    //     insert(word, word);
    // }
    
    // for(string &word: words) {
    //    if(word != search(word))
    //         cout << word << endl;
    // }

    cout << "Execution Finished." << endl;

    return 0;
} 