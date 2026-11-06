# Social Network Management System

## Long Assignment 2 – Data Structures and Algorithms  
## Kashvi Khurana – 2024CS10559

This project implements a command-line social network platform in C++. It supports adding users, managing bidirectional friendships, posting and listing user content, suggesting new friends based on mutual connections, and calculating degrees of separation between users. The system uses AVL trees and custom heap structures for efficient management of user relationships and data.

---

## How to Use

### Compilation

A helper script is provided for building your project. To compile, simply run:

```sh
bash compile.sh
```

This will compile the project using C++20 and produce an executable named `test`.

### Running

To run the program:

```sh
./test
```


You will see a prompt (`>`) where you can enter commands.

### Supported Commands

- **ADD_USER <username>**  
  Creates a new user with the given username (case-insensitive).
- **ADD_FRIEND <username1> <username2>**  
  Creates a bidirectional friendship (edge) between the two users.
- **LIST_FRIENDS <username>**  
  Lists all friends of the user in alphabetical order.
- **SUGGEST_FRIENDS <username> <n>**  
  Suggests up to `n` users as friend recommendations, prioritizing those with the most mutual friends.
- **DEGREES_OF_SEPARATION <username1> <username2>**  
  Shows the shortest path (degrees of separation) between two users.
- **ADD_POST <username> <content>**  
  Adds a new post (with a timestamp) to the specified user's account.
- **OUTPUT_POSTS <username> <n>**  
  Displays the most recent `n` posts by the user (`-1` for all posts).
- **EXIT**  
  Exits the program.

---

### Example Session

```
ADD_USER alice
User 'alice' created.
ADD_USER bob
User 'bob' created.
ADD_FRIEND alice bob
Friendship created between alice and bob.
ADD_POST alice Hello, this is Alice!
Added a post with required content to posts of alice.
OUTPUT_POSTS alice 1
Hello, this is Alice!
Most recent 1 posts of alice printed.
SUGGEST_FRIENDS alice 2
There are less than 2 suggestable friends of alice, all of them have been listed.
DEGREES_OF_SEPARATION alice bob
The shortest path between alice and bob is 1.
EXIT
Exiting system.
```


---

## Design and Implementation Decisions

### 1. **Custom Data Structures**
- **`AVLposts`**: AVL tree for posts, ordered by timestamp
- **`AVLneighbours`**: AVL tree for friends, ordered lexicographically by username
- **`Heap`**: Custom min-heap for top-N friend suggestions

### 2. **User ID System**
- Each user assigned unique integer ID (starting from 0)
- IDs enable efficient vector-based indexing for BFS and mutual friend counting
- Global counter `total` in `SocialNet` class

### 3. **Case-insensitive Usernames**
- All usernames normalized to lowercase via `transform()` and `::tolower`
- Consistent storage and lookup

### 4. **Bidirectional Friendships**
- `addEdge()` creates two `userNode` pointers and inserts each user into the other's friend tree
- Returns `false` if friendship already exists

### 5. **Friend Suggestions (Two-Level BFS)**
- Level 1: Mark immediate friends as "already friends"
- Level 2: Count mutual connections for friends-of-friends
- Min-heap maintains top `n` candidates dynamically
- **Tiebreaker**: Lexicographically larger username preferred for equal mutual counts
- Stack reverses heap output for descending order display

### 6. **Degrees of Separation**
- BFS with in-order traversal helper (`BFShelper()`) to enqueue friends
- `vector<bool>` for visited tracking, `vector<int>` for distances
- Returns `-1` if no path exists

### 7. **Post Management**
- AVL tree ordered by timestamp (earlier → left, later → right)
- In-order traversal collects posts into vector, output in reverse (most recent first)
- Timestamps generated via `time(nullptr)`

### 8. **Memory Management**
- Destructors for `AVLposts`, `AVLneighbours`, and `SocialNet` recursively delete all nodes
- Prevents memory leaks

### 9. **Input Parsing with Spaces**
- `getline()` captures content/numbers after username
- Leading space trimmed via `content.substr(1)` for posts

### 10. **Error Handling**
- Empty input validation for usernames, content, and numbers
- User existence checks before all operations
- Duplicate user/friendship prevention
- Try-catch blocks for `out_of_range` exceptions
- Special case: `n=0` in `SUGGEST_FRIENDS` handled in main loop to avoid unnecessary processing

---

## Edge Case Handling

- **Duplicate Users**: `"Error: User '<username>' already exists."`
- **Existing Friendships**: `"Friendship already exists between..."`
- **Non-existent Users**: `"This user does not exist..."`
- **Empty Input**: Validation errors like `"Error: Please enter a valid username."`
- **Fewer than N Suggestions/Posts**: All available items listed with informative message
- **No Path Between Users**: `"There does not exist a path between..."`
- **User with No Friends**: `"<username> has no friends."`
- **Content with Spaces**: Fully supported
- **Unknown Commands**: `"Error: Unknown command '<command>'."`

---

## Notes

- Uses `unordered_map` for user lookup, but custom AVL trees and heap for core structures
- All tree operations O(log n) via AVL balancing
- Friend suggestion O(n log n) with min-heap
- BFS O(V + E) for degrees of separation
- Memory properly managed with destructors

---
