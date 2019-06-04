
""" ShoppingCart.py
    
    Description:
        This is the Main GUI.
    
    Team: Daniel Bartelson, Ian Neville, Brian Draheim, Kong Jimmy Vang
    Coder(s): Kong Jimmy Vang, Ian Neville, Daniel Bartelson
    Database: Brian Draheim

    Last Updated: May 7th, 2019
"""

###########################################################
# Python Imports
import os
import sys
import sqlite3
import datetime
import base64
from cryptography.fernet import Fernet

# Our Imports (Pages)
from Header import Header
from HomePage import HomePage
from LoginRegisterPage import LoginRegisterPage
from CartPage import CartPage
from AccountPage import AccountPage

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

# OS variables
file_path = os.path.dirname(sys.argv[0])
file_name = os.path.basename(sys.argv[0])
print("\nFile Path: {}".format(str(file_path)))
print("\nFile Name: {}".format(str(file_name)))
print("Python Info: {}\n".format(str(sys.version_info)))


DEBUG = True

############################################################################################


class MainFrame(tk.Frame):
    """
        The Mainframe of the root class.

        Initializes the HomePage frame.
    """
    def __init__(self, *args, **kwargs):
        """
            :param args:
            :param kwargs:
        """

        tk.Frame.__init__(self, *args, **kwargs)

        ### Attributes ###
        self.conn = sqlite3.connect("ShoppingCart.db")
        self.cursor = self.conn.cursor()
        self.username = "guest"
        self.cid = 0
        self.role = "guest"
        self.cart_amount = 0

        self.active_promos = []

        ### Create Main Widget/Container. (Will contain all pages) ###
        self.container = tk.Frame(self)
        self.container.pack(side="top", fill="both", expand=True)

        ### Add a header (Will be updated using the function "move_header" ###
        self.header = Header(main_frame=self)

        ### Create page widgets and place them down. ###
        self.home = HomePage(top_frame=self)
        self.home.place(in_=self.container, x=0, y=0, relwidth=1, relheight=1)

        self.login_reg = LoginRegisterPage(top_frame=self)
        self.login_reg.place(in_=self.container, x=0, y=0, relwidth=1, relheight=1)

        self.account = AccountPage(top_frame=self)
        self.account.place(in_=self.container, x=0, y=0, relwidth=1, relheight=1)

        self.cart = CartPage(top_frame=self)
        self.cart.place(in_=self.container, x=0, y=0, relwidth=1, relheight=1)

        ### Show the home page to the user. ###
        self.goto_home_page()

        ### Create a user cart. ###
        self.create_user_cart()

        ### Other Setups ###
        self.setup()

    def setup(self):
        """
        Setup databases/etc.
        """
        self.remove_all_user_applied_promotions()

    def remove_all_user_applied_promotions(self):
        """
        Remove all applied promotions from a user's Cart in the database.
        :return:
        """
        self.cursor.execute('UPDATE CartItems '
                            + 'SET PromoID = ? '
                            + 'WHERE CID = {}'.format(self.cid)
                            , (None,))  # (None,) = NULL in database.

        self.conn.commit()  # Commit the changes.
        self.reconnect_database_conn()

    def move_header(self, new_top_frame):
        """
            Moves the Header to a new page.

            NOTE: In tkinter, It appearently isn't possible to attach Tkinter.Frames to a new Parent/Master Frame
                once they are created already.

                Because of that, I made this custom function which creates a new Header Tk.Frame
                every time the user wants to go to a new page. This allows me to attach the Header
                to a new Parent/Master Frame every time, thus allowing the top header to move across pages.

                Also, this function will prevent the top header from losing information such as:
                    - The Search Box's text info when enter is pressed
                    - The text that displays the Logged In User


            :param new_top_frame: The Frame (Such as the HomePage, LoginRegisterPage, etc.) to attach this header to.

        """

        # Save old values.
        username = self.username
        search_box_default_text = self.header.entry_search_box.get()
        login_register_button_text = self.header.btn_login_register.cget('text')

        self.cursor.execute("SELECT CID FROM Cart WHERE Username = '{}' AND CheckedOut == 0".format(self.username))
        row = self.cursor.fetchone()

        if row is not None:
            self.cid = row[0]
            self.cursor.execute("SELECT sum(Quantity) FROM CartItems WHERE CID = {}".format(self.cid))
            quantity = self.cursor.fetchone()[0]

            if quantity is not None:
                self.cart_amount = quantity
            else:
                self.cart_amount = 0
        else:
            self.cart_amount = 0

        # Destroy current header.
        self.header.destroy()

        # Create a new header with the old values and attach this header to a "new_top_frame".
        self.header = Header(top_frame=new_top_frame,
                             main_frame=self,
                             username=username,
                             search_box_default_text=search_box_default_text,
                             login_register_button_text=login_register_button_text,
                             cart_amount=self.cart_amount)

        # Place the header down on the main_frame's container.
        self.header.place(in_=self.container, x=0, y=0, relwidth=0, relheight=0)

    def goto_home_page(self):
        """
        Shows the Home Page.
        """
        self.move_header(new_top_frame=self.home)  # Update Header (Read the doc for "move_header()" for more info.)
        self.home.lift()  # Show this page.
        self.home.focus()

    def goto_login_register_page(self):
        """
        Shows the Login/Register Page.
        """
        # If the user is LOGGED OUT, send user to the login/register page.
        if self.role == "guest":
            self.move_header(new_top_frame=self.login_reg)  # Update Header (Read the doc for "move_header()" for more info.)
            self.login_reg.lift()  # Show this page.
            self.login_reg.focus()

        # If the user is LOGGED IN, log the user out.
        else:
            self.goto_account_page()

    def goto_account_page(self):
        """
        Shows the Account Page.
        """
        self.cursor.execute("SELECT Role FROM User WHERE Username == '{}'".format(self.username))
        self.role = self.cursor.fetchone()[0]

        self.move_header(new_top_frame=self.account)
        self.account.setup()
        self.account.lift()  # Show this page.
        self.account.focus()

    def goto_cart_page(self):
        """
        Shows the Cart Page.
        """
        self.move_header(new_top_frame=self.cart)  # Update Header (Read the doc for "move_header()" for more info.)
        self.cart.refresh_user_cart()
        self.cart.lift()  # Show this page.
        self.cart.focus()

    def reconnect_database_conn(self):
        self.conn = sqlite3.connect("ShoppingCart.db")
        self.cursor = self.conn.cursor()

    def browse(self):
        """
        Browse all products/items.
        """
        self.home.browse()
        self.goto_home_page()

    def search(self, search_text=""):
        """
        Search for a particular product/item.

        :param search_text:
        """
        self.home.search(search_text=search_text)
        self.goto_home_page()

    def create_user_cart(self):
        """
        Creates a user's cart and append it to the database.
        """

        # Get today's date.
        time = datetime.datetime.now()
        today = time.strftime("%Y-%m-%d")

        # Get highest CID number. Only if the current CID doesn't exist.
        self.reconnect_database_conn()

        self.cursor.execute("SELECT CID FROM Cart WHERE CID = {}".format(self.cid))
        c = self.cursor.fetchone()

        self.cursor.execute("SELECT MAX(CID) FROM Cart")
        rowid = self.cursor.fetchone()[0]

        if rowid is not None and c is not None:
            self.cid = rowid
            self.cid += 1  # Increment to a new CID.

        self.cursor.execute("SELECT CID, Username FROM Cart WHERE Username == '{}' AND CheckedOut != 1"
                            .format(self.username))
        row = self.cursor.fetchone()

        if row:
            # User has a cart, since row = [CID, Username]
            self.cid = row[0]
        else:
            # User has no cart, since row = []
            row = ["", ""]

        database_user = row[1]

        if database_user != self.username:
            # If the user does not have a shopping Cart, create a new Cart for them.
            self.cursor.execute('INSERT INTO "main"."Cart" '
                                + '("CID", "Username", "CheckedOut", "DateCreated") '
                                + "VALUES ({}, '{}', {}, '{}');".format(self.cid, self.username, 0, today))
        self.conn.commit()  # Commit the changes.
        self.reconnect_database_conn()

    def add_item_to_cart(self, itembox):
        """
        Add a product/item to the user's cart.

        :param itembox: An ItemBox object/instance. (ItemBox.py Class)
        """

        ### Checks to make sure the user has a shopping Cart. ###
        self.create_user_cart()

        ### Is there any products in stock? ###
        quantity_left = itembox.quantity_left
        if quantity_left <= 0:
            tk.messagebox.showinfo('Out of stock.',
                                    'Sorry, that product is unavailable.',
                                    icon='info')
            return

        ### Check if the Cart contains a Cart Item with given CID and PID. ###
        self.cursor.execute("SELECT * FROM CartItems WHERE CID == {} AND PID == {}"
                            .format(self.cid, itembox.pid))
        rows = self.cursor.fetchall()

        # Cart Item exists.
        if rows:
            # row = [CartItemId, CID, PID, Quantity, PromoID]
            row = rows[0]

            # Check if promo was applied.
            if itembox.pid in self.active_promos:
                self.cursor.execute("SELECT * FROM Promotions WHERE PID = '{}'".format(itembox.pid))
                promo_id = self.cursor.fetchone()[0]
                self.cursor.execute('UPDATE CartItems '
                                    + 'SET Quantity = Quantity + 1, PromoID = {} '.format(promo_id)
                                    + 'WHERE CID = {} AND PID = {}'.format(self.cid, itembox.pid))
            else:
                self.cursor.execute('UPDATE CartItems SET Quantity = Quantity + 1 '
                                    + 'WHERE CID = {} AND PID = {}'.format(self.cid, itembox.pid))

        # Cart item does not exist.
        else:
            row = ["", "", "", "", ""]

            # Get highest CartItemId
            self.cursor.execute("SELECT MAX(CartItemId) FROM CartItems")
            rowid = self.cursor.fetchall()[0][0]

            if rowid is not None:
                cart_item_id = rowid
                cart_item_id += 1  # Increment to a new CartItemId.
            else:
                cart_item_id = 0

            if itembox.pid in self.active_promos:
                self.cursor.execute("SELECT * FROM Promotions WHERE PID = '{}'".format(itembox.pid))
                promo_id = self.cursor.fetchone()[0]
                self.cursor.execute('INSERT INTO CartItems '
                                    + '("CartItemId", "CID", "PID", "Quantity", "PromoID") '
                                    + "VALUES ({}, '{}', {}, {}, ".format(cart_item_id, self.cid, itembox.pid, 1)
                                    + "'{}');".format(promo_id))

            else:
                self.cursor.execute('INSERT INTO CartItems '
                                    + '("CartItemId", "CID", "PID", "Quantity", "PromoID") '
                                    + "VALUES ({}, '{}', {}, {}, ".format(cart_item_id, self.cid, itembox.pid, 1)
                                    + "?);", (None,))  # (None,) = NULL in database.

        self.conn.commit()  # Commit the changes.
        self.reconnect_database_conn()

        # Update the header to display correct amount of items in shopping cart.
        self.cursor.execute('SELECT sum(Quantity) FROM CartItems WHERE CID = {}'.format(self.cid))
        self.cart_amount = self.cursor.fetchone()[0]

        self.header.set_lbl_view_cart_amount(self.cart_amount)

        # TODO: Edit the ItemBox to include the amount of product to be added, then uncomment the lines below.
        """if tk.messagebox.askyesno('Product Added',
                                  'This product was added to your cart. Would you like to view your cart?',
                                  icon='info'):
            # Yes
            self.goto_cart_page()
        else:
            # No
            pass"""


        # ---END of add_item_to_cart()---

    # TODO: Not used, may remove or rework this method.
    """def remove_from_cart(self, cart_item_box):
        if (cart_item_box.quantity == 1):
            self.cursor.execute("DELETE FROM CartItems WHERE CartItemId = {}".format(cart_item_box.cart_item_id))

        else:
            self.cursor.execute('UPDATE CartItems SET Quantity = Quantity - 1 '
                                + 'WHERE CID = {} AND PID = {}'.format(self.cid, cart_item_box.pid))
    """

    def login(self):
        """
        Login as a user.

        Performs a check if the user exists in the database. If not, warn the user.
            Next, check the password if it is correct. If not, warn the user.
            If the user types the correct password, they will be given a message and be successfully logged in.
        """
        if DEBUG:
            print("__________________\n"
                  "\\__ EVENT Login __/")

        username = str(self.login_reg.entry_login_username.get())
        passwd = str(self.login_reg.entry_login_password.get())

        # TODO: Escape Special Characters.
        # Check if the user is in the database.
        self.cursor.execute("SELECT * FROM User WHERE Username = '{}'".format(username))
        rows = self.cursor.fetchall()

        if rows:
            # row = [Username, Password, FirstName, LastName, Email, Role, Address]
            row = rows[0]
        else:
            row = ["", ""]

        database_user = row[0]
        database_password_encrypted = row[1]

        # Decrypt Password from the database.
        try:
            database_password_decrypted = decrypt(key=global_key, source=database_password_encrypted)
        except:
            if DEBUG: print("THIS USER IS -NOT- IN THE DATABASE!")
            tk.messagebox.showerror('Login Failure', 'That user does not exist.', icon='error')
            return

        if database_user == username:
            if DEBUG: print("THIS USER IS IN THE DATABASE!")
        else:
            if DEBUG: print("THIS USER IS -NOT- IN THE DATABASE!")
            tk.messagebox.showerror('Login Failure', 'That user does not exist.', icon='error')
            return

        if database_password_decrypted != passwd:
            if DEBUG: print("INCORRECT PASSWORD!")
            tk.messagebox.showerror('Login Failure', 'The password is incorrect.', icon='error')
            return
        else:
            if DEBUG: print("CORRECT PASSWORD!")
            self.username = username
            self.header.lbl_user.configure(text='Hello {}!'.format(username))

        # Successful login!
        self.header.btn_login_register.configure(text="Account")
        tk.messagebox.showinfo('Login Successful', 'Successfully logged in as {}.'.format(self.username), icon='info')
        self.login_reg.reset_defaults()

        # Flag the user as logged in.
        self.role = row[5]
        self.goto_home_page()

        if DEBUG:
            print("Username: {}".format(username))
            # print("Password: {}".format(passwd))
            print("Password (Encrypted): {}".format(database_password_encrypted))

        # Create that user's Cart.
        self.create_user_cart()
        self.remove_all_user_applied_promotions()  # Clear all applied promotions from the database for current user.
        self.active_promos.clear()  # Clear all applied promotions.

    def logout_user(self):
        """
        Log out the user. Keep them on the current page they are on.

        If they are on the "Logout Page", send them to the "Login And Registration Page".
        """
        if DEBUG:
            print("___________________\n"
                  "\\__ EVENT Logout __/")

        # Flag the user as logged out.
        self.role = "guest"
        self.username = "guest"
        self.header.btn_login_register.configure(text="Log In/Register")
        self.move_header(new_top_frame=self.header.top_frame)

        tk.messagebox.showinfo('Logout Successful', 'Successfully Logged out.'.format(self.username), icon='info')

        # Go to the login/register page.
        if self.header.top_frame == self.account:
            self.goto_login_register_page()

    def logout_goto_login_register_page(self):
        """
        Log out the user and send them to the "Login And Registration Page".
        """
        self.logout_user()
        self.goto_login_register_page()

    def register(self):
        """
        Register as a user.

        Performs a check if the current typed-in username is already taken. If taken, warn the user.
            Next, the password is checked with the confirmed password. If they don't match, warn the user.
            After that, the user will be registered with the info they provided.
        """
        if DEBUG:
            print("_____________________\n"
                  "\\__ EVENT Register __/")
        
        username = str(self.login_reg.entry_register_username.get())
        passwd = str(self.login_reg.entry_register_password.get())
        passwd_c = str(self.login_reg.entry_register_confirm_password.get())
        f_name = str(self.login_reg.entry_register_first_name.get())
        #m_name = str(self.login_reg.entry_register_middle_name.get())
        l_name = str(self.login_reg.entry_register_last_name.get())
        #s_address = str(self.login_reg.entry_register_street_address.get())
        #z_code = str(self.login_reg.entry_register_zip_code.get())

        # Make query to check if username is taken
        self.cursor.execute("SELECT * FROM User WHERE Username = '{}'".format(username))
        rows = self.cursor.fetchall()

        if rows:
            row = rows[0]
        else:
            row = ["", ""]

        database_user = row[0]

        if database_user == username:
            if DEBUG: print('USERNAME "{}" IS TAKEN.'.format(username))
            tk.messagebox.showerror('Registration Failure', 'That username is taken.', icon='error')
            return  # Return on failure.
        else:
            if DEBUG: print('USERNAME "{}" IS NOT TAKEN.'.format(username))
        
        # Check if password and check password match
        if passwd == passwd_c:
            if DEBUG: print("PASSWORDS MATCH.")
        else:
            if DEBUG: print("PASSWORDS DO NOT MATCH.")
            tk.messagebox.showerror('Registration Failure', 'The password and confirmed password did not match.', icon='error')
            return  # Return on failure.
        
        # Make query to insert user if above passes
        self.reconnect_database_conn()
        self.cursor.execute("SELECT MAX(AID) FROM Addresses")
        rowid = self.cursor.fetchall()[0][0]

        if rowid is not None:
            aid = rowid
            aid += 1  # Increment to a new AID.
        else:
            aid = 0

        ### Store all registration information into the database. ###
        self.cursor.execute('INSERT INTO Addresses ("AID", "Address1", "City", "State", "Country", "Zipcode") '
                            + "VALUES ({}, '{}', '{}', '{}', '{}', '{}');"
                            .format(aid, "", "", "", "", ""))

        # Encrypt Password for storage.
        passwd_encrypted = encrypt(key=global_key, source=passwd)
        # self.cursor.execute("INSERT INTO User (Username,Password,FirstName,LastName,Email,Role,Address) "
        #                     + "VALUES ('{}','{}','{}','{}','{}','{}','{}') "
        #                     .format(username, passwd_encrypted, f_name, l_name, '', 'user', aid))
        self.cursor.execute("INSERT INTO User (Username,Password,FirstName,LastName,Role,Address) "
                            + "VALUES ('{}','{}','{}','{}','{}','{}') "
                            .format(username, passwd_encrypted, f_name, l_name, 'user', aid))
        self.conn.commit()  # Commit the changes.

        # Successful registration!
        tk.messagebox.showinfo('Registration Successful', 'Successfully registered the user "{}".'.format(username), icon='info')
        self.login_reg.reset_defaults()


        # if our debug mode is on, print out the variables
        if DEBUG:
            print("Username: {}".format(username))
            print("Password (Encrypted): {}".format(passwd_encrypted))
            # print("Confirm Password: {}".format(passwd_c))
            print("First Name: {}".format(f_name))
            #print("Middle Name: {}".format(m_name))
            print("Last Name: {}".format(l_name))
            #print("StreetAddress: {}".format(s_address))
            #print("ZIP Code: {}".format(z_code))

            self.cursor.execute("SELECT * FROM User")
            rows = self.cursor.fetchall()

            print("\nHere are all the users in the database.")
            for row in rows:
                print(row)

        # End of register()


############################################################################################

w = None


def destroy():
    """
    Destroys the GUI. Needed to allow the root class of the GUI of tkinter to be destroyed.
    """
    global w
    w.destroy()
    w = None


# TODO: NOTE - Currently using Single key encryption.
# TODO: NOTE - Use public/private key encryption system for safer results.

global_key = b'pRmgMa8T0INjEAfksaq2aafzoZXEuwKI7wDe4c1F8AY='
# key = Fernet.generate_key() <--- Generates a random key.


def encrypt(key, source):
    """
    Encrypts a source String.
    :param key: The key.
    :param source: The String that will be encrypted.
    :return: Returns an encrypted String in the "utf-8" format.
    """

    cipher_suite = Fernet(key)

    # Encrypt.
    source_bytes = bytes(source, 'utf-8')
    bytes_ciphered_text = cipher_suite.encrypt(source_bytes)  # "source" is required to be bytes for encryption
    return bytes_ciphered_text.decode("utf-8")  # Convert Decrypted Bytes to String.


def decrypt(key, source):
    """
    Decrypts a source String.
    :param key: The key.
    :param source: The String that will be encrypted.
    :return: Returns an decrypted String in the "utf-8" format.
    """

    cipher_suite = Fernet(key)

    # Decrypt.
    source_bytes = bytes(source, 'utf-8')
    bytes_unciphered_text = cipher_suite.decrypt(source_bytes)
    return bytes_unciphered_text.decode("utf-8")  # Convert Decrypted Bytes to String.


def tester():
    """
    Minor Tester for Encryption/Decryption.
    """
    ORIGINAL = "jdoe"
    e = encrypt("", ORIGINAL)
    d = decrypt("", e)

    print(e)
    print(d)


def main():
    """ Starting point when module is the main routine. """
    global val, w, root, main_frame

    # Create GUI.
    root = tk.Tk()
    root.title('Shopping Cart')
    root.geometry("1024x768+459+174")
    # root.resizable(False, False)

    # Set the icon (Top-left icon in the GUI)
    # root.iconbitmap(os.path.join('Images', 'icon.ico'))

    main_frame = MainFrame(root)
    main_frame.pack(side="top", fill="both", expand=True)
    root.wm_geometry("1024x768+459+174")

    root.mainloop()


"""
    If main, run program.
"""
if __name__ == '__main__':
    # tester()
    main()

##############################################################################################
