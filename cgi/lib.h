#ifndef LIB_H
#define LIB_H

/* ---------- Standard Libraries ---------- */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* ---------- Constants ---------- */

#define MAX_LINE       1024
#define MAX_ID         20
#define MAX_USERNAME   50
#define MAX_PASSWORD   50
#define MAX_NAME       100
#define MAX_NOTE       500

#define MAX_USERS      50   /* room to grow the users[] array later */
#define MAX_NOTES      100  /* room to grow the notes[] array later */

/* ---------- Structures ---------- */

typedef struct
{
  char id[MAX_ID];
  char user_id[MAX_USERNAME];
  char password[MAX_PASSWORD];
  char name[MAX_NAME];

} User;

typedef struct
{
  char id[MAX_ID];
  char user_id[MAX_ID];
  char note[MAX_NOTE];

} Note;

/* ---------- Arrays ---------- */

extern User users[MAX_USERS];
extern int users_count;

extern Note notes[MAX_NOTES];
extern int notes_count;

/* ---------- HTML ---------- */

void html_header(void);
void html_footer(void);

/* ---------- Form ---------- */

void get_post_data(char *buffer, int size);
void url_decode(char *src, char *dest);

/* NEW — needed so main() can pull "username", "password", "note", "action"
   out of the one big decoded string. Belongs in the Form section. */
void extract_field(const char *data, const char *field_name, char *result);

/* ---------- Authentication ---------- */

int authenticate(const char *user_id, const char *password);

/* After authentication confirms the user_id/password, main() still needs the
   user's internal id (e.g. "u001") to tag/filter notes with. */
const char *find_user_id(const char *user_id);

/* ---------- Notes ---------- */

int add_note(const char *user_id, const char *note);

void view_notes(const char *user_id);

/**
 * understand how web work
 */
void print_request_info(void);
void read_post_data(char *buffer, int size);
int get_content_length(void);
void print_post_data(char *buffer);
void understanding(void);

#endif