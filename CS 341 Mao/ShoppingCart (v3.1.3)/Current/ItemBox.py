
""" ItemBox.py

    Description:
        This is the Item/Product Box for the browsing page (HomePage).

    Team: Daniel Bartelson, Ian Neville, Brian Draheim, Kong Jimmy Vang
    Coder(s): Kong Jimmy Vang, Ian Neville, Daniel Bartelson
    Database: Brian Draheim
"""

import os
import sys
import sqlite3
import math


#######################################################
# Check the version of Python and import accordingly. #
#######################################################
if sys.version_info.major == 2:
    # Python 2.x
    from Tkinter import *
    import Tkinter as tk
    import Tkinter.messagebox
    import ttk
elif sys.version_info.major == 3:
    # Python 3.x
    from tkinter import *
    import tkinter as tk
    import tkinter.messagebox
    import tkinter.ttk as ttk
else:
    # Unsupported Version
    print("\nThis version of Python is not supported.")
    print("***EXITING SOFTWARE***")
    sys.exit()

from PIL import ImageTk, Image

###########################################################


class ItemBox(tk.Frame):
    """
    This is an ItemBox that will store products to be used for the browsing page.
    The functions are below.

    :param main_frame: Main Frame (Must be a MainFrame instance in 'ShoppingCart.py')
    :param top_frame: The Frame this will be attached to.
    :param relative_y: Relative Y position of this Tkinter Frame.
    :param pid: PID (Product ID)
    :param name: Name of the product.
    :param category: "New" or "Used" product.
    :param price: The Product's price.
    :param quantity: How much of this product is left.
    :param info: Information about the product/item.
    :param image: A path to the image that will be used.

    """

    def __init__(self,
                 main_frame=None,
                 top_frame=None,
                 relative_y=0,
                 pid=0,
                 name="",
                 category="",
                 price=0.00,
                 quantity_left=0,
                 info="",
                 image="",
                 *args, **kwargs):
        tk.Frame.__init__(self, *args, **kwargs)

        ### Attributes ###
        self.conn = sqlite3.connect("ShoppingCart.db")
        self.cursor = self.conn.cursor()

        font13 = "-family {Franklin Gothic Book} -size 11"
        font10 = "-family {Franklin Gothic Heavy} -size 12"
        font12 = "-family {Franklin Gothic Heavy} -size 11"
        font14 = "-family {Franklin Gothic Heavy} -size 13"
        font17 = "-family {Franklin Gothic Heavy} -size 15 -underline 1"
        font18 = "-family {Franklin Gothic Heavy} -size 15"
        font21 = "TkDefaultFont"
        font9 = "-family {Franklin Gothic Heavy} -size 14"

        self.main_frame = main_frame
        self.top_frame = top_frame
        self.relative_y = relative_y
        self.pid = pid
        self.name = name
        self.category = category
        self.price = price
        self.quantity_left = quantity_left
        self.info = info
        try:
            img = Image.open(image)
            img = img.resize((128, 128), Image.ANTIALIAS)
            self.image = ImageTk.PhotoImage(img)
        except:
            pass

        ### Widgets ###
        self.lblframe_item1 = tk.LabelFrame(self.top_frame)
        # self.lblframe_item1.place(relx=0.036, rely=0.063, relheight=0.276, relwidth=0.934)
        # self.lblframe_item1.place(relx=0.036, rely=self.relative_y, relheight=0.276, relwidth=0.923)
        self.lblframe_item1.place(relx=0.03, rely=relative_y, relheight=0.276, relwidth=0.935)
        self.lblframe_item1.configure(relief='groove')
        self.lblframe_item1.configure(font=font9)
        self.lblframe_item1.configure(foreground="black")
        self.lblframe_item1.configure(text=self.name)
        self.lblframe_item1.configure(background="#d9d9d9")
        self.lblframe_item1.configure(highlightbackground="#d9d9d9")
        self.lblframe_item1.configure(highlightcolor="black")
        self.lblframe_item1.configure(width=940)

        self.btn_item1_image = tk.Button(self.lblframe_item1)
        self.btn_item1_image.place(relx=0.021, rely=0.171, height=128, width=128, bordermode='ignore')
        self.btn_item1_image.configure(activebackground="#ececec")
        self.btn_item1_image.configure(activeforeground="#000000")
        self.btn_item1_image.configure(background="#d9d9d9")
        self.btn_item1_image.configure(disabledforeground="#a3a3a3")
        self.btn_item1_image.configure(foreground="#000000")
        self.btn_item1_image.configure(highlightbackground="#d9d9d9")
        self.btn_item1_image.configure(highlightcolor="black")
        self.btn_item1_image.configure(pady="0")
        self.btn_item1_image.configure(image=self.image, compound=CENTER)

        self.lbl_item1_price = tk.Label(self.lblframe_item1)
        self.lbl_item1_price.place(relx=0.819, rely=0.514, height=31, width=154
                                   , bordermode='ignore')
        self.lbl_item1_price.configure(activebackground="#f9f9f9")
        self.lbl_item1_price.configure(activeforeground="black")
        self.lbl_item1_price.configure(anchor='w')
        self.lbl_item1_price.configure(background="#d9d9d9")
        self.lbl_item1_price.configure(disabledforeground="#a3a3a3")
        self.lbl_item1_price.configure(font=font10)
        self.lbl_item1_price.configure(foreground="#000000")
        self.lbl_item1_price.configure(highlightbackground="#d9d9d9")
        self.lbl_item1_price.configure(highlightcolor="black")
        self.lbl_item1_price.configure(text=self.price)

        self.lbl_item1_quantity_left = tk.Label(self.lblframe_item1)
        self.lbl_item1_quantity_left.place(relx=0.181, rely=0.343, height=31
                                           , width=124, bordermode='ignore')
        self.lbl_item1_quantity_left.configure(activebackground="#f9f9f9")
        self.lbl_item1_quantity_left.configure(activeforeground="black")
        self.lbl_item1_quantity_left.configure(anchor='w')
        self.lbl_item1_quantity_left.configure(background="#d9d9d9")
        self.lbl_item1_quantity_left.configure(disabledforeground="#a3a3a3")
        self.lbl_item1_quantity_left.configure(font=font10)
        self.lbl_item1_quantity_left.configure(foreground="#000000")
        self.lbl_item1_quantity_left.configure(highlightbackground="#d9d9d9")
        self.lbl_item1_quantity_left.configure(highlightcolor="black")
        if self.quantity_left > 0:
            self.lbl_item1_quantity_left.configure(foreground="#000000")
            self.lbl_item1_quantity_left.configure(text='{} Left'.format(self.quantity_left))
        else:
            self.lbl_item1_quantity_left.configure(foreground="#bb0000")
            self.lbl_item1_quantity_left.configure(text='Out of Stock')

        self.btn_item1_add_to_cart = tk.Button(self.lblframe_item1)
        self.btn_item1_add_to_cart.place(relx=0.819, rely=0.686, height=44
                                        , width=155, bordermode='ignore')
        self.btn_item1_add_to_cart.configure(activebackground="#ececec")
        self.btn_item1_add_to_cart.configure(activeforeground="#000000")
        self.btn_item1_add_to_cart.configure(background="#d9d9d9")
        self.btn_item1_add_to_cart.configure(disabledforeground="#a3a3a3")
        self.btn_item1_add_to_cart.configure(font=font10)
        self.btn_item1_add_to_cart.configure(foreground="#000000")
        self.btn_item1_add_to_cart.configure(highlightbackground="#d9d9d9")
        self.btn_item1_add_to_cart.configure(highlightcolor="black")
        self.btn_item1_add_to_cart.configure(pady="0")
        self.btn_item1_add_to_cart.configure(text='Add To Cart')

        self.lbl_item1_category = tk.Label(self.lblframe_item1)
        self.lbl_item1_category.place(relx=0.181, rely=0.171, height=31
                                      , width=124, bordermode='ignore')
        self.lbl_item1_category.configure(activebackground="#f9f9f9")
        self.lbl_item1_category.configure(activeforeground="black")
        self.lbl_item1_category.configure(anchor='w')
        self.lbl_item1_category.configure(background="#d9d9d9")
        self.lbl_item1_category.configure(disabledforeground="#a3a3a3")
        self.lbl_item1_category.configure(font=font10)
        self.lbl_item1_category.configure(foreground="#000000")
        self.lbl_item1_category.configure(highlightbackground="#d9d9d9")
        self.lbl_item1_category.configure(highlightcolor="black")
        self.lbl_item1_category.configure(text=self.category)

        self.msg_item1_info = tk.Message(self.lblframe_item1)
        self.msg_item1_info.place(relx=0.319, rely=0.171, relheight=0.703
                                  , relwidth=0.468, bordermode='ignore')
        self.msg_item1_info.configure(anchor='nw')
        self.msg_item1_info.configure(background="#d9d9d9")
        self.msg_item1_info.configure(font=font13)
        self.msg_item1_info.configure(foreground="#000000")
        self.msg_item1_info.configure(highlightbackground="#d9d9d9")
        self.msg_item1_info.configure(highlightcolor="black")
        self.msg_item1_info.configure(text="Info: {}".format(self.info))
        self.msg_item1_info.configure(width=440)

        # ------------------------------------------------------------------
        # All Button Commands
        self.btn_item1_add_to_cart.configure(command=lambda: self.add_item_to_cart())

    def add_item_to_cart(self):
        # Call upon the add_item_to_cart method in "ShoppingCart.py".
        self.main_frame.add_item_to_cart(self)

    def destroy(self):
        """
            Destroy this widget.
        """
        self.lblframe_item1.destroy()

###########################################################
