Sarah R. Sahibzada
CSCE 441-500
Heirarchical Insect Model

CONTROLS:
ZOOM MODE: Press 'z' to enable. Press the up arrow to zoom in and the down arrow to zoom out.
PAN MODE: Press 'p' to enable. Press the up arrow to look up, the down arrow to look down, and the left and right arrows to look left and right
ROTATE MODE: Press 'r' to enable. Press up to rotate around upwards, and down to rotate around downwards. 
			It will reset the model view matrix and projection matrix and then points the camera to where
			the last motion occurred, scales the image to its original size, but maintains transformations to the bug
			since none of the transformation matrices of the bug nor the values used to transform it are affected.
			Anything called after something in ROTATE mode will see these values reset, and ROTATE mode will reset the values.
			
MOTION MODES: 
	-Mouse Click: This bug is very excitable and wags its tail! This rapid motion is governed by
	mouse motion, and moves in the x- and y- directions as you sweep out with your mouse. Motion varies
	with the duration of time the mouse is held down as well as the direction (x or y motion), NOT relative
	motion (forwards/backwards).
	.
	- Slide Mode: press 's' to enable. Note that this can be intuitively justified: each key and the number 4 after it
		deal with the same bug segment.
		'1': Move bug segment closest to head away from viewer
		'5': Move bug segment closest to head towards viewer
		'2': Move second bug segment from head as well as all the things in front of it, away from the viewer
		'6': Move second bug segment from head as well as all the things in front of it, towards the viewer
		'3': Move third bug segment from head as well as all the things in front of it, away from the viewer
		'7': Move third bug segment from head as well as all the things in front of it, towards the viewer
		'4': Move fourth bug segment from head as well as all the things in front of it, away from the viewer
		'8': Move fourth bug segment from head as well as all the things in front of it, towards the viewer

	-Head mode: press 'h' to enable. 
		Up arrow moves the head up
		Down arrow moves the head down
		Right arrow moves the head away
		Left arrow moves the head towards viewer

	-Legs mode: press 'l' (L lowercase) to enable.
		Right arrow will cause continuous walking motion. Bug does not move.
HIDDEN SURFACE REMOVAL:
	This project did NOT utilize lighting, and all the shapes used were wire cubes. As such, you can find
	evidence that I put in the hidden surface removal (depth buffer and backface cull) calls from OpenGL
1	in the init() function and the fourth line of the display() function.

HEIRARCHY:

													Bug
			Body																		Head 
	Segment											Tail							(glutWireSphere()), glutWireSphere()
Joint,				Piece,			Legs			Joint, Piece
glutWireSphere()	glutWireCube()	GL_LINES		glutWireSphere, glutWirecube