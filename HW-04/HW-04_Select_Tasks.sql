-- HW-02 Создание таблиц
CREATE TABLE IF NOT EXISTS genres (
	id SERIAL PRIMARY KEY,
	name VARCHAR(60) NOT NULL
);

CREATE TABLE IF NOT EXISTS artists (
	id SERIAL PRIMARY KEY,
	name VARCHAR(60) NOT NULL	
);

CREATE TABLE IF NOT EXISTS albums (
	id SERIAL PRIMARY KEY,
	name VARCHAR(60) NOT NULL,
	year_of_release INTEGER NOT NULL
);

CREATE TABLE IF NOT EXISTS tracks (
	id SERIAL PRIMARY KEY,
	album_id INTEGER NOT NULL REFERENCES albums(id),
	name VARCHAR(60) NOT NULL,
	duration FLOAT NOT NULL
);

CREATE TABLE IF NOT EXISTS broker_artists_genres (
	genre_id INTEGER NOT NULL REFERENCES genres(id),	
	artist_id INTEGER NOT NULL REFERENCES artists(id)
);

CREATE TABLE IF NOT EXISTS broker_albums_of_artists (
	artist_id INTEGER NOT NULL REFERENCES artists(id),	
	album_id INTEGER NOT NULL REFERENCES albums(id)
);

CREATE TABLE IF NOT EXISTS broker_tracks_in_albums (
	album_id INTEGER NOT NULL REFERENCES albums(id),	
	track_id INTEGER NOT NULL REFERENCES tracks(id)
);

CREATE TABLE IF NOT EXISTS collection (
	id SERIAL PRIMARY KEY,
	name VARCHAR(60) NOT NULL,
	year_of_release INTEGER NOT NULL,
	album_id INTEGER NOT NULL REFERENCES albums(id),	
	track_id INTEGER NOT NULL REFERENCES tracks(id)
);


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
   
-- Задание 2
-- #1 Название и продолжительность самого длительного трека.
SELECT name, duration FROM tracks
WHERE duration = (SELECT MAX (duration) FROM tracks)
ORDER BY name;

-- #2 Название треков, продолжительность которых не менее 3,5 минут.
SELECT name, duration FROM tracks
WHERE duration >= 3.5
ORDER BY duration ASC;

-- #3 Названия сборников, вышедших в период с 2018 по 2020 годы включительно.
SELECT name, year_of_release FROM collection
WHERE year_of_release BETWEEN  2018 AND  2020
ORDER BY name;

-- #4 Исполнители, чьё имя состоит из одного слова.
SELECT name FROM artists
WHERE name NOT LIKE '% %';

-- #5 Название треков, которые содержат слова «мой» или «my».
SELECT name FROM tracks
WHERE name LIKE '%My%' OR name LIKE '%мой%';

-- Задание 3
-- #1 Количество исполнителей в каждом жанре.
SELECT g.name AS genre, COUNT(DISTINCT ag.artist_id) AS artist_count FROM genres g
LEFT JOIN broker_artists_genres ag ON g.id = ag.genre_id
GROUP BY g.name;

-- #2 Количество треков, вошедших в альбомы 2019–2020 годов.
SELECT COUNT(*) AS track_count FROM tracks t
JOIN broker_tracks_in_albums t_in_a ON t.id = t_in_a.track_id 
JOIN albums ON t_in_a.album_id = albums.id 
WHERE albums.year_of_release BETWEEN 2019 AND 2020

-- #3 Средняя продолжительность треков по каждому альбому.
SELECT a.name AS album_name, AVG(t.duration) AS average_duration FROM albums a
JOIN broker_tracks_in_albums AS t_in_a ON a.id = t_in_a.album_id
JOIN tracks AS t ON t_in_a.track_id = t.id
GROUP BY a.name;

-- #4 Все исполнители, которые не выпустили альбомы в 2020 году.
SELECT name AS artist_name FROM artists
WHERE id NOT IN (
	SELECT DISTINCT artist_id FROM broker_albums_of_artists AS a_of_a
	JOIN albums ON a_of_a.album_id = albums.id 
	WHERE albums.year_of_release = 2020
);

-- #5 Названия сборников, в которых присутствует конкретный исполнитель (выберите его сами).
SELECT DISTINCT c.name AS collection_name FROM Collection c
JOIN tracks t ON c.track_id = t.id
JOIN albums a ON c.album_id = a.id
JOIN broker_albums_of_artists a_of_a ON a.id = a_of_a.album_id
JOIN artists ON a_of_a.artist_id = artists.id
WHERE artists.name = 'Jazz_artist';

-- Задание 4
-- #1 Названия альбомов, в которых присутствуют исполнители более чем одного жанра.
SELECT DISTINCT a.name AS album_name FROM albums a
JOIN broker_albums_of_artists AS a_of_a ON a.id = a_of_a.album_id
JOIN artists ON a_of_a.artist_id = artists.id
JOIN broker_artists_genres AS a_g ON artists.id = a_g.artist_id
GROUP BY a.name
HAVING COUNT(DISTINCT a_g.genre_id) > 1;

-- #2 Наименования треков, которые не входят в сборники.
SELECT t.name AS track_name FROM tracks t
LEFT JOIN collection c ON t.id = c.track_id
WHERE c.id IS NULL;

-- #3 Исполнитель или исполнители, написавшие самый короткий по продолжительности трек, 
--    — теоретически таких треков может быть несколько.
SELECT a.name AS artist_name, t.name AS track_name, t.duration AS track_duration
FROM artists a
JOIN broker_albums_of_artists ba ON a.id = ba.artist_id
JOIN albums al ON ba.album_id = al.id
JOIN broker_tracks_in_albums ta ON al.id = ta.album_id
JOIN tracks t ON ta.track_id = t.id
WHERE t.duration IS NOT NULL
ORDER BY t.duration ASC
LIMIT 1;

-- #4 Названия альбомов, содержащих наименьшее количество треков.
SELECT name AS album_name FROM albums
WHERE id IN (
    SELECT album_id FROM broker_tracks_in_albums
    GROUP BY album_id
    HAVING COUNT(*) = (
        SELECT MIN(track_count)FROM (
            SELECT COUNT(*) AS track_count
            FROM broker_tracks_in_albums
            GROUP BY album_id
        ) AS track_counts
    )
);