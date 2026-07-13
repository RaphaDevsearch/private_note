#ifndef LIB_H
#define LIB_H

/* ---------- Standard Libraries ---------- */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ---------- Constants ---------- */

#define MAX_LINE       1024
#define MAX_ID         50
#define MAX_PASSWORD   50
#define MAX_NOTE       500

/* ---------- Structures ---------- */

typedef struct User
{
  char id[MAX_ID];
  char password[MAX_PASSWORD];

} User;

typedef struct Note
{
    int id;
    char user_id[MAX_ID];
    char note[MAX_NOTE];

} Note;

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