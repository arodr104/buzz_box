struct noteNode {
  float duration;
  float note;
  bool silent;
  struct noteNode* next;
};

void addNote(noteNode* head, noteNode* tail, noteNode* node);

void clearNotes(noteNode* head);
