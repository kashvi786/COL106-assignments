#include <string>
#include <iostream>
#include <vector>
#include <ctime>
#include <sstream>
#include <algorithm>
#include <stdexcept>

using namespace std;

template <typename A, typename B>
class HashEle{
private:
    A key;
    B value; 
    HashEle* next;

public:

    HashEle(A key, B value){
        this->key = key;
        this->value = value;
        this->next = nullptr;

    }
    A getKey(){
        return key;
    }

    B getVal(){
        return value;
    }

    void setVal(B value){
        this->value = value;
    }
    HashEle* getNext(){
        return next;
    }
    void setNext(HashEle* next){
        this->next = next;
    }


};

template <typename K>
struct KeyCode {

    unsigned long long operator()(const K& key) const {
        return reinterpret_cast<unsigned long long>(key) % 101;
    }
};
template <>
struct KeyCode<int> {
    unsigned long long operator()(const int& key) const {
        return key % 101;
    }
};

struct StringKeyCode {
    unsigned long long operator()(const string& key) const {
        unsigned long long hash = 5381; // some random prime
        for (char c : key) {
            hash = ((hash << 5) + hash) + c;
        }
        return hash % 101; 
    }
};




template <typename A, typename B, typename C = KeyCode<A>>
class HashMap{
private:
    int size = 101;
    HashEle<A,B> ** hashTable;
    C hashFunc;

public:
    HashMap(int size=101){
        this->size=size;
        hashTable = new HashEle<A,B> *[this->size]();

    }
    ~HashMap() {
        for (int i = 0; i < size; ++i) {
            HashEle<A,B> *entry = hashTable[i];
            while (entry != NULL) {
                HashEle<A,B> *prev = entry;
                entry = entry->getNext();
                delete prev;
            }
            hashTable[i] = NULL;
        }
        
        delete [] hashTable;
    }

    bool find(const A &key) {
        unsigned long long hashVal = hashFunc(key);
        HashEle<A,B> *temp = hashTable[hashVal];

        while (temp != nullptr) {
            if (temp->getKey() == key) {
                return true;
            }
            temp = temp->getNext();
        }
        return false;
    }

    B get(const A &key){
        unsigned long long hashVal = hashFunc(key);
        HashEle<A,B> *temp = hashTable[hashVal];

        while (temp != nullptr) {
            if (temp->getKey() == key) {
                return temp->getVal();
            }
            temp = temp->getNext();
        }
        throw out_of_range("Key not found in HashMap");

    }

    void insert(const A &key, const B &value){
        unsigned long long hashVal = hashFunc(key);
        HashEle<A,B> *last = nullptr;
        HashEle<A,B> *temp = hashTable[hashVal];

        while (temp != nullptr && temp->getKey() != key ) {
            last = temp;
            temp = temp->getNext();
        } 
        
        if(temp == nullptr){
            temp = new HashEle(key,value);
            if (last == nullptr){
                hashTable[hashVal] = temp;
            }
            else {
                last->setNext(temp);
            }
        }
        else{
            temp->setVal(value);
        }
        return;
    }

    void remove(const A &key){
        unsigned long long hashVal = hashFunc(key);
        HashEle<A,B> *last = nullptr;
        HashEle<A,B> *temp = hashTable[hashVal];

        while (temp != nullptr && temp->getKey() != key) {
            last = temp;
            temp = temp->getNext();
        }

        if (temp!=nullptr){
            if (last != nullptr){
                last->setNext(temp->getNext());
            }
            else {
                hashTable[hashVal] = temp->getNext();
            }
            delete temp;
        }
        return;
    }
};



class TreeNode{
private:
    int version_id;
    string content;
    string message;
    time_t created_timestamp;
    time_t snapshot_timestamp ;
    TreeNode* parent;
    vector <TreeNode*> children;

public:

    TreeNode(){

        version_id = 0;
        content = "";
        message = "";
        created_timestamp = time(nullptr);
        snapshot_timestamp = 0;
        parent = nullptr;
        vector <TreeNode*> a(0);
        children = a;

    }
    ~TreeNode() {
        for (TreeNode* child : children) {
            delete child;
        }
    }

    void setVersion(int k){
        version_id = k;
    }
    int getVersion(){
        return version_id;
    }
    void setContent(string cont){
        content = cont;
    }
    string getContent(){
        return content;
    }
    void setMessage(string mess){
        message = mess;
    }
    string getMessage(){
        return message;
    }
    time_t getCRTS(){
        return created_timestamp;
    }
    void snapShot(){
        snapshot_timestamp = time(nullptr);
    }
    time_t getSPTS(){
        return snapshot_timestamp;
    }
    void setParent(TreeNode* par){
        parent = par;
    }
    TreeNode* getParent(){
        return parent;
    }
    void addChild(TreeNode* child){
        children.push_back(child);
    }
    vector<TreeNode*> getChildren(){
        return children;
    }

};



class File{
private:

    TreeNode* root; 
    TreeNode* active_version;
    HashMap <int, TreeNode*> version_map;
    


public:
    int total_versions;
    
    File(){
        this->root = new TreeNode;
        this->active_version = root;
        this->total_versions = 1;
        this->version_map.HashMap<int, TreeNode *>::insert(0, this->root);
        this->root->setMessage("File created.");
        this->root->snapShot();

    }
    ~File() {
        delete root;

    }
    string read(){
        return active_version->getContent();

    }
    void insert(string cont){
        if (active_version->getSPTS() == 0){
            active_version->setContent(active_version->getContent()+ " " +cont);

        }
        else{
            TreeNode* temp = new TreeNode;
            temp->setVersion(total_versions);
            temp->setContent(active_version->getContent()+ " " +cont);
            temp->setMessage(active_version->getMessage());
            temp->setParent(active_version);
            active_version->addChild(temp);
            active_version = temp;
            version_map.insert(total_versions,active_version);
            total_versions++;
        }
        
    }
    void update(string cont){
        if (active_version->getSPTS() == 0){
            active_version->setContent(cont);

        }
        else{
            TreeNode* temp = new TreeNode;

            temp->setVersion(total_versions);
            temp->setContent(cont);
            temp->setMessage(active_version->getMessage());
            temp->setParent(active_version);
            active_version->addChild(temp);
            active_version = temp;
            version_map.insert(total_versions,active_version);
            total_versions++;
        }
        
    }
    void snap(string message){
        if (active_version->getSPTS() == 0){
            active_version->setMessage(message);
            active_version->snapShot();
            cout << "Marked the active version as snapshot." << endl;

        }
        else{
            cout << "It is already a snapshot." << endl;
        }
        
    }
    void rollBack(int vid = -1){
        if (vid == -1){
            if (active_version->getParent() != nullptr){
                active_version = active_version->getParent();
                cout << "Rolled back to parent." << endl;
            }
            else {
                cout << "Error: No parent exists." << endl;
            }

        }
        else if (vid < total_versions){
            active_version = version_map.HashMap<int, TreeNode *>::get(vid);
            cout << "Rolled back to required version ID." << endl;
        }
        else {
            cout << "Required version ID not found." << endl;
        }
    }
    void history(){
        TreeNode* temp = active_version;
        vector <TreeNode*> snapped;
        if (temp->getSPTS() != 0){
            snapped.push_back(temp);
            
        }
        temp = temp->getParent();

        while (temp != nullptr){
            snapped.push_back(temp);
            temp = temp->getParent();

            
        }
        reverse(snapped.begin(), snapped.end());
        int k = snapped.size();
        for (int i = 0; i<k; i++){
            time_t now = snapped[i]->getSPTS();
            cout << "Version ID: " << snapped[i]->getVersion() << ", Message: " << snapped[i]->getMessage() << ", Timestamp: " << ctime(&now);
        }
        
    }
    

};


struct RecentFilesCompare{
private:
    HashMap<File*, pair<unsigned long long, time_t>>& last_modified_map;
public:
    RecentFilesCompare(HashMap<File*, pair<unsigned long long, time_t>>& map) 
        : last_modified_map(map) {}

    bool operator()(File* a, File* b){
        pair<unsigned long long, time_t> val_a = this->last_modified_map.get(a);
        pair<unsigned long long, time_t> val_b = this->last_modified_map.get(b);
        
        return val_a.first > val_b.first;
    }
};

struct BiggestTreesCompare{
    bool operator()(File* a, File* b){
        return a->total_versions > b->total_versions;
    }
};


template <typename T, typename Comparator>
class Heap{
private:
    vector <T> H;
    Comparator compare;
    HashMap<T, int> fmap;
public:
    Heap(Comparator comp) : compare(comp) {}
    ~Heap() {}
    int parent(int i){
        return (i-1)/2;
    }
    int left(int i){
        return 2*i + 1;
    }
    int right(int i){
        return 2*i + 2;
    }

    void add(T t){
        H.push_back(t);
        int p = H.size()-1;
        fmap.insert(t,p);
        heapifyUp(p);
        
    }
    void heapifyUp(int i){
        while (i>0 && compare(H[i],H[parent(i)])){
            fmap.insert(H[i],parent(i));
            fmap.insert(H[parent(i)],i);
            swap(H[i], H[parent(i)]);
            i = parent(i);
        }

    }
    void heapifyDown(int i){
        int best = i;
        int l = left(i);
        int r = right(i);

        if (l < H.size() && compare(H[l],H[best])){
            best = l;
        }
        if (r < H.size() && compare(H[r],H[best])){
            best = r;
        }
        if (best != i){
            fmap.insert(H[i],best);
            fmap.insert(H[best],i);
            swap(H[i], H[best]);
            heapifyDown(best);
        }

    }
    T extract(){
        T temp = H[0]; 
        fmap.remove(temp);

        if (H.size() > 1) {
            H[0] = H.back();
            fmap.insert(H[0], 0); 
            H.pop_back(); 
            heapifyDown(0);

        }
        else {
            H.pop_back(); 
        }
        return temp;

    }
    void update(T t){
        int i = fmap.HashMap<File *, int>::get(t);
        heapifyUp(i);

    }

    vector <T> get_few(int nums){
        vector <T> result;
        T curr;
        for (int i = 0; i<nums; i++){
            curr = extract();
            result.push_back(curr);

        }
        for (int j = 0; j<nums; j++){
            add(result[j]);
        }
        return result;

    }


};



class FileSystem{
private:
    HashMap<File*, string> Names;
    HashMap<string, File*, StringKeyCode> Locate;
    Heap<File*, RecentFilesCompare> RF;
    //HashMap<File*, time_t> last_modified_map;
    Heap<File*, BiggestTreesCompare> BT;
    unsigned long long operation_counter;
    HashMap<File*, pair<unsigned long long, time_t>> last_modified_map;
    vector<File*> all_files;

public:

    FileSystem() : 
        last_modified_map(),
        RF(RecentFilesCompare(last_modified_map)),
        BT(BiggestTreesCompare())
    {

    }

    ~FileSystem() {
        for (size_t i = 0; i < all_files.size(); ++i) {
            delete all_files[i];
        }
    }


    bool CREATE(string name){
        if (Locate.find(name)) { 
            return false; 
        }

        File* temp = new File();
        all_files.push_back(temp);
        Names.insert(temp, name);
        Locate.insert(name,temp);
        operation_counter++;
        last_modified_map.insert(temp, {operation_counter, time(nullptr)});
        RF.add(temp);
        BT.add(temp);
        return true;


    }
    void READ(string name){
        File* curr = Locate.get(name);
        cout << curr->read() << endl;
    }
    void INSERT(string name, string content){
        File* curr = Locate.HashMap<std::string, File*, StringKeyCode>::get(name);
        curr->insert(content);
        //last_modified_map.insert(curr, time(nullptr));
        operation_counter++;
        last_modified_map.insert(curr, {operation_counter, time(nullptr)});
        BT.Heap<File *, BiggestTreesCompare>::update(curr);
        RF.update(curr);
    }
    void UPDATE(string name, string content){
        File* curr = Locate.get(name);
        curr->update(content);
        //last_modified_map.insert(curr, time(nullptr));
        operation_counter++;
        last_modified_map.insert(curr, {operation_counter, time(nullptr)});
        BT.Heap<File *, BiggestTreesCompare>::update(curr);
        RF.update(curr);

    }
    void SNAPSHOT(string name, string message){
        File* curr = Locate.get(name);
        curr->snap(message);

    }
    void ROLLBACK(string name, int vid){
        File* curr = Locate.get(name);
        curr->rollBack(vid);

    }
    void HISTORY(string name){
        File* curr = Locate.get(name);
        curr->history();

    }

    void RECENT_FILES(int num){
        if (num > all_files.size()) {
            cout << "Only " << all_files.size() << " files exist. Displaying all of them." << endl;
            num = all_files.size();
        }
        vector <File*> recent = RF.Heap<File*, RecentFilesCompare>::get_few(num);
        for (int i = 0; i < num; i++){
            cout << Names.HashMap<File *, std::string>::get(recent[i]) << " ";
        }
        cout << endl;
    }
    void BIGGEST_TREES(int num){
        if (num > all_files.size()) {
            cout << "Only " << all_files.size() << " files exist. Displaying all of them." << endl;
            num = all_files.size();
        }
        vector <File*> biggest = BT.Heap<File *, BiggestTreesCompare>::get_few(num);
        for (int i = 0; i < num; i++){
            cout << Names.HashMap<File *, std::string>::get(biggest[i]) << " ";
        }
        cout << endl;
    }
};

int main(){
    string line;
    cout << "> "; //prompt
    FileSystem W;

    while (getline(cin, line)) {
        stringstream s(line);
        string command;
        s >> command;

        if (command == "CREATE") {
            string filename;
            s >> filename;
            if (filename.empty()){
                cout << "Error: Please enter a valid filename." << endl;
                continue;
            }
            else if (W.CREATE(filename)) {
                cout << "File '" << filename << "' created." << endl;
            } else {
                cout << "Error: File '" << filename << "' already exists." << endl;
            }
        }
        else if (command == "READ") {
            try{
                string filename;
                s >> filename;
                if (filename.empty()){
                    cout << "Error: Please enter a valid filename." << endl;
                    continue;
                }
                W.READ(filename);
            }
            catch (const out_of_range& e) {
                cout << "Error: File not found." << endl;
            }

        }
        else if (command == "INSERT") {
            try{
                string filename, content;
                s >> filename;
                if (filename.empty()){
                    cout << "Error: Please enter a valid filename." << endl;
                    continue;
                }
                getline(s, content); 
                if (content.empty()) {
                    cout << "Please enter valid content to insert." << endl;              
                    continue;
                }
                if (!content.empty() && content[0] == ' ') {
                    content = content.substr(1);
                }
                W.INSERT(filename, content);
                cout << "Inserted content into '" << filename << "'." << endl;
            }
            catch (const out_of_range& e) {
                cout << "Error: File not found." << endl;
            }

        }
        else if (command == "UPDATE"){
            try{
                string filename, content;
                s >> filename;
                if (filename.empty()){
                    cout << "Error: Please enter a valid filename." << endl;
                    continue;
                }
                getline(s, content); 
                if (content.empty()) {
                    cout << "Please enter valid content to update." << endl;              
                    continue;
                }
                // Remove leading space from content
                if (!content.empty() && content[0] == ' ') {
                    content = content.substr(1);
                }
                W.UPDATE(filename, content);
                cout << "Updated content of '" << filename << "'." << endl;
            }
            catch (const out_of_range& e) {
                cout << "Error: File not found." << endl;
            }

        }
        else if (command == "SNAPSHOT"){
            try{
                string filename, message;
                s >> filename;
                if (filename.empty()){
                    cout << "Error: Please enter a valid filename." << endl;
                    continue;
                }
                getline(s, message); 
                if (message.empty()) {
                    message = "Please enter valid message.";    
                    continue;          
                }
                if (!message.empty() && message[0] == ' ') {
                    message = message.substr(1);
                }
                W.SNAPSHOT(filename, message);
            }
            catch (const out_of_range& e) {
                cout << "Error: File not found." << endl;
            }

        }
        else if (command == "ROLLBACK"){
            try {
                string filename, vid_string;
                s >> filename;
                s >> vid_string;
                if (filename.empty()){
                    cout << "Error: Please enter a valid filename." << endl;
                    continue;
                }
                if (vid_string.empty()){
                    W.ROLLBACK(filename, -1);    
                }
                else {
                    int vid = stoi(vid_string);
                    W.ROLLBACK(filename, vid);

                }
            }
            catch (const out_of_range& e) {
                cout << "Error: File not found." << endl;
            }

        }
        else if (command == "HISTORY"){
            try{
                string filename;
                s >> filename;
                if (filename.empty()){
                    cout << "Error: Please enter a valid filename." << endl;
                    continue;
                }
                W.HISTORY(filename);
            }
            catch (const out_of_range& e) {
                cout << "Error: File not found." << endl;
            }

        }
        else if (command == "RECENT_FILES"){
            try{
                string num_string;
                s >> num_string;
                if (num_string.empty()){
                    cout << "Error: Please enter a valid number." << endl;
                    continue;
                }
                int num = stoi(num_string);
                if (num <= 0) {
                    cout << "Error: Number must be positive." << endl;
                    continue;
                }
                W.RECENT_FILES(num);
            }
            catch (const std::invalid_argument& e) {
                cout << "Error: Invalid number provided." << endl;
            }
        }
        else if (command == "BIGGEST_TREES"){
            try{
                string num_string;
                s >> num_string;
                if (num_string.empty()){
                    cout << "Error: Please enter a valid number." << endl;
                    continue;
                }
                int num = stoi(num_string);
                if (num <= 0) {
                    cout << "Error: Number must be positive." << endl;
                    continue;
                }
                W.BIGGEST_TREES(num);
            }
            catch (const std::invalid_argument& e) {
                cout << "Error: Invalid number provided." << endl;
            }

        }
        else if (command == "EXIT") {
            cout << "Exiting system." << endl;
            break; // Exit the while loop

        }
        else {
            cout << "Error: Unknown command '" << command << "'." << endl;
        }

        cout << "> "; // Prompt for the next command
    }
    return 0;
}