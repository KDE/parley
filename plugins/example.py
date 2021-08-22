#!/usr/bin/env kf5kross
# -*- coding: utf-8 -*-
import Parley
import Kross

T = Kross.module("kdetranslation")

#FUNCTIONS

#marks the selected translations as known (grade 7)
def markAsKnown():
  for tr in Parley.selectedTranslations():
    tr.grade = 7

#SCRIPT MENU


#create a new action for the Scripts menu (action2)
action2 = Parley.newAction("example_action2",T.i18n("Mark as known (highest grade)"))
action2.statusTip=T.i18n("Sets the grade for the selected words to 7 (highest grade)")
Parley.connect(action2,"triggered()",markAsKnown)
