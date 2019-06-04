CREATE SCHEMA ShoppingCart;

USE ShoppingCart;

CREATE TABLE Addresses (
    AID INT PRIMARY KEY NOT NULL,
    Address1 VARCHAR(120) NOT NULL,
    Address2 VARCHAR(120),
    Address3 VARCHAR(120),
    City VARCHAR(31) NOT NULL,
    State VARCHAR(2) NOT NULL,
    Country VARCHAR(3) NOT NULL,
    Zipcode VARCHAR(10) NOT NULL
);

CREATE TABLE User (
    Username VARCHAR(31) PRIMARY KEY NOT NULL,
    Password VARCHAR(31),
    FirstName VARCHAR(31),
    LastName VARCHAR(31),
    Email VARCHAR(50),
    Role VARCHAR(31) NOT NULL,
    Address INT,
    FOREIGN KEY (Address) REFERENCES Addresses(AID)
 );

CREATE TABLE Cart (
    CID INT PRIMARY KEY NOT NULL,
    Username VARCHAR(31) NOT NULL,
    FOREIGN KEY (Username) REFERENCES User(Username),
    DateCreated DATE NOT NULL,
    CheckedOut BOOLEAN NOT NULL Default 0
);

CREATE TABLE Products (
    PID INT PRIMARY KEY NOT NULL,
    Name VARCHAR(31) NOT NULL,
    Category VARCHAR(31) NOT NULL,
    Price DOUBLE NOT NULL,
    Quantity INT NOT NULL,
    Info VARCHAR(255)
);

CREATE TABLE CartItems (
    CartItemId INT PRIMARY KEY NOT NULL,
    CID INT NOT NULL,
    FOREIGN KEY (CID) REFERENCES Cart(CID),
    PID INT NOT NULL,
    FOREIGN KEY (PID) REFERENCES Products(PID),
    Quantity INT NOT NULL
);

CREATE TABLE Purchases (
    Username VARCHAR(31) NOT NULL,
    FOREIGN KEY (Username) REFERENCES User(Username),
    PID INT NOT NULL,
    FOREIGN KEY (PID) REFERENCES Products(PID),
    Date DATE NOT NULL,
    AID INT NOT NULL,
    FOREIGN KEY (AID) REFERENCES Addresses(AID)
);

INSERT INTO Users (Username) VALUES ("guest");

INSERT INTO Users (Username, Password) VALUES ("admin", "changethis!"); 
