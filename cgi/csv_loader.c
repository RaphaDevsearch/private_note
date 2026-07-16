#include "lib.h"

/*
   Function: load_users
   Reads users.csv (format: id,user_id,password,name), skips the
   header line, and returns a freshly allocated array of User.
   The number of users loaded is written into *count_out.

   Returns NULL if the file couldn't be opened.
   IMPORTANT: the caller is responsible for calling free() on the
   returned pointer once done with it — this memory doesn't clean
   itself up automatically like a normal array does.
*/
User *load_users(const char *filename, int *count_out)
{
  FILE *file = fopen(filename, "r");
  char line[MAX_LINE];
  int count = 0;
  int first_line = 1;

  if (file == NULL)
  {
    *count_out = 0;
    return NULL;
  }

  User *result = malloc(sizeof(User) * MAX_USERS);
  if (result == NULL)
  {
    fclose(file);
    *count_out = 0;
    return NULL; /* out of memory — extremely rare, but must be checked */
  }

  while (fgets(line, MAX_LINE, file) != NULL && count < MAX_USERS)
  {
    if (first_line) { first_line = 0; continue; }

    line[strcspn(line, "\r\n")] = '\0';
    if (line[0] == '\0') continue;

    char *id       = strtok(line, ",");
    char *user_id  = strtok(NULL, ",");
    char *password = strtok(NULL, ",");
    char *name     = strtok(NULL, ",");

    if (id && user_id && password && name)
    {
      strncpy(result[count].id, id, MAX_ID - 1);
      result[count].id[MAX_ID - 1] = '\0';

      strncpy(result[count].user_id, user_id, MAX_USERNAME - 1);
      result[count].user_id[MAX_USERNAME - 1] = '\0';

      strncpy(result[count].password, password, MAX_PASSWORD - 1);
      result[count].password[MAX_PASSWORD - 1] = '\0';

      strncpy(result[count].name, name, MAX_NAME - 1);
      result[count].name[MAX_NAME - 1] = '\0';

      count++;
    }
  }

  fclose(file);
  *count_out = count;
  return result;
}

/*
   Function: load_notes
   Same pattern as load_users, but for notes.csv
   (format: id,user_id,note).
   Caller must free() the returned pointer when done.
*/
Note *load_notes(const char *filename, int *count_out)
{
  FILE *file = fopen(filename, "r");
  char line[MAX_LINE];
  int count = 0;
  int first_line = 1;

  if (file == NULL)
  {
    *count_out = 0;
    return NULL;
  }

  Note *result = malloc(sizeof(Note) * MAX_NOTES);
  if (result == NULL)
  {
    fclose(file);
    *count_out = 0;
    return NULL;
  }

  while (fgets(line, MAX_LINE, file) != NULL && count < MAX_NOTES)
  {
    if (first_line) { first_line = 0; continue; }

    line[strcspn(line, "\r\n")] = '\0';
    if (line[0] == '\0') continue;

    char *id      = strtok(line, ",");
    char *user_id = strtok(NULL, ",");
    char *note    = strtok(NULL, ",");

    if (id && user_id && note)
    {
      strncpy(result[count].id, id, MAX_ID - 1);
      result[count].id[MAX_ID - 1] = '\0';

      strncpy(result[count].user_id, user_id, MAX_ID - 1);
      result[count].user_id[MAX_ID - 1] = '\0';

      strncpy(result[count].note, note, MAX_NOTE - 1);
      result[count].note[MAX_NOTE - 1] = '\0';

      count++;
    }
  }

  fclose(file);
  *count_out = count;
  return result;
}