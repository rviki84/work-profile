%% Angiogram parameter analysis - sanity test
%
% This script is currently being used to scan the angiograms from the
% COMPLETE study based in CANADA
%
% NOTE: This script includes the analysis part taken from SCAN_ANGIOGRAMS.m
% and COUNT_ANGIOGRAMS.m, the very first script created for this purpose.
% For the scanning part refer SCAN_ANGIOGRAMS_SAVE_RESULT.m script.
% Only run this script after running above.
%
% MODIFIED:
%   1. Added the 30 degree criteria based analysis code recently.
%   2. 30 degree criteria applied on all images, valid/invalid [03-July-2020]
%
% Author: Vignesh Rammohan
% Mathematical Modelling in Medicine Group
% Department of Infection, Immunity and Cardiovascular Disease
% The University of Sheffield
% Email: v.rammohan@sheffield.ac.uk
%
% Date: 19-May-2020; Last revision: 03-July-2020
%
% Copyright © 2020, The University of Sheffield
% All rights reserved.
%


clearvars; close all;

%% User input directory
scanResDir = uigetdir('*.*','Select the directory containing the scan result file');
if scanResDir
    resultFile = fullfile(scanResDir,'scan_results.mat');
    % look for result .mat file
    if isfile(resultFile)
        load(resultFile)
    else
        disp('Result file ''scan_results.mat'' not available!')
        return
    end
else
    disp('No directory selected!')
    return
end


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    %% Write result to the files - VALID AND INVALID CASES BASED ON ROTATION
    
% Criteria: At least one pair of images 30 degrees apart and all parameters
%           available.

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% open files to write on
fileValidID = fopen(fullfile(scanResDir, 'valid_1Pair_30degApart_OnALL.txt'),'w');
fileInValidID = fopen(fullfile(scanResDir, 'invalid_noPair_30degApart_OnALL.txt'),'w');

% Structure to hold the DICOM parameters' value (used for initialisation)
paramValInit = struct('ImgFile',''...
                    ,'Run',''...
                    ,'PositionerMotion',''...
                    ,'LAORAO',''...
                    ,'CauCra',''...
                    ,'distSD',''...
                    ,'distSP',''...
                    ,'ImgPixScal',''...
                    ,'ECG','');
                
% loop through the patient/case folders
for i=1:size(scanRes,2)
%     if ~scanRes(i).IsValid % read all the invalid marked cases
        k = 0; % counter for valid image sequence (having all parameters)
        paramVal = paramValInit; % initialise
        % loop through the image sequences
        for j=1:size(scanRes(i).HasDICOMparams,2)
            hasParam = scanRes(i).HasDICOMparams(j);
            % look for all essential parameters
            if strcmp(hasParam.Rotation,'Y')... 
                && strcmp(hasParam.Distance,'Y')...
                && strcmp(hasParam.PixelSpacing,'Y')...
                && strcmp(hasParam.ECG,'Y')
                k = k+1;
                % add to the struct
                paramVal(k) = scanRes(i).ImgParams(j);
            end
        end
        cnt30 = 0; % counter for valid pairs, 30 degrees apart
        if k > 1 % if there is at least a pair of images with all parameters
            for m =1:size(paramVal,2)-1
                for n = m+1:size(paramVal,2)
                    if abs(paramVal(m).LAORAO - paramVal(n).LAORAO) >= 30 ||...
                            abs(paramVal(m).CauCra - paramVal(n).CauCra) >= 30
                        cnt30 = cnt30 + 1;
                    end
                end
            end
        end
        % if there exist at least a single pair 30 degrees apart
        if cnt30 > 0
            fprintf(fileValidID,'%s',scanRes(i).patient);
            fprintf(fileValidID,' - %d\n',cnt30);
        else
            fprintf(fileInValidID,'%s\n',scanRes(i).patient);
        end
%     end
end
% close all files
fclose(fileValidID);
fclose(fileInValidID);


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
        %% Write result to the files - VALID AND INVALID CASES
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% open files to write on
fileValidID = fopen(fullfile(scanResDir, 'valid_cases.txt'),'w');
num = sum([scanRes.IsValid]);
fprintf(fileValidID,'No. of valid cases: %d\n\n',num);

fileInValidID = fopen(fullfile(scanResDir, 'invalid_cases.txt'),'w');
fprintf(fileInValidID,'No. of invalid cases: %d\n\n',length(scanRes)-num);

% loop through the patient/case folders
for i=1:size(scanRes,2)
    if scanRes(i).IsValid
        % valid cases goes here
        fprintf(fileValidID,'%s\n',scanRes(i).patient);
    else
        % invalid cases goes here
        fprintf(fileInValidID,'%s\n',scanRes(i).patient);
        % loop through the image sequences
        for j=1:size(scanRes(i).HasDICOMparams,2)
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
                line = [hasParam.ImgFile,': ',line(1:end-1)];
                fprintf(fileInValidID,'\t%s\n',line);
            end
        end
    end
end
% close all files
fclose(fileValidID);
fclose(fileInValidID);


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    %% Write result to the files - VALID AND INVALID CASES WITH IMAGE COUNT
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% open files to write on
fileValidID = fopen(fullfile(scanResDir, 'valid_cases_ImgCount.txt'),'w');
num = sum([scanRes.IsValid]);
fprintf(fileValidID,'No. of valid cases: %d\n\n',num);

fileInValidID = fopen(fullfile(scanResDir, 'invalid_cases_ImgCount.txt'),'w');
fprintf(fileInValidID,'No. of invalid cases: %d\n\n',length(scanRes)-num);

% loop through the patient/case folders
pcnt10 = 0; pcnt25 = 0; pcnt50 = 0; pcnt98 = 0; % invalid image percentage case count 
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
        if percent <= 98; pcnt98 = pcnt98 + 1; end
    end
end

% invalid image percentage case statistics
fprintf(fileInValidID,'\n\n%s\n','--- Invalid image percentage case count ---');
fprintf(fileInValidID,'%s\n',['<=10% invalid images: ' num2str(pcnt10)]);
fprintf(fileInValidID,'%s\n',['<=25% invalid images: ' num2str(pcnt25)]);
fprintf(fileInValidID,'%s\n',['<=50% invalid images: ' num2str(pcnt50)]);
fprintf(fileInValidID,'%s\n',['<=98% invalid images: ' num2str(pcnt98)]);

% close all files
fclose(fileValidID);
fclose(fileInValidID);

