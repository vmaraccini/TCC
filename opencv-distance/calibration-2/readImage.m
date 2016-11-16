function I = readImage(file)
fileID = fopen(file,'r');
fscanf(fileID, '%c%c\n',2);
headerSpec= '%d %d\n%d';
header = fscanf(fileID,headerSpec);

formatSpec = '%c%c%c';
sizeA = [header(1) header(2) 3];
A = fscanf(fileID,formatSpec,sizeA);
end