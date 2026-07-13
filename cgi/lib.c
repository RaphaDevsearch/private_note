#include "lib.h"

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