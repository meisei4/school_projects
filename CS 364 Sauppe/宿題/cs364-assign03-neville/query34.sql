SELECT Name AS ArtistName, count(Composer) AS ComposerCount
	FROM (SELECT DISTINCT Composer, Artist.Name
			FROM Artist NATURAL JOIN Album JOIN Track ON Album.AlbumId = Track.AlbumId
            WHERE Composer IS NOT NULL) AS Composers
	GROUP BY ArtistName