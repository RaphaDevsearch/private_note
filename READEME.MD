# My Private Notes — a small step-by-step CGI project

Each logged-in user can see and add notes, but only ever **their own**
notes — never anyone else's. That's the whole point of this project:
showing *why* login matters, not just how to build a login form.

Right now this is a **learning version**: all data lives in C arrays
inside the program, not in files. We'll swap that for real file storage
in a later step.

---

## Files

```
private_notes/
├── lib.h            declarations — the "table of contents"
├── lib.c            all the actual function code + test data arrays
└── private_note.c   main() — ties everything together
```

- `lib.h` declares every function and the two data structures (`User`,
  `Note`), so `private_note.c` knows what's available without needing
  to see how it's implemented.
- `lib.c` implements every function, and holds the two test arrays.
- `private_note.c` is the entry point: reads the form data, checks the
  login, then adds a note and/or shows the user's own notes.

---

## The data (for now)

```c
User users[] = {
  {"u001", "raphael", "1234", "Raphael"},
  {"u002", "john",    "abcd", "John"},
  {"u003", "mary",    "pass", "Mary"}
};

Note notes[] = {
  {"n001", "u001", "I love Linux."},
  {"n002", "u002", "Apache is powerful."},
  {"n003", "u001", "Today I learned CGI."},
  {"n004", "u003", "Buy some books."},
  {"n005", "u001", "Finish my project."}
};
```

Notice `Note` links to `User` through `user_id` (e.g. `"u001"`), not
through the username. That's why the program needs a small lookup step
(`find_user_id()`) after login — to translate "raphael" into "u001"
before searching notes.

---

## What each function does

| Function | Job |
|---|---|
| `get_content_length()` | reads how many bytes the browser sent |
| `get_post_data()` | reads that many bytes safely into a buffer |
| `url_decode()` | turns `Buy+milk` back into `Buy milk` |
| `extract_field()` | pulls one field's value (e.g. `username`) out of the decoded data |
| `authenticate()` | checks username + password against the `users[]` array |
| `find_user_id()` | turns a valid username into its internal id (`"u001"`) |
| `add_note()` | appends a note to the `notes[]` array, tagged with the user's id |
| `view_notes()` | prints only the notes whose id matches the current user — the actual security boundary |

---

## An important limitation, on purpose

Because the data lives in a plain C array instead of a file, **nothing
you add is actually saved**. Every CGI request starts a brand new
program from scratch — the array resets to the test data every single
time. `add_note()` only affects that one request's response; the next
request starts fresh again.

This is fine for learning and testing the *logic* — but it means this
version can't be used as a real notes app yet. The natural next step
is swapping the array for a `notes.csv` file (the same pattern used in
the login project), so notes actually persist between requests.

---

## How to build and test it

**Compile:**
```bash
gcc -o private_note.cgi lib.c private_note.c
```

**Test directly, no server needed:**
```bash
export CONTENT_LENGTH=44
echo -n "username=raphael&password=1234&action=view" | ./private_note.cgi
```

You should see only Raphael's 3 notes — never John's or Mary's.