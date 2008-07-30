#!/usr/bin/env kross
import Parley

#FUNCTIONS

#moves selected entries to a new lesson
def moveSelectedToNewLesson():
  selected_entries = Parley.selectedEntries()
  if len(selected_entries) > 0:
    newlesson = Parley.doc.appendNewLesson("New Lesson")
    for entry in selected_entries:
      newlesson.appendEntry(entry)

#marks the selected translations as known (grade 7)
def markAsKnown():
  for tr in Parley.selectedTranslations():
    tr.grade = 7

#SCRIPT MENU

#create a new action for the Scripts menu (action1)
action1 = Parley.newAction("example_action1","Move to new lesson")
action1.statusTip="Moves the selected rows to a new lesson"
Parley.connect(action1,"triggered()",moveSelectedToNewLesson)


#create a new action for the Scripts menu (action2)
action2 = Parley.newAction("example_action2","Mark as known (highest grade)")
action2.statusTip="Sets the grade selected translations to 7 (highest grade)"
Parley.connect(action2,"triggered()",markAsKnown)
