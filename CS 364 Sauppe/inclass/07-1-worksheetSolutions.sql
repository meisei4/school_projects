-- 1. Which countries purchased which genres 10 or more times?

SELECT BillingCountry, GenreName
	FROM Invoice NATURAL JOIN InvoiceLine NATURAL JOIN Track NATURAL JOIN (SELECT GenreId, Name AS GenreName FROM Genre) AS GenreView
    GROUP BY BillingCountry, GenreName
    HAVING count(*) >= 10;

-- 2. How many times did France purchase the Latin genre?

SELECT BillingCountry, GenreName, count(*)
	FROM Invoice NATURAL JOIN InvoiceLine NATURAL JOIN Track NATURAL JOIN (SELECT GenreId, Name AS GenreName FROM Genre) AS GenreView
    WHERE GenreName = "Latin" AND BillingCountry = "France";

-- 3. Which countries purchased the Latin genre more than France?

SELECT BillingCountry, GenreName, count(*)
	FROM Invoice NATURAL JOIN InvoiceLine NATURAL JOIN Track NATURAL JOIN (SELECT GenreId, Name AS GenreName FROM Genre) AS GenreView
    WHERE GenreName = "Latin"
    GROUP BY BillingCountry, GenreName
    HAVING count(GenreName) > (SELECT count(GenreName)
								FROM Invoice NATURAL JOIN InvoiceLine NATURAL JOIN Track NATURAL JOIN (SELECT GenreId, Name AS GenreName FROM Genre) AS GenreView
								WHERE GenreName = "Latin" AND BillingCountry = "France");
   
SELECT * 
    FROM Table1 NATURAL JOIN Table2 
    WHERE 
    GROUP BY
    HAVING
-- 4. What albums did each country purchase?

SELECT BillingCountry, Title
	FROM Invoice NATURAL JOIN InvoiceLine NATURAL JOIN Track NATURAL JOIN Album;

-- 5. How many times was each album purchased (regardless of country)?

SELECT Title, count(*)
	FROM Invoice NATURAL JOIN InvoiceLine NATURAL JOIN Track NATURAL JOIN Album
    GROUP BY Title;

-- 6. What was the most purchased album (regardless of country)? Assume there are not two or more albums tied for most purchased.

SELECT Title, count(Title)
	FROM Invoice NATURAL JOIN InvoiceLine NATURAL JOIN Track NATURAL JOIN Album
    GROUP BY Title
    ORDER BY count(Title) DESC
    LIMIT 1;

-- 7. What was the most purchased album (regardless of country)? Assume that there could be two or more albums tied for most purchased.

SELECT Title, count(Title)
	FROM Invoice NATURAL JOIN InvoiceLine NATURAL JOIN Track NATURAL JOIN Album
    GROUP BY Title
    HAVING count(Title) = (SELECT count(Title)
							FROM Invoice NATURAL JOIN InvoiceLine NATURAL JOIN Track NATURAL JOIN Album
							GROUP BY Title
							ORDER BY count(Title) DESC
							LIMIT 1);

-- 8. How many times did a country purchase each album?

SELECT Title, count(*), BillingCountry
	FROM Invoice NATURAL JOIN InvoiceLine NATURAL JOIN Track NATURAL JOIN Album
    GROUP BY BillingCountry;

-- 9. What is the largest number of purchases of a single album made by each country? List both the country and the number of purchases, but you do not need to list the album that was most purchased.

SELECT BillingCountry, max(TitleCount)
	FROM (SELECT count(*) AS TitleCount, BillingCountry
					FROM Invoice NATURAL JOIN InvoiceLine NATURAL JOIN Track NATURAL JOIN Album
					GROUP BY Title, BillingCountry) AS counts
	GROUP BY BillingCountry;


SELECT count(DISTINCT BillingCountry)
