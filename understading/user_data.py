"""
here I want to understand
how each user see their data basend on their acount
"""

# data
note = [
  {
    "id": 1,
    "title": "My first note",
    "content": "This is the content of my first note.",
    "user_id": 1
  },
  {
    "id": 2,
    "title": "My second note",
    "content": "This is the content of my second note.",
    "user_id": 1
  },
  {
    "id": 3,
    "title": "My third note",
    "content": "This is the content of my third note.",
    "user_id": 2
  }
]

def display_note_i(note_i):
  """
  Function to display a single note.
  """
  print(f"Note ID: {note_i['id']}")
  print(f"Title: {note_i['title']}")
  print(f"Content: {note_i['content']}")
  print(f"User ID: {note_i['user_id']}")
  print("-" * 20)

def display_notes(notes):
  """
  Function to display notes.
  """
  for note in notes:
    display_note_i(note)

def view_note_by_user(user_id):
  """
  Function to view notes based on user_id.
  """
  user_notes = [
    note_i 
    for note_i in note 
    if note_i["user_id"] == user_id
  ]
  return user_notes

data = view_note_by_user(2)
display_notes(data)