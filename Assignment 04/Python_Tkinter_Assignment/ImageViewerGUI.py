from captioner.model import ImageCaptioningModel
from captioner.model import ImageClassificationModel
from tkinter import *
from functools import partial
from PIL import ImageTk, Image
from tkinter import filedialog

def fileClick(clicked):
    # Define the function you want to call when the filebrowser button (Open) is clicked.
    # This function should pop-up a dialog for the user to select an input image file.
    # To have a better clarity, please check out the sample video.

    # Allowing only jpg type file selection 
	imgTypes = [('jpg Images','*.jpg')]
	#  File Dialog
	global imgName # will use this for checking a condition in process(), so declaring gloablly
	imgName = filedialog.askopenfilename(title = 'Select Image', initialdir = './data/imgs/', filetypes = imgTypes)
	if len(imgName) == 0 : #If no image is selected
		print("No Image has been selected !!!")
		return
	
	panelText.config(text = "")

	# deleting previously written text in Entry widget
	e.delete(0,END)
	# writing name of current image, Assuming images are numbered from 0-9(single digit)
	e.insert(0,"Image : " + imgName[-5])

	global img
	img = Image.open(imgName)
	width = img.width
	height = img.height + 50
	width += 200
	root.geometry(str(width)+'x'+str(height))

	imgTk = ImageTk.PhotoImage(img)
	panelImg.config(image = imgTk)
	panelImg.image = imgTk

def process(clicked, captioner, classifier):
    # This function will produce the required output when 'Process' button is clicked.
    # Note: This should handle the case if the user clicks on the `Process` button without selecting any image file.

    # Catch for No Image Selected
	try : 
		if len(imgName) == 0 :
			print("Choose an image first !!!")
	except : 
		print("Choose an image first !!!")
		return

	sstr = "Top-3 "
	maxlen = 0
	width = img.width
	height = img.height + 50
	mul = 6

	if clicked.get() == "Caption":
		captions = captioner(imgName)
		sstr += "Captions :\n\n"
		for caption in captions:
			if len(caption)> maxlen:
				maxlen = len(caption)
			sstr += caption
			sstr +='\n'
		
	else:
		classification = classifier(imgName)
		sstr += "Classes :\n"
		for caption in classification:
			line = caption[1]
			line += ' - ' + str(100*caption[0])[:7] + '%'
			if(len(line)>maxlen):
				maxlen = len(line)
			sstr += line
			sstr +='\n'
	
	width += max(175, int(mul*maxlen))
	root.geometry(str(width)+'x'+str(height))
	panelText.config(text = sstr)

if __name__ == '__main__':
    # Complete the main function preferably in this order:
    # Instantiate the root window.
    # Provide a title to the root window.
    # Instantiate the captioner, classifier models.
    # Declare the file browsing button.
    # Declare the drop-down button.
    # Declare the process button.
    # Declare the output label.

	# Instantiate the root window.
	# Provide a title to the root window.
	root = Tk()
	root.title("Harshith's Assignment Window")
	root.geometry('750x100')
	####### CODE REQUIRED (END) #######

	# Setting up the Caption model.
	transforms = []

	# Instantiate the captioner, classifier models.
	captioner = ImageCaptioningModel()
	classifier = ImageClassificationModel()

	# Declare the options.
	options = ["Caption", "Classify"]
	clicked = StringVar()
	clicked.set(options[0])

	e = Entry(root, width=70)
	e.grid(row=0, column=0)

	# Declare the file browsing button
	fileButton = Button(root, text = "Choose Image" , command = partial(fileClick, clicked))
	fileButton.grid(row = 0, column = 1)
	# Declaring panels for images
	panelImg = Label(root, image = "")
	panelImg.grid(row = 1, column = 0, columnspan = 2) # left panel
	panelText = Label(root, text = "")
	panelText.grid(row = 1, column = 2, columnspan= 2) 

	# Declare the drop-down button
	dropDown = OptionMenu(root, clicked, *options)
	dropDown.grid(row = 0, column = 2)

	# Declare the process button.
	myButton = Button(root, text="Process", padx = 10, command=partial(process, clicked, captioner, classifier))
	myButton.grid(row=0, column=3)

	# Execute with mainloop()
	root.mainloop()