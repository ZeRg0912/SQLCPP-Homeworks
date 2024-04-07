-- Задание 1 Работа с заполнение таблиц
-- #1
INSERT INTO genres (name)
VALUES	('Pop'), 
		('Jazz'), 
		('Rock'), 
		('Classic');

INSERT INTO artists (name)
VALUES	('Pop_Artist'), 
		('Jazz_artist'), 
		('Rock_artist'), 
		('Classic_artist');
		
INSERT INTO albums(name ,year_of_release)
VALUES	('Pop_album_1', 2020),
		('Pop_album_2', 2007),
		('Pop_album_3', 1995),
		('Jazz_album_1', 1976),
		('Jazz_album_2', 2005),
		('Jazz_album_3', 2023),
		('Rock_album_1', 2024),
		('Rock_album_2', 2004),
		('Rock_album_3', 1996),
		('Classic_album_1', 1753),
		('Classic_album_2', 1749),
		('Classic_album_3', 1784);

INSERT INTO tracks(album_id, name, duration)
VALUES	((SELECT id FROM albums WHERE name = 'Pop_album_1'), 'Pop_track_1', 3.52),
		((SELECT id FROM albums WHERE name = 'Pop_album_2'), 'Pop_track_2', 3.11),
		((SELECT id FROM albums WHERE name = 'Pop_album_2'), 'Pop_track_3', 4.08),
		((SELECT id FROM albums WHERE name = 'Pop_album_3'), 'Pop_track_4', 3.21),
		((SELECT id FROM albums WHERE name = 'Jazz_album_1'), 'Jazz_track_1', 5.21),
		((SELECT id FROM albums WHERE name = 'Jazz_album_1'), 'Jazz_track_2', 3.45),
		((SELECT id FROM albums WHERE name = 'Jazz_album_2'), 'Jazz_track_3', 2.34),
		((SELECT id FROM albums WHERE name = 'Jazz_album_3'), 'Jazz_track_4', 7.21),
		((SELECT id FROM albums WHERE name = 'Rock_album_1'), 'Rock_track_1', 5.21),
		((SELECT id FROM albums WHERE name = 'Rock_album_2'), 'Rock_track_2', 4.32),
		((SELECT id FROM albums WHERE name = 'Rock_album_1'), 'Rock_track_3', 3.54),
		((SELECT id FROM albums WHERE name = 'Rock_album_3'), 'Rock_track_4', 4.53),
		((SELECT id FROM albums WHERE name = 'Classic_album_2'), 'Classic_track_1', 8.21),
		((SELECT id FROM albums WHERE name = 'Classic_album_2'), 'Classic_track_2', 6.42),
		((SELECT id FROM albums WHERE name = 'Classic_album_3'), 'Classic_track_3', 5.51),
		((SELECT id FROM albums WHERE name = 'Classic_album_1'), 'My_Classic_track_4', 3.21);

INSERT INTO broker_tracks_in_albums (album_id ,track_id)
SELECT albums.id, tracks.id FROM albums
JOIN tracks ON albums.id = tracks.album_id;

INSERT INTO broker_albums_of_artists (artist_id, album_id)
VALUES	((SELECT id FROM artists WHERE name = 'Pop_Artist'), (SELECT id FROM albums WHERE name = 'Pop_album_1')),
		((SELECT id FROM artists WHERE name = 'Pop_Artist'), (SELECT id FROM albums WHERE name = 'Pop_album_2')),
		((SELECT id FROM artists WHERE name = 'Pop_Artist'), (SELECT id FROM albums WHERE name = 'Pop_album_3')),
		((SELECT id FROM artists WHERE name = 'Jazz_artist'), (SELECT id FROM albums WHERE name = 'Jazz_album_1')),
		((SELECT id FROM artists WHERE name = 'Jazz_artist'), (SELECT id FROM albums WHERE name = 'Jazz_album_2')),
		((SELECT id FROM artists WHERE name = 'Jazz_artist'), (SELECT id FROM albums WHERE name = 'Jazz_album_3')),
		((SELECT id FROM artists WHERE name = 'Rock_artist'), (SELECT id FROM albums WHERE name = 'Rock_album_1')),
		((SELECT id FROM artists WHERE name = 'Rock_artist'), (SELECT id FROM albums WHERE name = 'Rock_album_2')),
		((SELECT id FROM artists WHERE name = 'Rock_artist'), (SELECT id FROM albums WHERE name = 'Rock_album_3')),
		((SELECT id FROM artists WHERE name = 'Classic_artist'), (SELECT id FROM albums WHERE name = 'Classic_album_1')),
		((SELECT id FROM artists WHERE name = 'Classic_artist'), (SELECT id FROM albums WHERE name = 'Classic_album_2')),
		((SELECT id FROM artists WHERE name = 'Classic_artist'), (SELECT id FROM albums WHERE name = 'Classic_album_3'));

INSERT INTO broker_artists_genres (artist_id, genre_id)
VALUES	((SELECT id FROM artists WHERE name = 'Pop_Artist'), (SELECT id FROM genres WHERE name = 'Pop')),
		((SELECT id FROM artists WHERE name = 'Pop_Artist'), (SELECT id FROM genres WHERE name = 'Rock')),
		((SELECT id FROM artists WHERE name = 'Jazz_artist'), (SELECT id FROM genres WHERE name = 'Jazz')),
		((SELECT id FROM artists WHERE name = 'Rock_artist'), (SELECT id FROM genres WHERE name = 'Rock')),
		((SELECT id FROM artists WHERE name = 'Classic_artist'), (SELECT id FROM genres WHERE name = 'Classic'));
		
INSERT INTO Collection (name, year_of_release, album_id, track_id)
SELECT DISTINCT 
    'Pop_collection' AS name,
    2022 AS year_of_release,
    a1.id AS album_id,
    t1.id AS track_id
FROM 
    albums a1
JOIN 
    tracks t1 ON a1.id = t1.album_id
WHERE 
    a1.name = 'Pop_album_2'
    AND t1.name IN ('Pop_track_2', 'Pop_track_3')

UNION ALL

SELECT DISTINCT
    'Jazz_collection' AS name,
    2018 AS year_of_release,
    a2.id AS album_id,
    t2.id AS track_id
FROM 
    albums a2
JOIN 
    tracks t2 ON a2.id = t2.album_id
WHERE 
    a2.name = 'Jazz_album_2'
    AND t2.name IN ('Jazz_track_3')

UNION ALL

SELECT DISTINCT
    'Jazz_collection' AS name,
    2018 AS year_of_release,
    a2.id AS album_id,
    t2.id AS track_id
FROM 
    albums a2
JOIN 
    tracks t2 ON a2.id = t2.album_id
WHERE 
    a2.name = 'Jazz_album_1'
    AND t2.name IN ('Jazz_track_1', 'Jazz_track_2')
    
UNION ALL

SELECT DISTINCT
    'Rock_collection' AS name,
    2005 AS year_of_release,
    a3.id AS album_id,
    t3.id AS track_id
FROM 
    albums a3
JOIN 
    tracks t3 ON a3.id = t3.album_id
WHERE 
    a3.name = 'Rock_album_1'
    AND t3.name IN ('Rock_track_1', 'Rock_track_3')
   
UNION ALL

SELECT DISTINCT
    'Rock_collection' AS name,
    2005 AS year_of_release,
    a3.id AS album_id,
    t3.id AS track_id
FROM 
    albums a3
JOIN 
    tracks t3 ON a3.id = t3.album_id
WHERE 
    a3.name = 'Rock_album_2'
    AND t3.name IN ('Rock_track_2')
   
UNION ALL

SELECT DISTINCT
    'Classic_collection' AS name,
    1746 AS year_of_release,
    a4.id AS album_id,
    t4.id AS track_id
FROM 
    albums a4
JOIN 
    tracks t4 ON a4.id = t4.album_id
WHERE 
    a4.name = 'Classic_album_2'
    AND t4.name IN ('Classic_track_1', 'Classic_track_2')
;


   
--DELETE FROM broker_tracks_in_albums;
--DELETE FROM tracks;
   
--DELETE FROM Collection
--WHERE album_id = (
--    SELECT id
--    FROM albums
--    WHERE name = 'Pop_album_1'
--);   