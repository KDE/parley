#!/usr/bin/env kf5kross
# -*- coding: utf-8 -*-
import Parley
import Kross

T = Kross.module("kdetranslation")

#FUNCTIONS

#moves selected entries to a new lesson
def moveSelectedToNewLesson():
  selected_entries = Parley.selectedEntries()
  if len(selected_entries) > 0:
    newlesson = Parley.doc.appendNewLesson(T.i18n("New Lesson"))
    for entry in selected_entries:
      newlesson.appendEntry(entry)

#marks the selected translations as known (grade 7)
def markAsKnown():
  for tr in Parley.selectedTranslations():
    tr.grade = 7

#SCRIPT MENU

#create a new action for the Scripts menu (action1)
action1 = Parley.newAction("example_action1",T.i18n("Move to new lesson"))
action1.statusTip=T.i18n("Moves the selected vocabulary to a new lesson")
Parley.connect(action1,"triggered()",moveSelectedToNewLesson)


#create a new action for the Scripts menu (action2)
action2 = Parley.newAction("example_action2",T.i18n("Mark as known (highest grade)"))
action2.statusTip=T.i18n("Sets the grade for the selected words to 7 (highest grade)")
Parley.connect(action2,"triggered()",markAsKnown)
