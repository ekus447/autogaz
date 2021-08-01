import gi
gi.require_version('Gtk', '3.0')
from gi.repository import Gtk, GdkPixbuf, Gdk
import playsound
from sys import platform
if platform == "linux" or platform == "linux2":
    print("linux")
else:
    print("nu onli linux")
    exit(0)

class Image(Gtk.Window):
    def __init__(self):
        
        Gtk.Window.__init__(self)
        arrow = Gdk.Cursor(Gdk.CursorType.ARROW)
        gdk_window = self.get_root_window()
        gdk_window.set_cursor(arrow)

        self.set_title("AutoGaz - Linux Edition")
        self.set_default_icon_from_file("./assets/icon.ico")
        self.dim_width = 640
        self.dim_height = 480
        self.set_resizable(False)
        self.set_position(Gtk.WindowPosition.CENTER)
        self.resize(self.dim_width, self.dim_height)
        self.connect("destroy", Gtk.main_quit)   
        self.state = 0  

        self.eventbox = Gtk.EventBox()        
        self.eventbox.connect("button-press-event", self.on_event_press)
        self.add(self.eventbox)        

        self.before = GdkPixbuf.Pixbuf.new_from_file('./assets/before.png')
        self.before = self.before.scale_simple(self.dim_width, self.dim_height * 0.9 + 50, GdkPixbuf.InterpType.BILINEAR)
        
        self.after = GdkPixbuf.Pixbuf.new_from_file('./assets/after.png')
        self.after = self.after.scale_simple(self.dim_width, self.dim_height * 0.9 + 50, GdkPixbuf.InterpType.BILINEAR)
        
        self.polat = GdkPixbuf.Pixbuf.new_from_file('./assets/polat.png')
        self.polat = self.polat.scale_simple(self.dim_width, self.dim_height * 0.9 + 50, GdkPixbuf.InterpType.BILINEAR)

        self.img = Gtk.Image.new_from_pixbuf(self.before)
              
        self.eventbox.add(self.img)

    def on_event_press(self, widget, event):
        if self.state == 0:
            watch = Gdk.Cursor(Gdk.CursorType.WATCH)
            gdk_window = self.get_root_window()
            gdk_window.set_cursor(watch)
            self.state = 1
            self.img.set_from_pixbuf(self.after)
            playsound.playsound('./assets/sound.wav', True)
            self.img.set_from_pixbuf(self.polat)
            arrow = Gdk.Cursor(Gdk.CursorType.ARROW)
            gdk_window = self.get_root_window()
            gdk_window.set_cursor(arrow)
            self.state = 2
        elif self.state == 2:
            arrow = Gdk.Cursor(Gdk.CursorType.ARROW)
            gdk_window = self.get_root_window()
            gdk_window.set_cursor(arrow)
            self.img.set_from_pixbuf(self.before)
            self.state = 0

window = Image()
window.show_all()

Gtk.main()
