SELECT Album.Title, Track.Name
FROM Album
INNER JOIN Chinook.Track ON Album.AlbumId = Track.AlbumId