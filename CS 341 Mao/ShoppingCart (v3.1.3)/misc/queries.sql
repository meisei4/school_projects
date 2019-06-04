--AT CHECK OUT ADDRESS INSERT
INSERT INTO Addresses (AID, Address1, Address2, Address3, City, State, Country, Zipcode)
               VALUES ('6', '666 6th st', '', '', 'Hell', 'ND', 'US', '66666');
UPDATE User
	SET address = '6'
	WHERE Username = "testuser";

--REGISTER NEW USER
INSERT INTO User (Username, Password, FirstName,LastName,Email,Role)
          VALUES ('testuser', 'testPass','TestFirst','TestLast', 'email@place.com','user');

--Get product table for main browsing page
SELECT Name, Category, Price, Quantity, Info
    FROM Products

--get the amount of times each product has been purchased
SELECT Name, count(*) 
    FROM Products NATURAL JOIN Purchases
    GROUP BY Name;

-- Filter products by name/category
SELECT PID, Name, Category, Price, Quantity, Info 
    FROM Products
    WHERE [column] = "identifier"
    ORDER BY [column] ASC 
--or
    ORDER BY [column] DESC
	-- if price filter use, change WHERE line to:
	WHERE Price BETWEEN [lower_range] AND [upper_range]
	-- if "in stock" filter used, change WHERE line to
    WHERE Quantity > 0
	-- if more than one filter, change "WHERE" to "AND" and put it after the WHERE

--Example filter:
SELECT PID, Name, Category, Price, Quantity, Info
    FROM Products
    WHERE Category = "electronics" AND Price BETWEEN 15 AND 100000
    ORDER BY Price ASC
	
-- SELECT count() of rows to check if entered username and password exist, count should be 1 or 0
SELECT count(*)
    FROM User
    WHERE Username = "username"
    AND Password = "password"

--get user info upon login for user page
SELECT Username, FirstName, LastName, Email, Address1, Address2, Address3, City, State, Country, Zipcode 
    FROM User NATURAL JOIN Addresses
    WHERE Username = "username" 
