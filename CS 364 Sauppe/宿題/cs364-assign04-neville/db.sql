CREATE USER 'grader'@'localhost' IDENTIFIED BY 'gradingpassword';
GRANT ALL PRIVILEGES ON * . * TO 'grader'@'localhost';

CREATE DATABASE MovieReview;

USE MovieReview;

CREATE TABLE Movie (
    MID INT PRIMARY KEY AUTO_INCREMENT,
    Title NVARCHAR(127) NOT NULL,
    Gross DECIMAL(8, 2) NOT NULL,
    DateReleased DATE NOT NULL
);

CREATE TABLE Actor (
    AID INT PRIMARY KEY NOT NULL AUTO_INCREMENT,
    FirstName NVARCHAR(31) NOT NULL,
    LastName NVARCHAR(31) NOT NULL,
    CreditedAs NVARCHAR(31)
);

CREATE TABLE User (
    Username NVARCHAR(15) PRIMARY KEY NOT NULL,
    Joined DATE NOT NULL
);

CREATE TABLE Reviews (
    Username NVARCHAR(15) NOT NULL,
    FOREIGN KEY (UserName) REFERENCES User(UserName),
    MID INT NOT NULL,
    FOREIGN KEY (MID) REFERENCES Movie(MID),
    Review NVARCHAR(255) NOT NULL,
    Posted TIMESTAMP NOT NULL
);

CREATE TABLE Stars (
    MID INT NOT NULL,
    FOREIGN KEY (MID) REFERENCES Movie(MID),
    AID INT NOT NULL,
    FOREIGN KEY (AID) REFERENCES Actor(AID)
);

-- INSERTIONS

INSERT INTO Actor (FirstName, LastName, CreditedAs) VALUES ("John", "Smith", "");
    
INSERT INTO Actor (FirstName, LastName, CreditedAs) VALUES ("Emily", "Jenson", "Em Jenson");
    
INSERT INTO Movie (Title, Gross, DateReleased) VALUES ("The Wild West", 6502831.03, "2004-10-05");
    
INSERT INTO Stars (AID, MID) VALUES (1, 1);
    
INSERT INTO Stars (AID, MID) VALUES (2,1);
    
INSERT INTO User (Username, Joined) VALUES ("movieJunkie", "2001-08-02 13:45:45"); 






