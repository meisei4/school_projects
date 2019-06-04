SELECT Track.Name, count(Track.Name) AS Appear
	FROM Playlist NATURAL JOIN PlaylistTrack JOIN Track ON Track.TrackId = PlaylistTrack.TrackId
	GROUP BY Track.Name
	HAVING Appear >= 5 AND Track.Name LIKE "%The%"