#!/usr/bin/env python

import gtk
import gtk.glade

import solver

class SolverGui:
    def __init__(self, specFileName="solver-frontend.glade"):
	self.specFileName = specFileName
	self.widgetTree = None

    def Start(self):
	self.Show()
	gtk.mainloop()

    def Show(self):
	self.widgetTree = gtk.glade.XML(self.specFileName)
	dic = { "on_window1_destroy" : self.Destroy,
		"on_question_changed" : self.Solve }
	self.widgetTree.signal_autoconnect(dic)

    def Destroy(self, obj):
	gtk.mainquit()

    def Solve(self, obj):
	question = self.widgetTree.get_widget('question').get_text()

	if not question.isdigit():
	    equation = "Question contains non-digits"
	else:
	    equation = solver.solve(question)
	    if equation == None:
		equation = "No solution found"

	self.widgetTree.get_widget('equation').set_text(equation)

	return True

myApp = SolverGui()
myApp.Start()
