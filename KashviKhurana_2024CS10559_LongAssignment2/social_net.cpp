#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <cmath>
#include <queue>
#include <stack>
#include <map>
#include <set>
#include <cctype>
#include <unordered_map>
#include <ctime>
#include <sstream>
#include <stdexcept>

using namespace std;

struct PostNode{
    string content;
    time_t timestamp;
    PostNode* left;
    PostNode* right;
    int height;
    PostNode(string postcont){
        content = postcont;
        timestamp = time(nullptr);
        left = nullptr;
        right = nullptr;
        height = 1;

    }

};

class AVLposts{
    public:
    PostNode* root;

    AVLposts(){
        root = nullptr;
    }
    ~AVLposts() {
        clear(root);
    }
    void clear(PostNode* p) {
        if (p) { 
            clear(p->left);  
            clear(p->right); 
            delete p;      
        }
    }

    PostNode* insert(PostNode* p, PostNode* tba){ // to be added = tba

        if (!p){
            if (!root) root = tba;
            return tba;
        }

        if (tba->timestamp<p->timestamp){
            p->left = insert(p->left,tba);
        }
        else {
            p->right = insert(p->right,tba);
        }

        p->height = max(h(p->left), h(p->right)) + 1;
        int b = balance(p);

        if (b < -1 && p->right->timestamp < tba->timestamp){
            return LRotate(p);
        }
        if (b > 1 && p->left->timestamp > tba->timestamp){
            return RRotate(p);
        }
        if (b < -1 && p->right->timestamp > tba->timestamp){
            p->right = RRotate(p->right);
            return LRotate(p);
        }
        if (b > 1 && p->left->timestamp < tba->timestamp){
            p->left = LRotate(p->left);
            return RRotate(p);
        }

        return p;

    }

    PostNode* RRotate(PostNode* p){
        PostNode* q = p->left;

        p->left = q->right;
        q->right = p;

        p->height = max(h(p->left), h(p->right)) + 1;
        q->height = max(h(q->left), h(q->right)) + 1;
        return q;


    }
    PostNode* LRotate(PostNode* p){
        PostNode* q = p->right;

        p->right = q->left;
        q->left = p;

        p->height = max(h(p->left), h(p->right)) + 1;
        q->height = max(h(q->left), h(q->right)) + 1;
        return q;


    }
    int balance(PostNode* p){
        if (!p) return 0;
        return h(p->left) - h(p->right);

    }
    int h(PostNode* p){
        if (!p) return 0;
        return p->height;
    }

    // posts are always added, with a greater time stamp as well
    // so like hamesha rightmost add ho rahe lmao

};


struct user; // pre declaration, i want to stare an AVL neighbours in this

struct userNode{
    public:
    user* who;
    userNode* left;
    userNode* right;
    int height;
    userNode(user* node){
        who = node;
        left = nullptr;
        right = nullptr;
        height = 1;
    }
};

class AVLneighbours{
    public:
    userNode* root;

    AVLneighbours(){
        root = nullptr;
    }
    ~AVLneighbours() {
        clear(root);
    }
    void clear(userNode* p) {
        if (p) {
            clear(p->left);
            clear(p->right);
            delete p; 
        }
    }
    
    userNode* insert(userNode* p, userNode* tba); // predeclraration; uses user

    userNode* RRotate(userNode* p){
        userNode* q = p->left;
        p->left = q->right;
        q->right = p;
        p->height = max(h(p->left), h(p->right)) + 1;
        q->height = max(h(q->left), h(q->right)) + 1;
        return q;
    }
    userNode* LRotate(userNode* p){
        userNode* q = p->right;
        p->right = q->left;
        q->left = p;
        p->height = max(h(p->left), h(p->right)) + 1;
        q->height = max(h(q->left), h(q->right)) + 1;
        return q;
    }
    int balance(userNode* p){
        if (!p) return 0;
        return h(p->left) - h(p->right);
    }
    int h(userNode* p){
        if (!p) return 0;
        return p->height;
    }
};

struct user{
    public:
    string username;
    int id;
    AVLposts myPosts;
    AVLneighbours myFriends; 
    user(string name, int num){
        username = name;
        id = num;
        AVLposts P;
        myPosts = P;
        AVLneighbours F;
        myFriends = F;
    }
};

userNode* AVLneighbours::insert(userNode* p, userNode* tba){ // to be added = tba
    if (!p){
        if (!root) root = tba;
        return tba;
    }

    if (tba->who->username < p->who->username){
        p->left = insert(p->left,tba);
    }
    else if (tba->who->username > p->who->username){
        p->right = insert(p->right,tba);
    }
    else{
        return nullptr;
    }

    p->height = max(h(p->left), h(p->right)) + 1;
    int b = balance(p);

    if (b < -1 && p->right->who->username < tba->who->username){
        return LRotate(p);
    }
    if (b > 1 && p->left->who->username > tba->who->username){
        return RRotate(p);
    }
    if (b < -1 && p->right->who->username > tba->who->username){
        p->right = RRotate(p->right);
        return LRotate(p);
    }
    if (b > 1 && p->left->who->username < tba->who->username){
        p->left = LRotate(p->left);
        return RRotate(p);
    }

    return p;
}

class Heap{
    public:
    vector<pair<user*, int>> a;
    
    Heap(){
        vector<pair<user*, int>> all(0);
        a = all;
    }  
    void insert(pair<user*, int> k){
        a.push_back(k);
        int s = a.size();
        heapifyUp(s-1);       
    }
    int left(int i) {
        return 2*i + 1;
    }
    int right(int i){
        return 2*i + 2;
    }
    int parent(int i){
        return (i-1)/2;
    }
    void build_heap(vector<pair<user*, int>> num){
        a = num;
        int n = num.size();
        for (int i = n-1; i>=0; i--){
            heapifyDown(i);

        }
    }   
    void heapifyUp(int i){
        if (i==0){
            return;
        }
        else{
            int p = parent(i);
            
            if (a[p].second > a[i].second || (a[p].second == a[i].second && a[p].first->username < a[i].first->username)) {
                swap(a[p],a[i]);
                heapifyUp(p);
            }
            else{
                return;
            }
        }    
    }
    
    void heapifyDown(int i){
        if (left(i) >= a.size()){
            return;
        }
        int l = left(i);
        int r = right(i);
        int k;
        if (r >= a.size()){
            k = l;
        }
        else{
            if (a[l].second < a[r].second || (a[l].second == a[r].second && a[l].first->username > a[r].first->username)) {
                k = l;
            }
            else {
                k = r;
            }
        }
        
        if (a[k].second < a[i].second || (a[k].second == a[i].second && a[k].first->username > a[i].first->username)) {
            swap(a[k],a[i]);
            heapifyDown(k);
        }
        else{
            return;
        }
        
    }
    void poptop(){
        a[0] = a[a.size()-1];
        a.pop_back();
        heapifyDown(0);

    }
    pair<user*, int> extract(){
        pair<user*, int> p = a[0];
        a[0] = a[a.size()-1];
        a.pop_back();
        heapifyDown(0);
        return p;
        
    }
 
};

class SocialNet{
    public: 
    
    unordered_map <string, user*> Map;
    vector<user*> U; // map users to integer ids and store these ids in user data structure so this can be accessed easily
    int total;

    SocialNet(){
        total = 0;
    }
    ~SocialNet() {
        for (int i = 0; i < total; i++) {
            delete U[i];
        }
    }
    bool addUser(string name){
        if (Map.count(name) > 0) return false;
        user* temp = new user(name, total);
        Map[name] = temp;
        U.push_back(temp);
        total++;
        return true;

    }
    bool addEdge(string a1, string b1){
        user* a = Map[a1];
        user* b = Map[b1];
        userNode* aNode = new userNode(a);
        userNode* bNode = new userNode(b);
        userNode* atree = a->myFriends.insert(a->myFriends.root, bNode);
        userNode* btree = b->myFriends.insert(b->myFriends.root, aNode);
        if (!atree) return false;
        a->myFriends.root = atree;
        b->myFriends.root = btree;
        return true;
    }
    void inorderHelperNames(userNode* c){
        if (!c) return;
        inorderHelperNames(c->left);
        cout << c->who->username << endl;
        inorderHelperNames(c->right);

    }
    void listFriends(string name){
        //inorder of uska myfriends tree mei username output
        //cout here only
        user* a = Map[name];
        if (!a->myFriends.root){
            cout << name << " has no friends." << endl;
            return;
        }
        inorderHelperNames(a->myFriends.root);
        cout << "All friends of " << name << " have been listed." << endl;

    }
    void inorderHelperMutuals(queue<user*> &q, vector<bool>&fr, userNode* node){
        if (!node) return;
        inorderHelperMutuals(q,fr,node->left);
        q.push(node->who);
        fr[node->who->id] = true;
        inorderHelperMutuals(q,fr,node->right);
    }
    void countMutuals(vector<int>& mut, vector<bool>&fr, userNode* p){
        if (!p) return;
        countMutuals(mut, fr, p->left);
        if (!fr[p->who->id]){
            mut[p->who->id]++;
        }
        
        countMutuals(mut,fr, p->right);
    }

    void sugFriends(string usera, int n){
        //output friends of friends with max mutuals
        //kuch kind of bfs till two layers laga do
        //cout here only
        //use id from user struct itself to work with an explored list 
        user* a = Map[usera];
        vector<bool>alrFr(total,0);
        alrFr[a->id] = 1;
        queue<user*>q;
        inorderHelperMutuals(q,alrFr,a->myFriends.root);
        vector<int>mut(total,0);
        //put number of mutuals of every suggestable friend in this list
        while(!q.empty()){
            user* u = q.front();
            q.pop();
            countMutuals(mut, alrFr, u->myFriends.root);
        }
        // i could also just have sorted the list and not done the whole heap stuff but ok
        bool isn = false;
        Heap A;
        int i = 0;
        for (int j = 0; j<total; j++){
            if (mut[j] > 0){
                if (i<n){
                    A.insert({U[j], mut[j]});
                }
                else{
                    isn = true;
                    if (mut[j] < A.a[0].second || (mut[j] == A.a[0].second && U[j]->username > A.a[0].first->username)){
                        continue;
                    }
                    else{
                        A.insert({U[j],mut[j]});
                        A.poptop();
                    }

                }
                i++;
            }
        }

        stack<string> st;
        int j = A.a.size();
        pair<user*,int> temp;
        while(j>0){
            temp = A.extract();
            st.push(temp.first->username);
            j--;
        }
        while (!st.empty()){
            cout << st.top() << endl;
            st.pop();
        }
        if (!isn){
            cout << "There are less than " << n << " suggestable friends of " << usera << ", all of them have been listed." << endl;
        }
        else{
            cout << "Top " << n << " friends of friends with maximum mutuals have been suggested for " << usera  << "." << endl;
        }


    }
    
    void BFShelper(vector<bool>&ex, vector<int>&dist, userNode* c, user*b, int d, queue<pair<user*, int>> &q){
        if (!c) return;
        user* curr = c->who;
        BFShelper(ex,dist,c->left,b, d, q);
        if (!ex[curr->id]){
            ex[curr->id] = true;
            dist[curr->id] = d;
            q.push({curr, d});
            if (curr == b){
                return;
            }
        }
        BFShelper(ex,dist,c->right,b, d, q);
        
        
    }
    int distance(string usera, string userb){
        user* a = Map[usera];
        user* b = Map[userb];
        queue<pair<user*, int>> q;
        q.push({a, 0});
        vector<bool> ex(total, false);
        vector<int>dist(total, -1);
        ex[a->id] = true;
        dist[a->id] = 0;
    
        user* p;
        int d;
        while(!q.empty()){
            p = q.front().first;
            d = q.front().second;
            q.pop();
            BFShelper(ex,dist,p->myFriends.root,b,d+1, q);
            if (ex[b->id]) break;
        }
        return dist[b->id];

    }
    void addPost(string name, string conti){
        user* temp = Map[name];
        PostNode* newpost = new PostNode(conti);
        PostNode* v = temp->myPosts.insert(temp->myPosts.root, newpost);
        temp->myPosts.root = v;


    }
    void posthelper(vector<PostNode*>& all, PostNode* node){
        if (!node) return;
        posthelper(all, node->left);
        all.push_back(node);
        posthelper(all, node->right);

    }
    void outputPosts(string name, int n){
        vector<PostNode*> allp;
        user* c = Map[name];
        posthelper(allp, c->myPosts.root);
        int m = allp.size();
        if (n == -1 || n > m){
            for (int i = m-1; i>=0; i--){
                cout << allp[i]->content << endl;
            }
            cout << "All " << m <<  " posts of " << name << " printed." << endl;
        }
        else{
            for (int i = m-1; i>= m-n; i--){
                cout << allp[i]->content << endl;
            }
            cout << "Most recent " << n <<  " posts of " << name << " printed." << endl;

        }

    }


};

int main(){
    string line;
    cout << "> "; //prompt
    SocialNet F;

    while (getline(cin, line)) {
        stringstream s(line);
        string command;
        s >> command;

        if (command == "ADD_USER") {
            try{
                string Username;
                s >> Username;
                transform(Username.begin(), Username.end(), Username.begin(), ::tolower);

                if (Username.empty()){
                    cout << "Error: Please enter a valid username." << endl;
                    continue;
                }
                else if (F.addUser(Username)) {
                    cout << "User '" << Username << "' created." << endl;
                } else {
                    cout << "Error: User '" << Username << "' already exists." << endl;
                }
            }
            catch (const out_of_range& e) {
                cout << "Error: user not found." << endl;
            }
        }
        
        else if (command == "ADD_FRIEND") {
            try{
                string username1, username2;
                s >> username1;
                s >> username2;
                transform(username1.begin(), username1.end(), username1.begin(), ::tolower);
                transform(username2.begin(), username2.end(), username2.begin(), ::tolower);
                if (username1.empty() || username2.empty()) {
                    cout << "Error: Please provide two valid usernames." << endl;
                    continue;
                }

                if (F.Map.count(username1) == 0 || F.Map.count(username2) == 0){
                    cout << "At least one of these users does not exist." << endl;
                    continue;
                }
                if (F.addEdge(username1, username2)) { // Assuming F is your SocialNet object
                    cout << "Friendship created between " << username1 << " and " << username2 << "." << endl;
                }
                else {
                    cout << "Friendship already exists between" << username1 << " and " << username2 << "." << endl;
                }
            }
            catch (const out_of_range& e) {
                cout << "Error: user not found." << endl;
            }
        }
        else if (command == "LIST_FRIENDS") {
            try{
                string Username;
                s >> Username;
                transform(Username.begin(), Username.end(), Username.begin(), ::tolower);
                if (Username.empty()){
                    cout << "Error: Please enter a valid username." << endl;
                    continue;
                }
                
                if (F.Map.count(Username) == 0) {
                    cout << "This user does not exist, please enter a valid username." << endl;
                    continue;
                }
                F.listFriends(Username);
            }
            catch (const out_of_range& e) {
                cout << "Error: user not found." << endl;
            }

        }
        else if (command == "SUGGEST_FRIENDS"){
            try{
                string Username, num;
                s >> Username;
                transform(Username.begin(), Username.end(), Username.begin(), ::tolower);
                if (Username.empty()){
                    cout << "Error: Please enter a valid username." << endl;
                    continue;
                }
                if (F.Map.count(Username) == 0) {
                    cout << "This user does not exist, please enter a valid username." << endl;
                    continue;
                }
                getline(s, num); 
                if (num.empty()) {
                    cout << "Please enter valid number." << endl;              
                    continue;
                }
                
                int n = stoi(num);
                if (n == 0){
                    cout << 0 << " friends suggested for " << Username << "." << endl;
                    continue;
                }

                
                F.sugFriends(Username, n);
                
            }
            catch (const out_of_range& e) {
                cout << "Error: user not found." << endl;
            }

        }
        else if (command == "DEGREES_OF_SEPARATION"){
            try{
                string username1, username2;
                s >> username1;
                s >> username2;
                transform(username1.begin(), username1.end(), username1.begin(), ::tolower);
                transform(username2.begin(), username2.end(), username2.begin(), ::tolower);
                if (username1.empty() || username2.empty()) {
                    cout << "Error: Please provide two valid usernames." << endl;
                    continue;
                }

                if (F.Map.count(username1) == 0 || F.Map.count(username2) == 0){
                    cout << "At least one of these users does not exist." << endl;
                    continue;
                }
                int m = F.distance(username1, username2);
                if(m == -1){
                    cout << "There does not exist a path between " << username1 << " and " << username2 << ". (distance is -1)" << endl;
                }
                else{
                    cout << "The shortest path between " << username1 << " and " << username2 << " is " << m << "." << endl;
                }
            }
            catch (const out_of_range& e) {
                cout << "Error: user not found." << endl;
            }

        }
        else if (command == "ADD_POST"){
            try {
                string Username, content;
                s >> Username;
                transform(Username.begin(), Username.end(), Username.begin(), ::tolower);
                if (Username.empty()){
                    cout << "Error: Please enter a valid username." << endl;
                    continue;
                }
                
                if (F.Map.count(Username) == 0) {
                    cout << "This user does not exist, please enter a valid username." << endl;
                    continue;
                }
                getline(s, content); 
                if (content.empty()) {
                    cout << "Please enter valid content to post." << endl;              
                    continue;
                }
                if (!content.empty() && content[0] == ' ') {
                    content = content.substr(1);
                }
                F.addPost(Username, content);
                cout << "Added a post with required content to posts of " << Username << "." << endl;
            }
            catch (const out_of_range& e) {
                cout << "Error: user not found." << endl;
            }

        }
        else if (command == "OUTPUT_POSTS"){
            try{
                string Username, num;
                s >> Username;
                transform(Username.begin(), Username.end(), Username.begin(), ::tolower);
                if (Username.empty()){
                    cout << "Error: Please enter a valid username." << endl;
                    continue;
                }
                if (F.Map.count(Username) == 0) {
                    cout << "This user does not exist, please enter a valid username." << endl;
                    continue;
                }
                getline(s, num); 
                if (num.empty()) {
                    cout << "Please enter valid number." << endl;              
                    continue;
                }
                
                int n = stoi(num);
                F.outputPosts(Username, n);
            }
            catch (const out_of_range& e) {
                cout << "Error: user not found." << endl;
            }

        }
        
        else if (command == "EXIT") {
            cout << "Exiting system." << endl;
            break;   // exit the while loop

        }
        else {
            cout << "Error: Unknown command '" << command << "'." << endl;
        }

        cout << "> "; // prompt for the next command
    }
    return 0;
}


