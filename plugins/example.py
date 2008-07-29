#!/usr/bin/env kross
import Parley

#create a new action for the Scripts menu
action1 = Parley.newAction("example_action1","Move selected to new lesson")
Parley.connect(action1,"triggered()",moveSelectedToNewLesson)

#moves selected entries to a new lesson
def moveSelectedToNewLesson():
  selected_entries = Parley.selectedEntries()
  if len(selected_entries) > 0:
    newlesson = Parley.appendNewLesson("New Lesson")
    for entry in selected_entries:
      newlesson.appendEntry(entry)
      Parely.activeLesson.removeEntry(entry)
