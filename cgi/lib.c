#include "lib.h"

/* ---------- Users ---------- */

User users[] =
{
  {"u001", "raphael", "1234", "Raphael"},
  {"u002", "john",    "abcd", "John"},
  {"u003", "mary",    "pass", "Mary"}
};

int users_count = sizeof(users) / sizeof(users[0]);

/* ---------- Notes ---------- */

Note notes[] =
{
  {"n001", "u001", "I love Linux."},
  {"n002", "u002", "Apache is powerful."},
  {"n003", "u001", "Today I learned CGI."},
  {"n004", "u003", "Buy some books."},
  {"n005", "u001", "Finish my project."}
};

int notes_count = sizeof(notes) / sizeof(notes[0]);

void html_header(void)
{
  printf("Content-Type: text/html\n\n");
}

void html_footer(void)
{
}

void get_post_data(char *buffer, int size)
{
}

void url_decode(char *src, char *dest)
{
}

int authenticate(const char *id, const char *password)
{
  return 0;
}

int add_note(const char *user_id, const char *note)
{
  return 0;
}

void view_notes(const char *user_id)
{
}