%% Scan OCT images and extract the vessel 2D contours
%
%
% Author: Vignesh Rammohan
% Mathematical Modelling in Medicine Group
% Department of Infection, Immunity and Cardiovascular Disease
% The University of Sheffield
% Email: v.rammohan@sheffield.ac.uk
%
% Date: 02-Dec-2020; Last revision: 07-Dec-2020
%
% Copyright © 2020, The University of Sheffield
% All rights reserved.
%


clearvars; close all;

%% User input DICOM directory
dicomDir = uigetdir('*.*','Select the angiogram DICOM directory');

if dicomDir
    % get all files in the dicomDir
    dicomFiles = dir([dicomDir, '\**\*.*']); % also look into sub-directories
    nfiles = length(dicomFiles);
    if nfiles<1
        disp('No files available!')
        return
    end
else
    disp('No directory selected!')
    return
end


% initialise image paths and image holder variables
imagePaths = cell(nfiles,1);
images = cell(nfiles,1);
% initiate progress bar
hwait = waitbar(0,'Scanning images...',...
                'WindowStyle','modal');

% read image(DICOM) files 
for n=1:nfiles
    fname = dicomFiles(n).name;
    if ~dicomFiles(n).isdir && strcmp(fname(1:3),'IMG')
       fpath = [dicomFiles(n).folder '\' fname];
    else
        continue
    end

    % get all the valid images
    imagePaths{n} = fpath;
    images{n} = dicomread(imagePaths{n});
    images{n} = squeeze(images{n});
    waitbar(n/nfiles,hwait);
end

% remove empty fields
imagePaths = imagePaths(~cellfun('isempty',imagePaths));
images = images(~cellfun('isempty',images));
% if no image files found, display warning and return
if isempty(imagePaths)
    waitbar(1,hwait,'No angiograms spotted!!!');
    close(hwait)
    return
else
    % close(hwait)
end


nS = length(images); % number of slices
imgSz = size(images{1});
% Initialise storage for Red, Green and Blue component of pixels 
imgRcomp = zeros(imgSz(1),imgSz(1),nS);
imgGcomp = zeros(size(imgRcomp));
imgBcomp = zeros(size(imgRcomp));
% Extract the contours - Green pixels [0 255 0]
for n=1:nS
    imgRcomp(:,:,n) = images{n}(:,:,1);
    imgGcomp(:,:,n) = images{n}(:,:,2);
    imgBcomp(:,:,n) = images{n}(:,:,3);
    waitbar(n/nS,hwait,'Extracting components...');
end


grnPxRows = cell(nS,1);
grnPxCols = cell(nS,1);
for n=1:nS
    [grnPxRows{n},grnPxCols{n}] = find(imgGcomp(:,:,n) == 255 & ...
                            imgRcomp(:,:,n) == 0 & imgBcomp(:,:,n) == 0);
    waitbar(n/nS,hwait,'Extracting green pixels...');
end

close(hwait);

% remove empty fields
grnPxRows = grnPxRows(~cellfun('isempty',grnPxRows));
grnPxCols = grnPxCols(~cellfun('isempty',grnPxCols));


nvS = length(grnPxRows); % number of valid slices
grnPxRowsRd = cell(nvS,1);
grnPxColsRd = cell(nvS,1);
IDs = zeros(nvS,2);
for i=1:nvS
    grnPxColsRd{i} = find(grnPxCols{i}>181 & grnPxCols{i}<550);
    IdS = grnPxColsRd{i}(1,1); % start ID
    IdE = grnPxColsRd{i}(end,1); % end ID
    grnPxRowsRd{i} = grnPxRows{i}(IdS:IdE,1);
    IDs(i,:) = [IdS IdE];
end


% figure; hold on;
% idx = 50;
% IdS = grnPxColsRd{idx}(1,1); % start ID
% IdE = grnPxColsRd{idx}(end,1); % end ID
% z = ones(1130,1)*5;
% plot3(grnPxCols{idx}(IdS:IdE,1),-grnPxRows{idx}(IdS:IdE,1),z,'g.');xlim([0 704]);ylim([-704 0]);
% idx = 100;
% IdS = grnPxColsRd{idx}(1,1); % start ID
% IdE = grnPxColsRd{idx}(end,1); % end ID
% z = ones(1266,1)*10;
% plot3(grnPxCols{idx}(IdS:IdE,1),-grnPxRows{idx}(IdS:IdE,1),z,'g.');xlim([0 704]);ylim([-704 0]);


% figure; hold on;
% for i=1:50:nvS
%     x = grnPxCols{i}(IDs(i,1):IDs(i,2),1);
%     y = -grnPxRows{i}(IDs(i,1):IDs(i,2),1);
%     z = ones(size(x))*i;
%     plot3(x,y,z,'.','MarkerEdgeColor',[0.6500 0.3250 0.0980],'MarkerSize',1);xlim([0 704]);ylim([-704 0]);
% end


centroids = cell(nvS,1);
for i=1:nvS
    x = grnPxCols{i}(IDs(i,1):IDs(i,2),1);
    y = grnPxRows{i}(IDs(i,1):IDs(i,2),1);
    contour = zeros(704,704);
    contPts = [x y];
    contIdx = sub2ind(size(contour), contPts(:,1), contPts(:,2));
    contour(contIdx) = 1;

    %I = images{i};
    %ga = rgb2gray(I);
    BW = contour;
    %BW2 = bwpropfilt(BW,'EulerNumber',[1 1]);
    CONN = [ 0 1 0; 
            1 1 1;
            0 1 0];
    BW2 = bwareaopen(BW,15,CONN);
    %BW3 = bwareafilt(BW2,[15 200]);
    %CC = bwconncomp(BW2);
    %figure;imshowpair(I,BW2,'montage')

    prop = regionprops(BW2,'Centroid');
    centroids{i} = cat(1,prop.Centroid);
    %figure;imshow(BW2)
    %plot(centroids(:,2),-centroids(:,1),'b-')
    %sCent = sortrows(centroids,2);

    
%     x1=centroids(:,1)'; y1=centroids(:,2)';
%     xy = [x1;y1]; df = diff(xy,1,2); 
%     t = cumsum([0, sqrt([1 1]*(df.*df))]); 
%     cv = csapi(t,xy);
%     fnplt(cv), hold on, plot(x1,y1,'o'), hold off
end

centroids{343} = [];
centroids = centroids(~cellfun('isempty',centroids));

vertContours = cell(length(centroids),1);
for i=1:length(centroids)
    [k,av] = convhull(centroids{i});
    %figure; plot(centroids(k,1),centroids(k,2),'r-')
    vertContours{i} = [centroids{i}(k,1) centroids{i}(k,2)];
    x1 = vertContours{i}(:,1);
    y1 = vertContours{i}(:,2);
    z1 = ones(size(x1))*i;
    plot3(x1,y1,z1,'o-','MarkerEdgeColor',[0.6500 0.3250 0.0980],'MarkerSize',3)%;xlim([0 704]);ylim([-704 0]);
    hold on
end


for i=1:10%length(centroids)
    x1 = vertContours{i}(:,1);
    y1 = vertContours{i}(:,2);
    pp = spline(x1,y1);
    y2 = ppval(pp, linspace(0,2*pi,101));
    z1 = ones(size(x1))*i;
    plot3(x1,y2,z1,'o-','MarkerEdgeColor',[0.6500 0.3250 0.0980],'MarkerSize',3)%;xlim([0 704]);ylim([-704 0]);
    hold on
end

for i=1:10:length(vertContours)
    x1 = vertContours{i}(:,1);
    y1 = vertContours{i}(:,2);
    z1 = ones(size(x1))*i;
    plot3(x1,y1,z1,'o-','MarkerEdgeColor',[0.6500 0.3250 0.0980],'MarkerSize',3)%;xlim([0 704]);ylim([-704 0]);
    hold on
end
    