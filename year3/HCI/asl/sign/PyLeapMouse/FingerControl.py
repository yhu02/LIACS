# William Yager
# Leap Python mouse controller POC
# This file is for pointer-finger-based control (--finger and default)

import math
import sys
import time
from leap import Leap, Mouse


# The Listener that we attach to the controller. This listener is for pointer finger movement
class Finger_Control_Listener(Leap.Listener):
    def __init__(self, mouse, smooth_aggressiveness=8, smooth_falloff=1.3):
        # Initialize like a normal listener
        super(Finger_Control_Listener, self).__init__()

        self.blank_frame = True
        self.was_grabbing = False
        self.cursor = mouse.relative_cursor()

        self.track_hand = 1
        self.track_finger = Leap.Finger.TYPE_MIDDLE
        self.click_fingers = [Leap.Finger.TYPE_INDEX,
                              Leap.Finger.TYPE_RING]
        self.rclick_fingers = [Leap.Finger.TYPE_THUMB]

        self.base_scale = 10.0
        self.scale_fac = 20.0
        self.distance_cutoff = 0.15
        self.min_confidence = 0.0

        self.do_grab = False
        self.grab_cutoff = 0.85

        self.tracking = True
        self.track_change = 0
        self.track_min_delta = 1.0

        self.circling = False
        self.circle_finger = Leap.Finger.TYPE_INDEX
        self.circle_radius = 1.0
        self.circle_duration = 0.0

        self.scroll_last = 0.0
        self.scroll_delay = 0.2

        self.tap_finger = Leap.Finger.TYPE_PINKY

    def on_init(self, controller):
        print("Initialized")
        controller.config.set("Gesture.KeyTap.MinDistance", 4.0)
        controller.config.set("Gesture.KeyTap.MinDownVelocity", 1.0)

        controller.config.set("Gesture.ScreenTap.MinForwardVelocity", 30.0)
        controller.config.set("Gesture.ScreenTap.HistorySeconds", .1)
        controller.config.set("Gesture.ScreenTap.MinDistance", 1.0)

        controller.enable_gesture(Leap.Gesture.TYPE_KEY_TAP)
        controller.enable_gesture(Leap.Gesture.TYPE_CIRCLE)
        controller.enable_gesture(Leap.Gesture.TYPE_SCREEN_TAP)

        controller.config.save()

    def on_connect(self, controller):
        print("Connected")

    def on_disconnect(self, controller):
        print("Disconnected")

    def on_exit(self, controller):
        print("Exited")

    def on_frame(self, controller):
        frame = controller.frame()
        hand = frame.hands.rightmost

        if hand.confidence >= self.min_confidence:
            if self.do_grab \
               and hand.grab_strength >= self.grab_cutoff \
               and not self.was_grabbing \
               and hand.confidence > 0.50:
                self.grab_begin()
            elif self.do_grab \
                    and hand.grab_strength < self.grab_cutoff \
                    and self.was_grabbing:
                self.grab_end()

            self.do_gestures(frame)
            self.do_motion(frame.fingers)

    def do_gestures(self, frame):
        for g in frame.gestures():
                
            if g.is_valid and g.type == Leap.Gesture.TYPE_KEY_TAP:
                f = Leap.Finger(Leap.KeyTapGesture(g).pointable)
                print(("Did get key tap "
                      + "circling = " + str(self.circling) + ", "
                      + "tracking = " + str(self.tracking) + ", "
                      + "finger = " + str(f.type) + ", "))

            if not g.is_valid:
                continue
            elif g.type == Leap.Gesture.TYPE_KEY_TAP \
                    and not self.circling:
                kt = Leap.KeyTapGesture(g)
                if kt.pointable.is_finger:
                    f = Leap.Finger(kt.pointable)
                    if self.tracking and f.type in self.click_fingers:
                        self.cursor.click()
                        return
                    elif self.tracking and f.type in self.rclick_fingers:
                        self.cursor.rightClick()
                        return
            elif g.type == Leap.Gesture.TYPE_CIRCLE:
                fl = g.pointables
                extended = len(g.frame.fingers.extended())
                print((extended, fl[0].is_finger))
                if extended == 2 and fl[0].is_finger:
                    f = Leap.Finger(fl[0])
                    if f.type == self.circle_finger:
                        self.do_circle(Leap.CircleGesture(g), f)
                elif self.circling:
                    self.end_circle()

            elif g.type == Leap.Gesture.TYPE_SCREEN_TAP:
                t = Leap.ScreenTapGesture(g)
                if len(g.frame.fingers.extended()) == 1 \
                   and t.pointable.is_finger:
                    f = Leap.Finger(t.pointable)
                    if f.type == self.tap_finger:
                        self.toggle_tracking()

    def toggle_tracking(self):
        self.tracking = not self.tracking
        if self.tracking:
            self.circling = False
            self.blank_frame = True

    def grab_begin(self):
        self.was_grabbing = True
        self.cursor.click_down()
        print("GRAB START")

    def grab_end(self):
        self.was_grabbing = False
        self.cursor.click_up()
        print("GRAB END")

    def end_circle(self):
        self.scroll_last = 0.0
        self.circling = False
        self.blank_frame = True
        print("End Circle")

    def do_circle(self, c, f):
        if c.state == Leap.Gesture.STATE_STOP:
            self.end_circle()
            return

        if not self.tracking:
            return

        if c.duration >= self.circle_duration \
           and c.radius > self.circle_radius:

            if self.circling == False:
                print(("Start Circle " + ("-" if (c.normal[0] > 0.0) else "+")))

            self.circling = True

            if f.type == self.circle_finger:
                print(("Scroll " + ("UP" if c.normal[0] < 0.0 else "DOWN")))
                print((c.progress))
                if c.progress > (self.scroll_last + self.scroll_delay):
                    self.cursor.scroll(c.normal[0] > 0.0)
                    self.scroll_last = c.progress

    def calc_xy(self, finger):
        stabilizedPosition = finger.stabilized_tip_position
        distance = self.calc_distance(finger)
        scale = self.base_scale + (distance * self.scale_fac)
        sx = stabilizedPosition.x
        sy = 1 - stabilizedPosition.y

        return sx, sy, scale

    def calc_distance(self, finger):
        return (-finger.tip_position[2] + 100.0)/200.0

    def do_motion(self, fingers):
        fingers.extended()
        finger = fingers.finger_type(self.track_finger)[0]

        if not finger or not self.tracking:
            return

        #distance = finger.touch_distance
        distance = self.calc_distance(finger)

        if distance < self.distance_cutoff:
            self.blank_frame = True
        elif distance >= self.distance_cutoff and finger.touch_zone > 0:
            sx, sy, scale = self.calc_xy(finger)

            if self.circling:
                self.end_circle()

            if self.blank_frame:
                print(("Reacquire @") + str(sx) + "," + str(sy))
                self.cursor.setlast(sx, sy)
                self.blank_frame = False
            else:
                self.cursor.move(sx, sy, scale)
