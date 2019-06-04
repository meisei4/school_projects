SELECT BillingCountry, count(BillingCountry)
	FROM Invoice NATURAL JOIN InvoiceLine
	GROUP BY BillingCountry 
	ORDER BY count(BillingCountry) DESC
	LIMIT 1;

SELECT * 
	FROM Artist NATURAL JOIN Album NATURAL JOIN Track;

SELECT ArtistName, count(ArtistName)
	FROM (SELECT ArtistId, Name AS ArtistName FROM Artist) AS ArtistMod JOIN Album ON ArtistMod.ArtistId = Album.ArtistId
	GROUP BY ArtistName;

SELECT Name, count(Name)
	FROM ARTIST NATURAL JOIN Album
	GROUP BY Name;

SELECT Name, count(*)
	FROM Artist NATURAL JOIN Album
    WHERE Name LIKE "%The%"
    GROUP BY Name;
    
-- having EXAMPLE
SELECT BillingCountry, GenreName
	FROM Invoice NATURAL JOIN InvoiceLine NATURAL JOIN Track NATURAL JOIN (SELECT GenreId, Name AS GenreName FROM Genre) AS GenreView
    GROUP BY BillingCountry, GenreName
    HAVING count(*) >= 10;
    
SELECT BillingCountry, GenreName, count(*)
	FROM Invoice NATURAL JOIN InvoiceLine NATURAL JOIN Track NATURAL JOIN (SELECT GenreId, Name AS GenreName FROM Genre) AS GenreView
    WHERE GenreName ="Latin"
	GROUP BY BillingCountry, GenreName;
-- needs groupby for aggrregating values and sum the genre amounts




    
    
    
    
    