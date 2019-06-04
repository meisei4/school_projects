SELECT avg(AlbumLength) AS AverageAlbumLength
	FROM (SELECT Title, sum(Milliseconds) AS AlbumLength
			FROM Track NATURAL JOIN Album 
			GROUP BY Title) AS AlbumLengths	