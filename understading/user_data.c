#include <stdio.h>
#include <string.h>

typedef struct user_data{
  const char *user_id;
  const char *note_id;
  const char *note;
}user_data;

const user_data data[10] = {
  {"u001", "n001", "This is the first note."},
  {"u002", "n002", "This is the second note."},
  {"u001", "n003", "This is another note for user u001."},
  {"u003", "n004", "This is a note for user u003."},
  {"u002", "n005", "This is another note for user u002."},
  {"u001", "n006", "This is yet another note for user u001."},
  {"u003", "n007", "This is a second note for user u003."},
  {"u002", "n008", "This is a third note for user u002."},
  {"u001", "n009", "This is a fourth note for user u001."},
  {"u003", "n010", "This is a third note for user u003."}
};

void display_user_data(const user_data *data){
  printf("\tUser ID: %s\n", data->user_id);
  printf("\tNote ID: %s\n", data->note_id);
  printf("\tNote: %s\n", data->note);
  printf("--------------------------\n");
}

void display_all_user_data(const user_data *data, int size, const char *target_user_id){
  for(int i = 0; i < size; i++){
    if(strcmp(data[i].user_id, target_user_id) == 0){
      display_user_data(&data[i]);
    }
  }
}

int main() {
  // load data

  const char *target_user_id = "u003";

  printf("Displaying notes for user: %s\n", target_user_id);
  display_all_user_data(data, 10, target_user_id);

  

  return 0;
}