%% Scan angiograms for essential parameters used in the segmentation tool
%%   Store and save the results for further analysis.
%
% This script is currently being used to scan the angiograms from the
% COMPLETE study based in CANADA
%
% NOTE: This script is extracted from SCAN_ANGIOGRAMS.m, the very first
% script created for this purpose. As the scanning part takes a while it is
% extracted here and the results are saved to be used for any further
% analysis, avoiding re-scan, thereby saving time.
%
% MODIFIED:
%   Also the image parameter values (imgParams) are saved inside 'scanRes'
%   struct.
%
% Author: Vignesh Rammohan
% Mathematical Modelling in Medicine Group
% Department of Infection, Immunity and Cardiovascular Disease
% The University of Sheffield
% Email: v.rammohan@sheffield.ac.uk
%
% Date: 15-May-2020; Last revision: 15-May-2020
%
% Copyright © 2020, The University of Sheffield
% All rights reserved.
%


clearvars; close all;

tic % Start stopwatch timer

%% User input directory
scanDir = uigetdir('*.*','Select the directory to scan all angiograms cases');
if scanDir
    % get all files/folders in the scanDir
    dirFiles = dir(scanDir); % look into the directory
    dirFiles = dirFiles([dirFiles.isdir]); % remove files
    nfolder = length(dirFiles);
    if nfolder<1
        disp('No folder available!')
        return
    end
else
    disp('No directory selected!')
    return
end



%% Fetch all the valid folders under the chosen directory; ignore '.' and '..'
fdPaths = cell(nfolder,1);
for nf=3:nfolder % '.' and '..' are removed
    fdPaths{nf} = fullfile(dirFiles(nf).folder, dirFiles(nf).name);
end
% remove empty fields and update number of valid folders
fdPaths = fdPaths(~cellfun('isempty',fdPaths));
nfolder = size(fdPaths,1);
if nfolder<1
    disp('No folder available!')
    return
end


%% Initialise structure variable to hold the result
scanRes =  struct('patient',{}...
                    ,'HasDICOMparams',struct([])...
                    ,'ImgParams',struct([])...
                    ,'IsValid',0);

                
%% Scan each case folder for angiograms
parfor nf=1:nfolder % parallel processing; uncomment to put in action
% for nf=1:nfolder      % comment this line when above is active
    % get all files in the each folder
    disp(['Scanning ' fdPaths{nf}])
    fdparts = regexp(fdPaths{nf},filesep,'split');
    scanRes(nf).patient = fdparts{end};
    scanRes(nf).IsValid = 0;
    dicomFiles = dir([fdPaths{nf},filesep,'**',filesep,'*.*']); % also look into sub-directories
    nfiles = length(dicomFiles);

    % initialise image paths and image holder variables
    imagePaths = cell(nfiles,1);
%     images = cell(nfiles,1);
    % initiate progress bar
    hwait = waitbar(0,'Scanning for images...','WindowStyle','modal');

    % read image(DICOM) files 
    for n=1:nfiles
        fname = dicomFiles(n).name;
        if strcmp(fname,'DICOMDIR') % ignore DICOMDIR file
            continue
        end
        try
           fpath = fullfile(dicomFiles(n).folder, fname);
           metadata = dicominfo(fpath, 'UseDictionaryVR', true);
           imgType = metadata.ImageType;
        catch % if not DICOM/Image file proceed to next
            continue
        end

        % store valid image path and image
        imagePaths{n} = fpath;
%         images{n} = dicomread(imagePaths{n});
%         images{n} = squeeze(images{n});
    end

    % remove empty fields
    imagePaths = imagePaths(~cellfun('isempty',imagePaths));
%     images = images(~cellfun('isempty',images));
    % if no image files found, display warning and continue
    if isempty(imagePaths)
        waitbar(1,hwait,'No angiograms spotted!!!');
        close(hwait)
%         scanRes(nf)=[];
        continue
    end

    
    % structure to store DICOM parameters' availability('Y'/'-N-') as a
    % field in the above structure(scanRes)
    % Note: This is just for reference, so commented; replaced below by a empty struct
%     hasDcmParam = struct('ImgFile',''...
%                         ,'RUN',''...
%                         ,'Rotation',''...
%                         ,'Distance',''...
%                         ,'PixelSpacing',''...
%                         ,'ECG','');

    % structure to hold the DICOM parameters' value
    imgParams = struct('ImgFile',''...
                        ,'Run',''...
                        ,'PositionerMotion',''...
                        ,'LAORAO',''...
                        ,'CauCra',''...
                        ,'distSD',''...
                        ,'distSP',''...
                        ,'ImgPixScal',''...
                        ,'ECG','');

    %% Get all required image parameters
    numImgs = length(imagePaths); % total number of available images
    waitbar(.2,hwait,'Reading image parameters...');
    disp(['Number of images: ' num2str(numImgs)])
    hasAllParams = 1;
    hasDcmParam = struct([]);
    for n=1:numImgs
        % exception for Philips medical device corresponding to
        % Rotation,Distance,PixelSpacing,ECG checks 
        philips = [0 0 0 0];
        % get DICOM header data
        metadata = dicominfo(imagePaths{n}, 'UseDictionaryVR', true);
        fdparts = regexp(imagePaths{n},filesep,'split');
        hasDcmParam(n).ImgFile = fdparts{end};
        imgParams(n).ImgFile = fdparts{end};
        % ~~~~~ get the image sequence(RUN) ~~~~~ 
        if isfield(metadata,'SeriesNumber')
            Run = metadata.SeriesNumber;
            % if no series number available get the instance number
            if isempty(Run) && isfield(metadata,'InstanceNumber')
                Run = metadata.InstanceNumber;
            end
            imgParams(n).Run = Run;
            hasDcmParam(n).RUN = 'Y';
        elseif isfield(metadata,'InstanceNumber')
            Run = metadata.InstanceNumber;
            imgParams(n).Run = Run;
            hasDcmParam(n).RUN = 'Y';
        else
            hasDcmParam(n).RUN = '-N-';
            hasAllParams = 0;
        end
        
        try 
            % ~~~~~ get C-arm imaging device rotations ~~~~~ 
%             imgParams(n).PositionerMotion = metadata.PositionerMotion;
            imgParams(n).LAORAO = metadata.PositionerPrimaryAngle;
            imgParams(n).CauCra = metadata.PositionerSecondaryAngle;
            hasDcmParam(n).Rotation = 'Y';
        catch
            if strcmp(metadata.ImageType,'DERIVED\SECONDARY') % QCA image
                imgParams(n).LAORAO = 'NA';
                imgParams(n).CauCra = 'NA';
                hasDcmParam(n).Rotation = 'NA';
                philips(1)=1;
            else
                hasDcmParam(n).Rotation = '-N-';
                hasAllParams = 0;
            end
        end
%         % if positioner motion is 'DYNAMIC' modify the rotation angles
%         if isfield(metadata,'PositionerMotion') && strcmp(metadata.PositionerMotion,'DYNAMIC')
%             image = dicomread(imagePaths{n});
%             image = squeeze(image);
%             imgSliceNum = floor(size(image,3)/2);
%             imgParams(n).PositionerMotion = metadata.PositionerMotion;
%             imgParams(n).LAORAO = metadata.PositionerPrimaryAngle...
%                 +sum(metadata.PositionerPrimaryAngleIncrement(1:imgSliceNum));
%             imgParams(n).CauCra = metadata.PositionerSecondaryAngle...
%                 +sum(metadata.PositionerSecondaryAngleIncrement(1:imgSliceNum));
%         end

        try
            % ~~~~~ get C-arm imaging device distances ~~~~~ 
            imgParams(n).distSD = metadata.DistanceSourceToDetector;
            imgParams(n).distSP = metadata.DistanceSourceToPatient;
            hasDcmParam(n).Distance = 'Y';
        catch
            if sum(philips)==1 && strcmp(metadata.ImageType,'DERIVED\SECONDARY') % QCA image
                imgParams(n).distSD = 'NA';
                imgParams(n).distSP = 'NA';
                hasDcmParam(n).Distance = 'NA';
                philips(2)=1;
            else
                hasDcmParam(n).Distance = '-N-';
                hasAllParams = 0;
            end
        end

        try
            % ~~~~~ look for Imager Pixel Spacing ~~~~~ 
            imgParams(n).ImgPixScal = metadata.ImagerPixelSpacing;
            hasDcmParam(n).PixelSpacing = 'Y';
        catch
            if sum(philips)==2 && strcmp(metadata.ImageType,'DERIVED\SECONDARY') % QCA image
                imgParams(n).ImgPixScal = 'NA';
                hasDcmParam(n).PixelSpacing = 'NA';
                philips(3)=1;
            else
                hasDcmParam(n).PixelSpacing = '-N-';
                hasAllParams = 0;
            end
        end
            
        
        % ~~~~~ check for ECG curve ~~~~~ 
        if isfield(metadata, 'CurveData_0')
            imgParams(n).ECG = 'Available';
            hasDcmParam(n).ECG = 'Y';
        elseif contains(metadata.ImageType,'fluo','IgnoreCase',true) % Fluoroscopy image in Toshiba
            imgParams(n).ECG = 'NA';
            hasDcmParam(n).ECG = 'NA';
        elseif isfield(metadata,'SeriesDescription') &&... 
            contains(metadata.SeriesDescription,'fluo','IgnoreCase',true) % Fluoroscopy image in Philips
            imgParams(n).ECG = 'NA';
            hasDcmParam(n).ECG = 'NA';
        elseif sum(philips)==3 && strcmp(metadata.ImageType,'DERIVED\SECONDARY') % QCA image
            imgParams(n).ECG = 'NA';
            hasDcmParam(n).ECG = 'NA';
            philips(4)=1;
        else
            hasDcmParam(n).ECG = '-N-';
            imgParams(n).ECG = 'Not Available';
            hasAllParams = 0;   
        end
        
        % store the result for current DICOM file scan
%         scanRes(nf).HasDICOMparams(n) = hasDcmParam(n);
        
        % update progress bar
        len = 0.2 + 0.8*n/numImgs;
        waitbar(len,hwait,'Reading image parameters...');
        % display image parameters
%         disp(['Parameters for image: ' imagePaths{n}])
%         disp(imgParams(n))
    end
    
    close(hwait)
    % store the result for current patient case
    scanRes(nf).HasDICOMparams = hasDcmParam;
    scanRes(nf).ImgParams = imgParams;

    if hasAllParams
        scanRes(nf).IsValid = 1;
    end
    
end


%% Save results
filename = fullfile(scanDir,'scan_results.mat');
save(filename)

toc % Read elapsed time from stopwatch

