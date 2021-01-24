%% Count angiograms as part of the sanity test
%
% This script is currently being used to scan the angiograms from the
% COMPLETE study based in CANADA
%
% Author: Vignesh Rammohan
% Mathematical Modelling in Medicine Group
% Department of Infection, Immunity and Cardiovascular Disease
% The University of Sheffield
% Email: v.rammohan@sheffield.ac.uk
%
% Date: 23-Mar-2020; Last revision: 23-Mar-2020
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

    if hasAllParams
        scanRes(nf).IsValid = 1;
    end
    
end



%% Write result to the files
% open files to write on
fileValidID = fopen(fullfile(scanDir, 'valid_cases_ImgCount.txt'),'w');
num = sum([scanRes.IsValid]);
fprintf(fileValidID,'No. of valid cases: %d\n\n',num);

fileInValidID = fopen(fullfile(scanDir, 'invalid_cases_ImgCount.txt'),'w');
fprintf(fileInValidID,'No. of invalid cases: %d\n\n',length(scanRes)-num);

% loop through the patient/case folders
pcnt10 = 0; pcnt25 = 0; pcnt50 = 0; % invalid image percentage case count 
for i=1:size(scanRes,2)
    numImgs = size(scanRes(i).HasDICOMparams,2); % total number of available images/sequences
    linecase = [];
    if scanRes(i).IsValid   % valid case goes here
        fprintf(fileValidID,'%s\n',scanRes(i).patient);
        linecase = strcat(linecase,' - Img. Count:',num2str(numImgs));
        fprintf(fileValidID,'\t%s\n',linecase);
    else    % invalid case goes here
%         fprintf(fileInValidID,'%s\n',scanRes(i).patient);
        % loop through the image sequences
        cntInvImgs = 0; % invalid image count
        for j=1:numImgs
            line = [];
            hasParam = scanRes(i).HasDICOMparams(j);
            if strcmp(hasParam.RUN,'-N-')
                line = strcat(line,'RUN,');
            end
            if strcmp(hasParam.Rotation,'-N-')
                line = strcat(line,'Rotation,');
            end
            if strcmp(hasParam.Distance,'-N-')
                line = strcat(line,'Distance,');
            end
            if strcmp(hasParam.PixelSpacing,'-N-')
                line = strcat(line,'PixelSpacing,');
            end
            if strcmp(hasParam.ECG,'-N-')
                line = strcat(line,'ECG,');
            end
            if ~isempty(line)
                cntInvImgs = cntInvImgs + 1;
%                 line = [hasParam.ImgFile,': ',line(1:end-1)];
%                 fprintf(fileInValidID,'\t%s\n',line);
            end
        end
        percent = round((cntInvImgs/numImgs)*100);
        fprintf(fileInValidID,'%s\n',scanRes(i).patient);
        linecase = strcat(linecase,' - Invalid Img. Count out of Total (%invalid):'...
            ,num2str(cntInvImgs),'/',num2str(numImgs)...
            ,' (',num2str(percent),'%)');
        fprintf(fileInValidID,'\t%s\n',linecase);
        if percent <= 10; pcnt10 = pcnt10 + 1; end
        if percent <= 25; pcnt25 = pcnt25 + 1; end
        if percent <= 50; pcnt50 = pcnt50 + 1; end   
    end
end

% invalid image percentage case statistics
fprintf(fileInValidID,'\n\n%s\n','--- Invalid image percentage case count ---');
fprintf(fileInValidID,'%s\n',['<=10% invalid images: ' num2str(pcnt10)]);
fprintf(fileInValidID,'%s\n',['<=25% invalid images: ' num2str(pcnt25)]);
fprintf(fileInValidID,'%s\n',['<=50% invalid images: ' num2str(pcnt50)]);

% close all files
fclose(fileValidID);
fclose(fileInValidID);

toc % Read elapsed time from stopwatch

