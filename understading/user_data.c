#include <stdio.h>
#include <string.h>

/*=========================================
    Structure
=========================================*/

typedef struct
{
  const char *id;
  const char *user_id;
  const char *content;

} Note;

/*=========================================
    Database (Temporary)
=========================================*/

const Note notes[] =
{
  {"n001", "u001", "This is the first note."},
  {"n002", "u002", "This is the second note."},
  {"n003", "u001", "This is another note for user u001."},
  {"n004", "u003", "This is a note for user u003."},
  {"n005", "u002", "This is another note for user u002."},
  {"n006", "u001", "This is yet another note for user u001."},
  {"n007", "u003", "This is a second note for user u003."},
  {"n008", "u002", "This is a third note for user u002."},
  {"n009", "u001", "This is a fourth note for user u001."},
  {"n010", "u003", "This is a third note for user u003."}
};

const int note_count = sizeof(notes) / sizeof(notes[0]);

/*=========================================
    Display One Note
=========================================*/

void display_note(const Note *note)
{
  printf("Note ID : %s\n", note->id);
  printf("User ID : %s\n", note->user_id);
  printf("Content : %s\n", note->content);
  printf("---------------------------------\n");
}

/*=========================================
    Display Notes By User
=========================================*/

int display_notes_by_user(
  const Note *notes,
  int note_count,
  const char *target_user_id)
{
  int found = 0;

  for (int i = 0; i < note_count; i++)
  {
    if (strcmp(notes[i].user_id, target_user_id) == 0)
    {
      display_note(&notes[i]);
      found++;
    }
  }

  return found;
}

/*=========================================
    Main
=========================================*/

int main(void)
{
  const char *target_user_id = "u003";

  printf("=================================\n");
  printf("Private Notes\n");
  printf("=================================\n\n");

  printf("Current User : %s\n\n", target_user_id);

  int total = display_notes_by_user(
    notes,
    note_count,
    target_user_id
  );

  if (total == 0)
  {
    printf("No notes found.\n");
  }
  else
  {
    printf("Total Notes : %d\n", total);
  }
  
  return 0;
}