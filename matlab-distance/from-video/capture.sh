videosnap -t 5 -p '1280x720' -d 'USB Camera #2' --no-audio r.mov &
videosnap -t 5 -p '1280x720' -d 'USB Camera' --no-audio l.mov

sleep 1
ffmpeg -i l.mov -vf "transpose=2" -c:a copy l2.mov -y
ffmpeg -i r.mov -vf "transpose=2" -c:a copy r2.mov -y 

mv l2.mov l.mov
mv r2.mov r.mov
