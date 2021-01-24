%% Display angiograms and corresponding ECG data from the input DICOM directory
%
% Author: Vignesh Rammohan
% Mathematical Modelling in Medicine Group
% Department of Infection, Immunity and Cardiovascular Disease
% The University of Sheffield
% Email: v.rammohan@sheffield.ac.uk
%
% Date: 18-Sep-2019; Last revision: 18-Nov-2019
%
% Copyright © 2019, The University of Sheffield
% All rights reserved.
%


clearvars; close all;

%% Global control parameters
% choose the image slice, 1 to max. slice number (size(image,3))
imgSliceNum = randi(20,1); % random slice number


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
    if strcmp(fname,'DICOMDIR')
        continue
    end
    try
       fpath = [dicomFiles(n).folder '\' fname];
       metadata = dicominfo(fpath, 'UseDictionaryVR', true);
       imgType = metadata.ImageType;
    catch ME
%         disp(ME.message)
%         disp(ME.identifier)
%         for i=1:length(ME.stack)
%             disp(ME.stack(i))
%         end
%         msg = msgbox({fname ' file is NOT a DICOM!'}...
%                         ,'Input file error','error');
        continue
    end

    % get all the valid images
    imagePaths{n} = fpath;
    images{n} = dicomread(imagePaths{n});
    images{n} = squeeze(images{n});
end

% remove empty fields
imagePaths = imagePaths(~cellfun('isempty',imagePaths));
images = images(~cellfun('isempty',images));
% if no image files found, display warning and return
if isempty(imagePaths)
    waitbar(1,hwait,'No angiograms spotted!!!');
    close(hwait)
    return
end


%% Get all required image parameters
imgParams = struct([]); % structure to hold the DICOM parameters' value
numImgs = length(images); % total number of available images
try
    waitbar(.2,hwait,'Preparing images...');
catch
    waitbar(1,hwait,'Image loading cancelled!');
    close(hwait)
    return
end

for n=1:numImgs
    metadata = dicominfo(imagePaths{n}, 'UseDictionaryVR', true);
    Run = metadata.SeriesNumber;
    % if no series number available get the instance number
    if isempty(Run)
        Run = metadata.InstanceNumber;
    end
    imgParams{n}.Run = Run;
    try 
        % get C-arm imaging device rotations
        imgParams{n}.PositionerMotion = metadata.PositionerMotion;
        imgParams{n}.LAORAO = metadata.PositionerPrimaryAngle;
        imgParams{n}.CauCra = metadata.PositionerSecondaryAngle;
    catch
        imgParams{n}.Rotations = 'Not Available';
    end
    
    try
        % get C-arm imaging device distances
        imgParams{n}.distSD = metadata.DistanceSourceToDetector;
        imgParams{n}.distSP = metadata.DistanceSourceToPatient;
    catch
        imgParams{n}.Distances = 'Not Available';
    end
    
    try
        % look for Imager Pixel Spacing
        imgParams{n}.ImgPixScal = metadata.ImagerPixelSpacing; 
    catch
        imgParams{n}.ImgPixScal = 'Not Available';
    end

    if imgSliceNum > size(images{n},3)
        imgSliceNum = floor(size(images{n},3)/2);
        if imgSliceNum == 0; imgSliceNum=1; end
    end

    % if positioner motion is 'DYNAMIC' modify the rotation angles
    if exist('metadata.PositionerMotion', 'var') && strcmp(metadata.PositionerMotion,'DYNAMIC')
        imgParams{n}.PositionerMotion = metadata.PositionerMotion;
        imgParams{n}.LAORAO = metadata.PositionerPrimaryAngle+sum(metadata.PositionerPrimaryAngleIncrement(1:imgSliceNum));
        imgParams{n}.CauCra = metadata.PositionerSecondaryAngle+sum(metadata.PositionerSecondaryAngleIncrement(1:imgSliceNum));
    end

end


%% Display all the images and ECG curve data
numRows = ceil(numImgs/4);
imgFigH = figure; ECGFigH = figure;
for n=1:numImgs
    len = 0.4 + 0.6*n/numImgs;
    try
        waitbar(len,hwait,'Loading images...');
    catch
        waitbar(1,hwait,'Image loading cancelled!');
        close(hwait)
        return
    end
    
    if imgSliceNum > size(images{n},3)
        imgSliceNum = floor(size(images{n},3)/2);
    end
    
    figure(imgFigH)
    axImg = subplot(numRows,4,n); % image plots
    imageSlice = imadjust(images{n}(:,:,imgSliceNum));
    %xlim(axImg,[0 124]); ylim(axImg,[0 124])
    imshow(imageSlice,'InitialMagnification',200);
    colormap(gray);
    title(axImg,['Run: ',num2str(imgParams{n}.Run),' | Slice: ',num2str(imgSliceNum)]);

    % get the ECG curve if available and display it
    metadata = dicominfo(imagePaths{n}, 'UseDictionaryVR', true);
    hasECG = isfield(metadata, 'CurveData_0');
    if hasECG
        imgParams{n}.ECG = 'Available';
        ECG = metadata.CurveData_0;
        ECG = im2double(ECG);
        if size(ECG,1)<21; continue; end
        ECG = sgolayfilt(ECG,7,21);
        M = size(images{n},3);
        N = size(ECG,1);
        k = round(imgSliceNum*(N/M));
        figure(ECGFigH)
        axECG = subplot(numRows,4,n); % ECG plots   
        plot(axECG,ECG,'Color',[0.2 1.0 0.2]);
        hold on
        title(axECG,['Run: ',num2str(imgParams{n}.Run)]);
        set(gca,'Color',[0.5 0.5 0.5]); 
        plot(axECG,k,ECG(k),'*r','MarkerSize',7);
    else
        imgParams{n}.ECG = 'Not Available';
    end
    
end
close(hwait)

