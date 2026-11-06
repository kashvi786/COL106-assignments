# File Versioning System

## Long Assignment 1 - Data Structures and Algorithms
## Kashvi Khurana - 2024CS10559

This project implements a command-line file versioning and snapshot system in C++. It supports creating files, inserting and updating content, taking snapshots, rolling back to previous versions, and querying for recent or largest files.

---

## How to Use

### Compilation

A helper script is provided. To compile, simply run:

```sh
bash compile.sh
```

This will compile the project using C++20 and produce an executable named `test`.

### Running

Run the program:

```sh
./test
```

You will see a prompt (`> `) where you can enter commands.

### Supported Commands

- **CREATE \<filename\>**
  - Creates a new file with the given name.
- **READ \<filename\>**
  - Prints the current content of the file.
- **INSERT \<filename\> \<content\>**
  - Appends content to the file (creates a new version if the current version is a snapshot).
- **UPDATE \<filename\> \<content\>**
  - Replaces the content of the file (creates a new version if the current version is a snapshot).
- **SNAPSHOT \<filename\> \<message\>**
  - Marks the current version as a snapshot with a message.
- **ROLLBACK \<filename\> [version_id]**
  - Rolls back to the parent version or to a specific version if `version_id` is provided.
- **HISTORY \<filename\>**
  - Shows the version history (snapshots) of the file.
- **RECENT_FILES \<num\>**
  - Lists the `num` most recently modified files. If `num` is greater than the number of files, a message is shown and all files are listed.
- **BIGGEST_TREES \<num\>**
  - Lists the `num` files with the most versions. If `num` is greater than the number of files, a message is shown and all files are listed.
- **EXIT**
  - Exits the program.

---

### Example Session

```
> CREATE myfile
File 'myfile' created.
> INSERT myfile Hello
Inserted content into 'myfile'.
> SNAPSHOT myfile First version
Marked the active version as snapshot.
> INSERT myfile World
Inserted content into 'myfile'.
> HISTORY myfile
Version ID: 0, Message: File created., Timestamp: ...
Version ID: 1, Message: First version, Timestamp: ...
> RECENT_FILES 1
myfile 
> EXIT
Exiting system.
```

---

## Design and Implementation Decisions

Below are the specific decisions made during implementation that are not explicitly specified in the assignment file:

### 1. **Custom Data Structures**
- Implemented custom `HashMap` and `Heap` classes instead of using STL containers, to meet assignment requirements and for learning purposes.
- Used open addressing with chaining for the hash map, and a binary heap for the heap.

### 2. **Hashing**
- For string keys, used the DJB2 hash function modulo 101 (a prime) for better distribution.
- For pointer keys, used reinterpret_cast to unsigned long and modulo 101.
- For integer keys, used modulo 101.

### 3. **File Content Handling**
- `INSERT` appends content to the current version if it is not a snapshot; otherwise, it creates a new version with appended content.
- `UPDATE` replaces the content of the current version if it is not a snapshot; otherwise, it creates a new version with the new content.

### 4. **Snapshots**
- A snapshot can only be marked on a version that is not already a snapshot.
- If a snapshot is attempted on an already snapshotted version, a message is shown.

### 5. **Rollback**
- If no version ID is provided, rollback moves to the parent version.
- If a version ID is provided, rollback moves to that version if it exists; otherwise, an error is shown.
- If rollback is attempted on the root version (no parent), an error is shown.

### 6. **Version History**
- The `HISTORY` command outputs **only the snapshotted versions** along the path from the root to the current active version.
- **If the active version is not snapshotted, it is not outputted** in the history.
- Each output includes the version ID, snapshot message, and timestamp.

### 7. **Recent and Biggest Files**
- `RECENT_FILES` and `BIGGEST_TREES` use custom heaps to efficiently retrieve the top N files.
- **If the requested number exceeds the number of files, a message is shown and only the available files are listed.**
- **Tiebreaker for RECENT_FILES:**  
  If two files have the same last modified timestamp, the file with the higher operation count (i.e., the one modified later in the sequence of operations) is considered more recent. This is implemented using an `operation_counter` that increments with each file modification and is stored alongside the timestamp.

### 8. **Content with Spaces**
- Both `INSERT` and `UPDATE` commands support content with spaces by reading the rest of the line after the filename.

### 9. **Error Handling**
- All commands that reference a file that does not exist print `Error: File not found.`
- Creating a file with an existing name prints an error and does not overwrite.
- All exceptions from hash map lookups are caught and handled gracefully.

### 10. **Memory Management**
- All dynamically allocated memory (files, tree nodes, hash elements) is deleted in destructors to prevent memory leaks.

### 11. **Timestamps**
- Timestamps are generated using `time(nullptr)` at the time of creation or modification.

### 12. **Prompt**
- The prompt (`> `) is shown after every command for a better user experience.

### 13. **Operation Counter**
- An operation counter is used to track the order of file modifications for the `RECENT_FILES` heap.

### 14. **File Naming**
- File names are case-sensitive and must be unique.

---

## Edge Case Handling

- **Duplicate File Creation:**  
  Attempting to create a file with a name that already exists will print an error and not overwrite the existing file.

- **Non-existent File Access:**  
  Any command that references a file that does not exist (e.g., `READ`, `INSERT`, `UPDATE`, `SNAPSHOT`, `ROLLBACK`, `HISTORY`) will print `Error: File not found.`

- **ROLLBACK Without Parent:**  
  If you try to roll back a file that has no parent version, an error message is shown.

- **ROLLBACK to Invalid Version:**  
  If you specify a version ID that does not exist, an error message is shown.

- **SNAPSHOT on Already Snapshotted Version:**  
  If you try to snapshot a version that is already a snapshot, a message is shown indicating this.

- **RECENT_FILES / BIGGEST_TREES with Too Large Number:**  
  If you request more files than exist, a message is shown and only the available files are listed.

- **Content with Spaces:**  
  Content for `INSERT` and `UPDATE` commands can include spaces.

- **Graceful Exit:**  
  The system can be exited at any time with the `EXIT` command.

---

## Notes

- All memory is managed safely; files are deleted when the system exits.
- The system uses custom hash maps and heaps for efficient file lookup and ordering.
- Timestamps are based on the system clock at the time