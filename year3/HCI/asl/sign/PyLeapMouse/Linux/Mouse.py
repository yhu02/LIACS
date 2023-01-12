from pymouse import PyMouse
from pykeyboard import PyKeyboard

mouse = PyMouse()
keyboard = PyKeyboard()

def AbsoluteMouseMove(posx, posy):
    # print 'move to', posx, posy
    mouse.move(int(posx), int(posy))


def AbsoluteMouseClick(posx, posy):
    print(('click on '), int(posx), int(posy))
    mouse.click(int(posx), int(posy))


def AbsoluteMouseClickDown(posx, posy):
    print('left button down')
    mouse.press(int(posx), int(posy))


def AbsoluteMouseClickUp(posx, posy):
    # print 'left button up'
    mouse.release(int(posx), int(posy))


def AbsoluteMouseDrag(posx, posy):  # Only relevant in OS X(?)
    mouse.move(int(posx), int(posy))


def AbsoluteMouseRightClick(posx, posy):
    print("right-click on", int(posx), int(posy))
    mouse.click(int(posx), int(posy), button=2)


# PyUserInput doesn't appear to support relative scrolling
def AbsoluteMouseScroll(posx, posy, up=True):
    if up is True:
        mouse.click(int(posx), int(posy), button=4)
    elif up is False:
        mouse.click(int(posx), int(posy), button=5)

def GetDisplayWidth():
    return mouse.screen_size()[0]


def GetDisplayHeight():
    return mouse.screen_size()[1]


# A cursor that does commands based on absolute position (good for finger pointing)
class absolute_cursor(object):
    def __init__(self):
        self.x_max = GetDisplayWidth() - 1
        self.y_max = GetDisplayHeight() - 1
        self.left_button_pressed = False
        self.x = 0
        self.y = 0

    def move(self, posx, posy):  # Move to coordinates
        self.x = posx
        self.y = posy
        if self.x > self.x_max:
            self.x = self.x_max
        if self.y > self.y_max:
            self.y = self.y_max
        if self.x < 0.0:
            self.x = 0.0
        if self.y < 0.0:
            self.y = 0.0
        if self.left_button_pressed:  # We are dragging
            AbsoluteMouseDrag(self.x, self.y)
        else:  # We are not dragging
            AbsoluteMouseMove(self.x, self.y)

    def click(self, posx=None, posy=None):  # Click at coordinates (current coordinates by default)
        if posx == None:
            posx = self.x
        if posy == None:
            posy = self.y
        AbsoluteMouseClick(posx, posy)

    # Set the state of the left button
    def set_left_button_pressed(self, boolean_button):
        if boolean_button == True:  # Pressed
            self.click_down()
        else:  # Not pressed
            self.click_up()

    def click_down(self, posx=None, posy=None):
        if posx == None:
            posx = self.x
        if posy == None:
            posy = self.y
        AbsoluteMouseClickDown(posx, posy)
        self.left_button_pressed = True

    def click_up(self, posx=None, posy=None):
        if posx == None:
            posx = self.x
        if posy == None:
            posy = self.y
        AbsoluteMouseClickUp(posx, posy)
        self.left_button_pressed = False

    def rightClick(self, posx=None, posy=None):
        if posx == None:
            posx = self.x
        if posy == None:
            posy = self.y
        AbsoluteMouseRightClick(posx, posy)

    def scroll(self, x_movement, y_movement):
        posx = self.x
        posy = self.y
        up = False
        if y_movement < 0:
            up = True
        AbsoluteMouseScroll(posx, posy, up)


# Allows for relative movement instead of absolute movement. This implementation is not a "true" relative mouse,
# but is really just a relative wrapper for an absolute mouse. Not the best way to do it, but I need to
# figure out how to send raw "mouse moved _this amount_" events. This class is (as of writing) untested.
# It's only here in case someone else wants to figure out how to do this properly on OS X.
# I will be "actually" implementing this on Windows shortly. OSX TBD.
class relative_cursor(absolute_cursor):
    def __init__(self):
        absolute_cursor.__init__(self)
        pos = mouse.position()
        self.x = pos[0]
        self.y = pos[1]
        self.abs_x = pos[0]
        self.abs_y = pos[1]

    def scroll(self, up):
        AbsoluteMouseScroll(self.x, self.y, up)

    def setlast(self, posx, posy):
        self.abs_x = posx
        self.abs_y = posy

    def move(self, posx, posy, scale=1.0):
        if scale < 0.0:
            scale = -scale
            dx = (posx - self.abs_x) / scale
            dy = (posy - self.abs_y) / scale
        else:
            dx = (posx - self.abs_x) * scale
            dy = (posy - self.abs_y) * scale
        # if dx > 10 or dy > 10:
        #    print str(dx) + "," + str(dy)
        self.x = self.x + dx
        self.y = self.y + dy
        if self.x > self.x_max:
            self.x = self.x_max
        if self.y > self.y_max:
            self.y = self.y_max
        if self.x < 0.0:
            self.x = 0.0
        if self.y < 0.0:
            self.y = 0.0
        if self.left_button_pressed:  # We are dragging
            AbsoluteMouseDrag(self.x, self.y)
        else:  # We are not dragging
            AbsoluteMouseMove(self.x, self.y)
        self.abs_x = posx
        self.abs_y = posy
