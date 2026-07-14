#include "lib.h"

/* ---------- Test data (arrays instead of files, for now) ---------- */

User users[MAX_USERS] =
{
  {"u001", "raphael", "1234", "Raphael"},
  {"u002", "john",    "abcd", "John"},
  {"u003", "mary",    "pass", "Mary"}
};

int users_count = 3;

Note notes[MAX_NOTES] =
{
  {"n001", "u001", "I love Linux."},
  {"n002", "u002", "Apache is powerful."},
  {"n003", "u001", "Today I learned CGI."},
  {"n004", "u003", "Buy some books."},
  {"n005", "u001", "Finish my project."}
};

int notes_count = 5;

/* ---------- HTML ---------- */

void html_header(void)
{
  printf("Content-Type: text/html\n\n");
}

void html_footer(void)
{
}

/* ---------- Form: reading and decoding raw input ---------- */

int get_content_length(void)
{
  char *value = getenv("CONTENT_LENGTH");

  if (value == NULL)
  {
    return 0;
  }

  int length = atoi(value);

  if (length < 0)
  {
    return 0;
  }

  return length;
}

void get_post_data(char *buffer, int size)
{
  int length = get_content_length();

  if (length <= 0 || length >= size)
  {
    buffer[0] = '\0';
    return;
  }

  size_t bytes_read = fread(buffer, 1, (size_t)length, stdin);
  buffer[bytes_read] = '\0';
}

void url_decode(char *src, char *dest)
{
  char a, b;

  while (*src)
  {
    if (*src == '%' && (a = src[1]) && (b = src[2]) &&
        isxdigit((unsigned char)a) && isxdigit((unsigned char)b))
    {
      if (a >= 'a') a -= 'a' - 'A';
      if (a >= 'A') a -= ('A' - 10); else a -= '0';
      if (b >= 'a') b -= 'a' - 'A';
      if (b >= 'A') b -= ('A' - 10); else b -= '0';

      *dest++ = 16 * a + b;
      src += 3;
    }
    else if (*src == '+')
    {
      *dest++ = ' ';
      src++;
    }
    else
    {
      *dest++ = *src++;
    }
  }
  *dest = '\0';
}

/*
   Function: extract_field
   The decoded data still looks like one long string:
   "username=raphael&password=1234&note=Buy+milk&action=add"
   This pulls out the value for one field name, e.g. "username".
*/
void extract_field(const char *data, const char *field_name, char *result)
{
  char search_key[MAX_USERNAME];
  char raw_value[MAX_LINE];
  const char *start;
  const char *end;
  int len;

  result[0] = '\0';
  snprintf(search_key, sizeof(search_key), "%s=", field_name);

  start = strstr(data, search_key);
  if (start == NULL)
  {
    return; /* this field wasn't sent */
  }

  start += strlen(search_key);
  end = strchr(start, '&');
  if (end == NULL)
  {
    end = start + strlen(start);
  }

  len = end - start;
  if (len >= MAX_LINE)
  {
    len = MAX_LINE - 1;
  }

  strncpy(raw_value, start, len);
  raw_value[len] = '\0';

  url_decode(raw_value, result);
}

/* ---------- Authentication ---------- */

/*
   Function: authenticate
   Looks through the users[] array for a username + password
   that both match. Returns 1 if found, 0 otherwise.
*/
int authenticate(const char *username, const char *password)
{
  for (int i = 0; i < users_count; i++)
  {
    if (strcmp(users[i].username, username) == 0 &&
        strcmp(users[i].password, password) == 0)
    {
      return 1; /* match found */
    }
  }
  return 0; /* no match */
}

/*
   Function: find_user_id
   Once we know the username is valid, we need their internal
   id (like "u001") to tag and filter notes with — the note
   struct links to users by id, not by username.
*/
const char *find_user_id(const char *username)
{
  for (int i = 0; i < users_count; i++)
  {
    if (strcmp(users[i].username, username) == 0)
    {
      return users[i].id;
    }
  }
  return NULL; /* not found */
}

/* ---------- Notes ---------- */

/*
   Function: add_note
   Appends one note to the notes[] array, tagged with user_id.
   Returns 1 on success, 0 if there's no room left (array full).

   NOTE: this only changes the array in THIS program's memory.
   Since every CGI request starts a brand new process, the
   array resets to the test data above on the next request —
   nothing is actually saved permanently yet. That comes in a
   later step, once we write to a file instead of an array.
*/
int add_note(const char *user_id, const char *note)
{
  if (notes_count >= MAX_NOTES)
  {
    return 0; /* array is full */
  }

  snprintf(notes[notes_count].id, MAX_ID, "n%03d", notes_count + 1);
  strncpy(notes[notes_count].user_id, user_id, MAX_ID - 1);
  notes[notes_count].user_id[MAX_ID - 1] = '\0';
  strncpy(notes[notes_count].note, note, MAX_NOTE - 1);
  notes[notes_count].note[MAX_NOTE - 1] = '\0';

  notes_count++;
  return 1;
}

/*
   Function: view_notes
   Prints only the notes whose user_id matches — this is the
   actual security boundary: one user never sees another's notes.
*/
void view_notes(const char *user_id)
{
  int found_any = 0;

  printf("<h2>Your notes</h2><ul>");

  for (int i = 0; i < notes_count; i++)
  {
    if (strcmp(notes[i].user_id, user_id) == 0)
    {
      printf("<li>%s</li>", notes[i].note);
      found_any = 1;
    }
  }

  printf("</ul>");

  if (!found_any)
  {
    printf("<p>No notes yet.</p>");
  }
}

/* ---------- "understand how web works" — separate learning helpers ---------- */

void print_request_info(void)
{
  printf("<h3>Request info</h3><ul>");
  printf("<li>REQUEST_METHOD: %s</li>", getenv("REQUEST_METHOD") ? getenv("REQUEST_METHOD") : "(none)");
  printf("<li>CONTENT_LENGTH: %s</li>", getenv("CONTENT_LENGTH") ? getenv("CONTENT_LENGTH") : "(none)");
  printf("<li>CONTENT_TYPE: %s</li>", getenv("CONTENT_TYPE") ? getenv("CONTENT_TYPE") : "(none)");
  printf("<li>REMOTE_ADDR: %s</li>", getenv("REMOTE_ADDR") ? getenv("REMOTE_ADDR") : "(none)");
  printf("</ul>");
}

void read_post_data(char *buffer, int size)
{
  get_post_data(buffer, size);
}

void print_post_data(char *buffer)
{
  printf("<h3>Raw POST data</h3><p>%s</p>", buffer);
}

void understanding(void)
{
  char buffer[MAX_LINE];

  print_request_info();
  read_post_data(buffer, MAX_LINE);
  print_post_data(buffer);
}