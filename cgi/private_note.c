#include "lib.h"

int main(void)
{
  char raw[MAX_LINE];
  char decoded[MAX_LINE];
  char username[MAX_USERNAME];
  char password[MAX_PASSWORD];
  char note[MAX_NOTE];
  char action[20];

  html_header();

  //understanding();

  get_post_data(raw, MAX_LINE);
  url_decode(raw, decoded);

  extract_field(decoded, "username", username);
  extract_field(decoded, "password", password);
  extract_field(decoded, "note",     note);
  extract_field(decoded, "action",   action);

  if (!authenticate(username, password))
  {
    printf("<p>Login failed. Check your username and password.</p>");
    html_footer();
    return 0;
  }

  const char *user_id = find_user_id(username);

  if (strcmp(action, "add") == 0)
  {
    if (add_note(user_id, note))
    {
      printf("<p>Note saved.</p>");
    }
    else
    {
      printf("<p>Could not save your note (no room left).</p>");
    }
  }

  view_notes(user_id);

  html_footer();

  return 0;
}