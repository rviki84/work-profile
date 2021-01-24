%% Script to extract Pa and Pd pressures from respective files and compute their average value
%
% Author: Vignesh Rammohan
% Mathematical Modelling in Medicine Group
% Department of Infection, Immunity and Cardiovascular Disease
% The University of Sheffield
% Email: v.rammohan@sheffield.ac.uk
%
% Date: 28-Jan-2020; Last revision: 30-Jan-2020
%
% Copyright © 2019, The University of Sheffield
% All rights reserved.
%


clearvars; close all;


%% User input 'pressure files' root directory
rootDir = uigetdir('*.*','Select the pressure files root directory');

if rootDir
    % get all files in the rootDir
    txtFiles = dir([rootDir, '\**\*.*']); % also look into sub-directories
    nfiles = length(txtFiles);
    if nfiles<1
        disp('No files available!')
        return
    end
else
    disp('No directory selected!')
    return
end


%% Scan for valid files
filePaths = cell(nfiles,1); % initialise file paths
% get pressure file paths
for n=1:nfiles
    fname = txtFiles(n).name;
    if ~contains(fname,'.txt')
        continue
    end
    
    if strcmp(fname,'AllTracesAndAnalysisInfo.txt')
        continue
    end

    fpath = fullfile(txtFiles(n).folder, fname);

    % get all the valid files
    filePaths{n} = fpath;
end

% remove empty fields
filePaths = filePaths(~cellfun('isempty',filePaths));


%% Read file content; if not empty store it in result
formatSpec = '%f %f'; % pattern to read the file, float values in 2 columns
sizeTable = [2 Inf]; % column order to store the read output
len = length(filePaths);
fsInd = strfind(rootDir,filesep); % get file separator indices
result = cell(len,2);
for i=1:len
    freadID = fopen(filePaths{i},'r');
    content = fscanf(freadID,formatSpec,sizeTable);
    fclose(freadID);
    if ~isempty(content)
        result{i,1} = filePaths{i}(fsInd(end)+1:end);
        result{i,2} = mean(content(2,:)); % compute average and store
    end
end
% remove empty fields
result = result'; % transpose just for the reshape to work properly
result = reshape(result(~cellfun('isempty',result)),2,[]);


%% Save the result to a file
% Convert cell to a table with the specified variable names
resTable = cell2table(result','VariableNames',{'Filename' 'Mean Pa/Pd'});

% Write the result table to a CSV file at the root directory level
writetable(resTable,fullfile(rootDir,'..','Pa_Pd_3.csv'))



