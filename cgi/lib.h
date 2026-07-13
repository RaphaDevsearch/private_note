#ifndef LIB_H
#define LIB_H

/* ---------- Standard Libraries ---------- */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ---------- Constants ---------- */

#define MAX_LINE       1024
#define MAX_ID         20
#define MAX_USERNAME   50
#define MAX_PASSWORD   50
#define MAX_NAME       100
#define MAX_NOTE       500

/* ---------- Structures ---------- */

typedef struct
{
  char id[MAX_ID];
  char username[MAX_USERNAME];
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

extern User users[];
extern int users_count;

extern Note notes[];
extern int notes_count;

/* ---------- HTML ---------- */

void html_header(void);
void html_footer(void);

/* ---------- Form ---------- */

void get_post_data(char *buffer, int size);
void url_decode(char *src, char *dest);

/* ---------- Authentication ---------- */

int authenticate(const char *id, const char *password);

/* ---------- Notes ---------- */

int add_note(const char *user_id, const char *note);

void view_notes(const char *user_id);

#endif