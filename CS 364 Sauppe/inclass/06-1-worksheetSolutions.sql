SELECT BillingCountry, count(BillingCountry)
	FROM Invoice NATURAL JOIN InvoiceLine
    GROUP BY BillingCountry
    ORDER BY count(BillingCountry) DESC
    LIMIT 1;
    
SELECT DISTINCT BillingCountry, count(BillingCountry)
	FROM Invoice NATURAL JOIN InvoiceLine
    GROUP BY BillingCountry
    HAVING count(*) >= 100
    ORDER BY count(BillingCountry) DESC;
    
SELECT ArtistName, Name
	FROM (SELECT ArtistId, Name AS ArtistName FROM Artist) AS ArtistMod JOIN Album ON ArtistMod.ArtistId = Album.ArtistId
		JOIN Track ON Track.AlbumId = Album.AlbumId;
        
SELECT *
	FROM Artist NATURAL JOIN Album NATURAL JOIN Track;
    
SELECT ArtistName, count(ArtistName)
	FROM (SELECT ArtistId, Name AS ArtistName FROM Artist) AS ArtistMod JOIN Album ON ArtistMod.ArtistId = Album.ArtistId
		JOIN Track ON Track.AlbumId = Album.AlbumId
	GROUP BY ArtistName;

SELECT Name, count(Name)
	FROM Artist NATURAL JOIN Album
    GROUP BY Name;
    
SELECT Name, count(*)
	FROM Artist NATURAL JOIN Album
    WHERE Name LIKE "%The%"    -- match "The Black-Eyed Peas" "The The" "Name The"
    GROUP BY Name;
