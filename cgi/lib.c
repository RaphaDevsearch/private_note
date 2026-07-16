#include "lib.h"

/* ---------- Test data (arrays instead of files, for now) ---------- */

User users[MAX_USERS] =
{
  {"u001", "24ug1234", "1234", "Raphael"},
  {"u002", "25jh5678", "abcd", "John"},
  {"u003", "26ma9012", "pass", "Mary"}
};

int users_count = 3;

Note notes[MAX_NOTES] =
{
  {"n001", "u001", "I love Linux."},
  {"n002", "u002", "Apache is powerful."},
  {"n003", "u001", "Today I learned CGI."},
  {"n004", "u003", "Buy some books."},
  {"n005", "u001", "Finish my project."},
  {"n006", "u001", "I need to buy milk."},
  {"n007", "u001", "I like manjaro."},
  {"n008", "u001", "I did coding."},
  {"n009", "u001", "I need to buy groceries."},
  {"n010", "u001", "I love programming."}
};

int notes_count = 10;

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
int authenticate(const char *user_id, const char *password)
{
  for (int i = 0; i < users_count; i++)
  {
    if (strcmp(users[i].user_id, user_id) == 0 &&
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
const char *find_user_id(const char *user_id)
{
  for (int i = 0; i < users_count; i++)
  {
    if (strcmp(users[i].user_id, user_id) == 0)
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
  int shown = 0;
 
  printf(
    "<meta charset=\"UTF-8\">"
    "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">"
    "<style>"
    ":root{--paper:#EDE7D6;--paper-2:#E2D9C1;--ink:#221D13;--ink-soft:#5B5540;"
    "--line:#B9AC85;--amber:#B96A1E;}"
    "@media (prefers-color-scheme: dark){:root{--paper:#15130C;--paper-2:#1C1810;"
    "--ink:#EDE3C6;--ink-soft:#B3A987;--line:#3A331F;--amber:#E7A23C;}}"
    "*{box-sizing:border-box;}"
    "html{-webkit-text-size-adjust:100%%;text-size-adjust:100%%;}"
    "body{background:var(--paper);color:var(--ink);"
    "font-family:'IBM Plex Sans',-apple-system,sans-serif;margin:0;"
    "padding:24px;padding-bottom:calc(24px + env(safe-area-inset-bottom));}"
    ".notes-wrap{max-width:480px;width:100%%;margin:0 auto;}"
    ".topbar{display:flex;align-items:center;justify-content:space-between;"
    "padding-bottom:14px;border-bottom:1px solid var(--line);margin-bottom:16px;}"
    ".brand{font-family:'JetBrains Mono',monospace;font-weight:700;font-size:0.95rem;"
    "color:var(--ink);}"
    ".brand span{color:var(--amber);}"
    ".logout-btn{font-family:'JetBrains Mono',monospace;font-size:0.78rem;"
    "background:transparent;border:1px solid var(--line);color:var(--ink-soft);"
    "padding:6px 10px;border-radius:3px;cursor:pointer;text-decoration:none;}"
    ".logout-btn:hover{border-color:var(--amber);color:var(--amber);}"
    ".notes-title{font-family:'JetBrains Mono',monospace;"
    "font-size:clamp(0.8rem,3.5vw,0.85rem);color:var(--ink-soft);margin-bottom:16px;}"
    ".notes-title b{color:var(--ink);font-weight:600;}"
    ".note-card{background:var(--paper-2);border:1px solid var(--line);"
    "border-left:3px solid var(--amber);border-radius:3px;padding:12px 14px;"
    "margin-bottom:10px;display:flex;gap:8px;align-items:flex-start;}"
    ".note-index{font-family:'JetBrains Mono',monospace;color:var(--amber);"
    "font-size:0.78rem;flex-shrink:0;padding-top:2px;}"
    ".note-text{font-size:clamp(0.95rem,3.8vw,1rem);color:var(--ink);"
    "line-height:1.5;word-break:break-word;overflow-wrap:anywhere;"
    "min-width:0;flex:1;}"
    ".empty-state{color:var(--ink-soft);font-size:0.9rem;padding:24px 0;"
    "text-align:center;border:1px dashed var(--line);border-radius:3px;}"
    "@media (max-width:480px){"
    "body{padding:16px;padding-bottom:calc(16px + env(safe-area-inset-bottom));}"
    ".note-card{padding:12px;}"
    "}"
    "@media (max-width:360px){"
    "body{padding:12px;}"
    ".note-card{padding:10px;gap:6px;}"
    "}"
    "</style>"
  );
 
  printf("<div class=\"notes-wrap\">");
 
  printf(
    "<div class=\"topbar\">"
    "<div class=\"brand\"><span>$</span> private_note</div>"
    "<a class=\"logout-btn\" href=\"../index.html\">log out</a>"
    "</div>"
  );
 
  for (int i = 0; i < notes_count; i++)
  {
    if (strcmp(notes[i].user_id, user_id) == 0)
    {
      shown++;
    }
  }
  printf("<div class=\"notes-title\"><b>%d</b> note%s</div>",
         shown, shown == 1 ? "" : "s");
 
  shown = 0; /* reset — this counter now drives the badge numbers below */
 
  for (int i = 0; i < notes_count; i++)
  {
    if (strcmp(notes[i].user_id, user_id) == 0)
    {
      shown++;
      printf(
        "<div class=\"note-card\">"
        "<span class=\"note-index\">[%02d]</span>"
        "<span class=\"note-text\">%s</span>"
        "</div>",
        shown, notes[i].note
      );
      found_any = 1;
    }
  }
 
  if (!found_any)
  {
    printf("<div class=\"empty-state\">no notes yet — add your first one above.</div>");
  }
 
  printf("</div>");
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