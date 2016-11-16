mkdir l
mkdir r

ffmpeg -i r.mov -r 3 r/$filename%03d.bmp
ffmpeg -i l.mov -r 3 l/$filename%03d.bmp

sips -r 270 r/*
sips -r 270 l/*

cp r/001.bmp r.bmp
cp l/001.bmp l.bmp
