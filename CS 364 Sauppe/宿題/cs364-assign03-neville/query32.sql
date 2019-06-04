SELECT Artist.Name AS ArtistName, sum(Milliseconds/1000) AS TotalTrackLength
	FROM Artist NATURAL JOIN Album JOIN Track ON Album.AlbumId = Track.AlbumId
	GROUP BY ArtistName 
	ORDER BY sum(Milliseconds) DESC