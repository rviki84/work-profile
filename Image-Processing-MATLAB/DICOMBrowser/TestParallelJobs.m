clear all

dicomDir = 'D:\VIRTUheart\cases_for_verification\V010_excelera\IMAGE';
dicomDir = 'D:\VIRTUheart\FromACI\V101_xcelera\IMAGE';
dicomFiles = dir(dicomDir);
dicomFiles = dicomFiles(3:end);    % avoid . and ..
nfiles = length(dicomFiles);
if nfiles<1
    disp('No files available!')
    return;
end

imagePaths = cell(nfiles,1);
images = cell(nfiles,1);

for n=1:nfiles
    fname = dicomFiles(n).name;
    try
       info = dicominfo([dicomDir '\' fname]);
       LAORAO = info.PositionerPrimaryAngle;
    catch
%        disp ('Not a DICOM file!');
       continue
    end
    
    imagePaths{n}=[dicomDir '\' fname];
    images{n} = dicomread(imagePaths{n});
    images{n} = squeeze(images{n});
end

imagePaths = imagePaths(~cellfun('isempty',imagePaths));
images = images(~cellfun('isempty',images));

figure
ax = gobjects(length(imagePaths),1);
nrows = ceil(length(imagePaths)/5);
yfactor = 1/nrows;
ystart = yfactor*(nrows-1);
height = yfactor;

for j=1:length(imagePaths)
    ax(j) = subplot(nrows,5,j);
    axis off
    m = mod(j,5);
    q = fix(j/5);
    if m ~= 0
        pos = [(m-1)*0.195 ystart-q*yfactor 0.19 height];
    else
        pos = [4*0.195 ystart-(q-1)*yfactor 0.19 height];
    end
    ax(j).Position = pos;
%     h = implay(images{j,1});
%     play(h.DataSource.Controls)
    TestCINEImage(images{j,1},ax(j))
end


% c = parcluster;
% % job = createJob(c);
% 
% 
% %% Add tasks to the job.
% for i = 1:10
% %     createTask(job,@TestCINEImage,0,{images{1,1},ax(i)});
%     job=batch(c,@TestCINEImage,0,{images{1,1},ax(i)});
% end
% 
% %% Run the job.
% % submit(job);
% 
% %% Wait for the job to finish running, and retrieve the job results.
% wait(job);
% 
% % out = fetchOutputs(job);
% % disp(out{3});
% 
% %% Delete the job.
% delete(job);
