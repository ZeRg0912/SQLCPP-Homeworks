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