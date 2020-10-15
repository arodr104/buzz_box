#include "NoteList.h"

void addNote(noteNode* head, noteNode* tail, noteNode* node)
{
	if (!head) // Empty list
	{
		head->next = node;
		tail->next = node;
	}
	
	else
	{
		tail->next = node;
		tail = tail->next;
	}
}

void clearNotes(noteNode* head)
{
	noteNode* curr = head->next;
	head->next = 0;
	
	while (!curr)
	{
		noteNode* tmp = curr->next;
		delete curr;
		curr = tmp;
	}
}
