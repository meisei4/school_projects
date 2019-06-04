SELECT Album.Title AS Album, Track.Name AS TrackName
FROM Chinook.Album
LEFT JOIN Chinook.Track ON Track.Name = Album.Title