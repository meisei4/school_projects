
""" Header.py

    Description:
        This is the Header for the self.top_frame of each page.

    Team: Daniel Bartelson, Ian Neville, Brian Draheim, Kong Jimmy Vang
    Coder(s): Kong Jimmy Vang, Ian Neville, Daniel Bartelson
    Database: Brian Draheim
"""

###########################################################
# Python Imports
import os
import sys
import sqlite3
import math

# Check the version of Python and import accordingly.
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


DEBUG = True

###########################################################


class Header(tk.Frame):
    """
    Simple header for the top of all pages.

    This class will hold the search functions, search boxes and search buttons.

    :param main_frame: Main Frame (Must be a MainFrame instance in 'ShoppingCart.py')
    :param top_frame: A reference to the TOP Tkinter.Frame object this Header is attached to.
    :param username: Stored username for the header to display.
                [Used by 'ShoppingCart.py' method(s): move_header()  ]
    :param search_box_default_text: Default text in search box.
                [Used by 'ShoppingCart.py' functions(s): move_header()  ]
    :param login_register_button_text: Default text for the top-right button.
                [Used by 'ShoppingCart.py' functions(s): move_header()  ]

    """

    def __init__(self,
                 main_frame=None,
                 top_frame=None,
                 username='*N/A*',
                 search_box_default_text='',
                 login_register_button_text='Log In/Register',
                 cart_amount=0,
                 *args, **kwargs):

        tk.Frame.__init__(self, *args, **kwargs)

        font10 = "-family {Franklin Gothic Heavy} -size 12"
        font12 = "-family {Franklin Gothic Heavy} -size 11"
        font14 = "-family {Franklin Gothic Heavy} -size 13"
        font17 = "-family {Franklin Gothic Heavy} -size 15 -underline "  \
            "1"
        font18 = "-family {Franklin Gothic Heavy} -size 15"
        font21 = "TkDefaultFont"
        font9 = "-family {Franklin Gothic Heavy} -size 14"

        ### Attributes ###
        self.top_frame = top_frame
        self.main_frame = main_frame
        self.username = username
        self.search_box_default_text = search_box_default_text
        self.cart_amount = cart_amount

        ### Widgets ###
        self.frame_header = tk.Frame(self.top_frame)
        self.frame_header.place(relx=0.01, rely=0.013, relheight=0.111
                , relwidth=0.981)
        self.frame_header.configure(relief='groove')
        self.frame_header.configure(borderwidth="2")
        self.frame_header.configure(relief='groove')
        self.frame_header.configure(background="#d9d9d9")
        self.frame_header.configure(highlightbackground="#d9d9d9")
        self.frame_header.configure(highlightcolor="black")
        self.frame_header.configure(width=1005)

        self.lbl_user = tk.Label(self.frame_header)
        self.lbl_user.place(relx=0.109, rely=0.118, height=34, width=720)
        self.lbl_user.configure(activebackground="#f9f9f9")
        self.lbl_user.configure(activeforeground="black")
        self.lbl_user.configure(anchor='e')
        self.lbl_user.configure(background="#d9d9d9")
        self.lbl_user.configure(disabledforeground="#a3a3a3")
        self.lbl_user.configure(font=font14)
        self.lbl_user.configure(foreground="#000000")
        self.lbl_user.configure(highlightbackground="#d9d9d9")
        self.lbl_user.configure(highlightcolor="black")
        self.lbl_user.configure(text='Hello {}!'.format(username))

        self.btn_login_register = tk.Button(self.frame_header)
        self.btn_login_register.place(relx=0.836, rely=0.118, height=34
                , width=151)
        self.btn_login_register.configure(activebackground="#ececec")
        self.btn_login_register.configure(activeforeground="#000000")
        self.btn_login_register.configure(background="#d9d9d9")
        self.btn_login_register.configure(disabledforeground="#a3a3a3")
        self.btn_login_register.configure(font=font12)
        self.btn_login_register.configure(foreground="#000000")
        self.btn_login_register.configure(highlightbackground="#d9d9d9")
        self.btn_login_register.configure(highlightcolor="black")
        self.btn_login_register.configure(pady="0")
        self.btn_login_register.configure(text=login_register_button_text)

        self.btn_home = tk.Button(self.frame_header)
        self.btn_home.place(relx=0.01, rely=0.118, height=34, width=84)
        self.btn_home.configure(activebackground="#ececec")
        self.btn_home.configure(activeforeground="#000000")
        self.btn_home.configure(background="#d9d9d9")
        self.btn_home.configure(disabledforeground="#a3a3a3")
        self.btn_home.configure(font=font12)
        self.btn_home.configure(foreground="#000000")
        self.btn_home.configure(highlightbackground="#d9d9d9")
        self.btn_home.configure(highlightcolor="black")
        self.btn_home.configure(pady="0")
        self.btn_home.configure(text='Home')
        #self.btn_home.configure(state='disabled')

        self.btn_browse = tk.Button(self.frame_header)
        self.btn_browse.place(relx=0.01, rely=0.588, height=24, width=99)
        self.btn_browse.configure(activebackground="#ececec")
        self.btn_browse.configure(activeforeground="#000000")
        self.btn_browse.configure(background="#d9d9d9")
        self.btn_browse.configure(disabledforeground="#a3a3a3")
        self.btn_browse.configure(font="-family {Franklin Gothic Heavy} -size 11")
        self.btn_browse.configure(foreground="#000000")
        self.btn_browse.configure(highlightbackground="#d9d9d9")
        self.btn_browse.configure(highlightcolor="black")
        self.btn_browse.configure(pady="0")
        self.btn_browse.configure(text='Browse')
        self.btn_browse.configure(width=99)

        self.entry_search_box = tk.Entry(self.frame_header)
        self.entry_search_box.place(relx=0.159, rely=0.588, height=24
                , relwidth=0.392)
        self.entry_search_box.configure(background="white")
        self.entry_search_box.configure(disabledforeground="#a3a3a3")
        self.entry_search_box.configure(font="-family {Arial} -size 10 -weight bold")
        self.entry_search_box.configure(foreground="#000000")
        self.entry_search_box.configure(highlightbackground="#d9d9d9")
        self.entry_search_box.configure(highlightcolor="black")
        self.entry_search_box.configure(insertbackground="black")
        self.entry_search_box.configure(selectbackground="#c4c4c4")
        self.entry_search_box.configure(selectforeground="black")
        self.entry_search_box.configure(width=394)
        self.entry_search_box.delete(0, END)
        self.entry_search_box.insert(0, str(self.search_box_default_text))
        #self.entry_search_box.bind('<FocusIn>', self.save_search_box)
        #self.entry_search_box.bind('<FocusOut>', self.save_search_box)
        self.entry_search_box.bind('<Return>', self.search_event)

        self.btn_search = tk.Button(self.frame_header)
        self.btn_search.place(relx=0.567, rely=0.588, height=24, width=96)
        self.btn_search.configure(activebackground="#ececec")
        self.btn_search.configure(activeforeground="#000000")
        self.btn_search.configure(background="#d9d9d9")
        self.btn_search.configure(disabledforeground="#a3a3a3")
        self.btn_search.configure(font="-family {Franklin Gothic Heavy} -size 11")
        self.btn_search.configure(foreground="#000000")
        self.btn_search.configure(highlightbackground="#d9d9d9")
        self.btn_search.configure(highlightcolor="black")
        self.btn_search.configure(pady="0")
        self.btn_search.configure(text='Search')
        self.btn_search.configure(width=96)

        # self.lbl_advanced_search = tk.Label(self.frame_header)
        # self.lbl_advanced_search.place(relx=0.672, rely=0.588, height=24
        #         , width=134)
        # self.lbl_advanced_search.configure(activebackground="#f9f9f9")
        # self.lbl_advanced_search.configure(activeforeground="black")
        # self.lbl_advanced_search.configure(anchor='w')
        # self.lbl_advanced_search.configure(background="#d9d9d9")
        # self.lbl_advanced_search.configure(disabledforeground="#a3a3a3")
        # self.lbl_advanced_search.configure(foreground="#0645AD")
        # self.lbl_advanced_search.configure(highlightbackground="#d9d9d9")
        # self.lbl_advanced_search.configure(highlightcolor="black")
        # self.lbl_advanced_search.configure(text='Advanced Search')
        # self.lbl_advanced_search.configure(width=134)

        self.btn_view_cart = tk.Button(self.frame_header)
        self.btn_view_cart.place(relx=0.886, rely=0.588, height=24, width=101)
        self.btn_view_cart.configure(activebackground="#ececec")
        self.btn_view_cart.configure(activeforeground="#000000")
        self.btn_view_cart.configure(background="#d9d9d9")
        self.btn_view_cart.configure(disabledforeground="#a3a3a3")
        self.btn_view_cart.configure(font=font12)
        self.btn_view_cart.configure(foreground="#000000")
        self.btn_view_cart.configure(highlightbackground="#d9d9d9")
        self.btn_view_cart.configure(highlightcolor="black")
        self.btn_view_cart.configure(pady="0")
        self.btn_view_cart.configure(text='View Cart')

        self.lbl_view_cart_amount = tk.Label(self.frame_header)
        self.lbl_view_cart_amount.place(relx=0.806, rely=0.588, height=24
                , width=74)
        self.lbl_view_cart_amount.configure(activebackground="#f9f9f9")
        self.lbl_view_cart_amount.configure(activeforeground="black")
        self.lbl_view_cart_amount.configure(anchor='e')
        self.lbl_view_cart_amount.configure(background="#d9d9d9")
        self.lbl_view_cart_amount.configure(disabledforeground="#a3a3a3")
        self.lbl_view_cart_amount.configure(foreground="#000000")
        self.lbl_view_cart_amount.configure(highlightbackground="#d9d9d9")
        self.lbl_view_cart_amount.configure(highlightcolor="black")
        self.lbl_view_cart_amount.configure(text="{} Items".format(self.cart_amount))

        # ------------------------------------------------------------------
        # All Button Commands
        self.btn_login_register.configure(command=self.main_frame.goto_login_register_page)
        self.btn_home.configure(command=lambda: self.main_frame.browse())
        self.btn_browse.configure(command=lambda: self.main_frame.browse())
        self.btn_search.configure(command=lambda: self.search(self.entry_search_box.get()))
        self.btn_view_cart.configure(command=lambda: self.main_frame.goto_cart_page())

    def search_event(self, event):
        """
        Perform a search when a tkinter event occurs. (Used for the "bind" functions.)
        """
        self.search(self.entry_search_box.get())

    def search(self, search_text=""):
        """
        Perform a search.
        """
        self.main_frame.search(search_text)

    def set_lbl_view_cart_amount(self, num_of_items):
        """
        Set the display value of the Shopping CartItem Amount.
        :param num_of_items: The number of items to display.
        """
        self.lbl_view_cart_amount.configure(text='{} Items'.format(num_of_items))

    def destroy(self):
        """
        Destroy this widget.
        """
        self.frame_header.destroy()

###########################################################
