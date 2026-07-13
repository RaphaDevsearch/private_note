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

/**
 * Here we try to understand how web work
 * 
 */
/*
    +---------------------+
    |      HTML Form      |
    +---------------------+
              |
              V
      username = raphael
      password = 1234
              |
              V
        Browser creates

    username=raphael&password=1234
              |
              V
           Apache
              |
      CONTENT_LENGTH = 31
              |
              V
            stdin
              |
              V
      private_note.cgi
              |
         fread(...)
              |
              V
            buffer
              |
         printf(...)
              |
              V
           Browser
*/

void print_request_info()
{
    printf("<h2>Request Information</h2>");

    printf("REQUEST_METHOD : %s<br>",
           getenv("REQUEST_METHOD"));

    printf("CONTENT_LENGTH : %s<br>",
           getenv("CONTENT_LENGTH"));

    printf("CONTENT_TYPE : %s<br>",
           getenv("CONTENT_TYPE"));

    printf("<hr>");
}