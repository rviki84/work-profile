function CircadianBloodflow(HolterBP_TimeSeriesFile, ...
                            Demographics_Weight, ...
                            Demographics_Height, ...
                            Demographics_Age, ...
                            Demographics_IsFemale, ...
                            Lifestyle_IsSmoker, ...
                            Lifestyle_ActivityStatus, ...
                            CardiacUS_LVEjectionFraction, ...
                            CardiacUS_LVEndDiastolicVolume, ...
                            CarotidUS_ICA_L_Diameter, ....
                            CarotidUS_ICA_R_Diameter, ...
                            CarotidUS_ICA_L_MinVelocity, ...
                            CarotidUS_ICA_R_MinVelocity, ...
                            ID, ...
                            OutputFolder)
clc
close all

if (nargin < 15)
    OutputFolder = '.';
end

% Record subject values in one struct
subject.weight         = str2num(Demographics_Weight);
subject.height         = str2num(Demographics_Height);
subject.age            = str2num(Demographics_Age);
subject.isFemale       = str2num(Demographics_IsFemale);
subject.isSmoker       = str2num(Lifestyle_IsSmoker);

if (str2num(Lifestyle_ActivityStatus) <=2 )
    % Sedentary
    subject.activityStatus = 1;
elseif (str2num(Lifestyle_ActivityStatus) < 5)
    % Active
    subject.activityStatus = 2;
else
    % Endurance-trained
    subject.activityStatus = 3;
end

subject.BSA            = 0.007184 * subject.weight^0.425 * subject.height^0.725; % DuBois formula
subject.LV_EF          = str2num(CardiacUS_LVEjectionFraction) / 100;
subject.LV_EDV         = str2num(CardiacUS_LVEndDiastolicVolume);
subject.d_ICAL         = str2num(CarotidUS_ICA_L_Diameter) / 10;
subject.d_ICAR         = str2num(CarotidUS_ICA_R_Diameter) / 10;
subject.vmin_ICAL      = str2num(CarotidUS_ICA_L_MinVelocity);
subject.vmin_ICAR      = str2num(CarotidUS_ICA_R_MinVelocity);

%--------------------------------------------------------------------------
% CARDIAC OUTPUT -MODEL (Ursino 1998)
%--------------------------------------------------------------------------

% REQUIRED INPUT:
%
% 1. Holter BP data file (time series for DBP, SBP, and HR)
% 2. Subject demographics (age, gender, BSA, smoker status, activity
%                          status)
% 3. Cardiac US information (LV ejection fraction, LV EDV)
%
% OUTPUT:
%
% 1. Time series of arterial pressure
% 2. Simulated waveforms for cardiac output over the 24 hour cycle + mean
%    waveform and variability bounds
% 3. Simulated waveforms for cerebral blood flow over the 24 hour cycle
%    + mean waveform and variability bounds
% 4. Figures for inspection of the parameter fitting procedure

% Read Holter BP recordings

fprintf('Loading Holter BP record... ')
%cd(folderPath)
try
    %cd(folderPath)
    %result = dir('*.csv');
    fileName = HolterBP_TimeSeriesFile;
    strIndex = strfind(fileName, 'VL_DARE');
    id = fileName(strIndex:end-4);
    id=ID;
    %id = fileName(1:end-4);
    holterData = importdata(fileName, ',', 1);
    holterData = holterData.data;
catch
    error(['Could not navigate to folder  or did not find a unique file with .csv extension.'])
end
disp('done.')

INDEX_TIME      = 2;%       Relative Time [min]
INDEX_SYSTOLIC  = 5;%       Systolic Pressure [mmHg]
INDEX_DIASTOLIC = 6;%       Diastolic Pressure [mmHg]
INDEX_HEARTRATE = 7;%       Heart Rate [bpm]

% Train the surrogate models using pre-computed snapshots

fprintf('Training surrogate model for closed-loop case...\n')
% Load the pre-computed simulation runs
loadSurrogateModels;
fprintf('done.\n')

% ...and do the same with the open-loop model

fprintf('Training surrogate model for open-loop case...\n')
loadSurrogateModels_OpenLoop;
fprintf('done.\n')

% Read patient demographics and set model default parameters


Set_Constants;

LVCorr = 56.4; % correction to echo volume estimation from MRI (from Malm et al. 2004)
EFCorr = 0.06; % correction to echo ejection fraction from MRI (from Malm et al. 2004)

% Generate initial parameter calibration from resting state

output = [holterData(1,INDEX_DIASTOLIC) holterData(1,INDEX_SYSTOLIC) holterData(1,INDEX_HEARTRATE) subject.LV_EF+EFCorr subject.LV_EDV+LVCorr];

save output output;
opts = optimset('Display','off');

% Perform parameter identification using surrogate model
fprintf('Fitting parameters at rest from surrogate model... ');
warning off;
% Step 1: Genetic algorithm to find initial region of parameter space
[optimParams, fVal, exitflag] = ga(@modelFit, 5, [], [], [], [], [0.8 0.6 0.6 0.6 0.6 ], [1.2 1.4 1.4 1.4 1.4 ], [], opts);

% Step 2: Continuous optimisation to find precise value of parameters
[optimParams, fVal, exitflag] = fmincon(@modelFit, optimParams, [], [], [], [], [0.8 0.6 0.6 0.6 0.6 ], [1.2 1.4 1.4 1.4 1.4 ], [], opts);

disp('done.')
disp('                           DBP           SBP           HR           EF             LVV')
disp(['Surrogate output at rest:  ' num2str([predict(mdl1,[optimParams optimParams.^2]) predict(mdl2,[optimParams optimParams.^2]) predict(mdl3,[optimParams optimParams.^2]) predict(mdl4,[optimParams optimParams.^2]) predict(mdl5,[optimParams optimParams.^2])])])

params = [optimParams holterData(1,INDEX_HEARTRATE)/75]; % Add HR to the parameter set for open loop configuration

disp('Fitting dynamic parameters')
% Parametric continuation in time, fix parameter #5 and the HR (open loop)
for n=2:size(holterData,1)
    fprintf([' at time t = ' num2str(holterData(n, INDEX_TIME)) ' min...'])
    output = [holterData(n,INDEX_DIASTOLIC) holterData(n,INDEX_SYSTOLIC) holterData(n,INDEX_HEARTRATE) 0.472+EFCorr 119.3+LVCorr];
    save output output;
    Aeq = zeros(2,6);
    Aeq(1,5) = 1;
    Aeq(2,6) = 1;
    %Aeq(3,1) = 1;
    beq = [params(1,5); holterData(n,INDEX_HEARTRATE)/75];% optimParams(1)];
    [optimParams, fVal, exitflag] = fmincon(@modelFit_OpenLoop, params(n-1,:), [], [], Aeq, beq, [0.8 0.6 0.6 0.6 0.6 0.6], [1.2 1.4 1.4 1.4 1.4 1.4], [], opts);
    params = [params; optimParams];
    disp(['residual = ' num2str(fVal)])
end
warning on;
disp('done.')

% Full-model evaluation to compute cardiac output flow rate
disp('Solving full model for cardiac output...')
for n=1:size(params,1)
    % Solve full model for each time point
    disp([' at time t = ' num2str(holterData(n,INDEX_TIME)) ' min'])
    modelQuery_openloop(params(n,:), modelParams, subject);
    load sol sol;
    ind_lhb = find(sol.x > 48);
    t_CO{n} = sol.x(ind_lhb);
    CO{n} = sol.y(6,ind_lhb);
    BP{n} = sol.y(5,ind_lhb);
end
disp('done.')

% Extraction of flow information and removal of phase shift for comparison
CO_SUM = zeros(1,100);
CO_MIN = ones(1,100) * 1e6;
CO_MAX = ones(1,100) * -1e6;
BP_SUM = zeros(1,100);
BP_MIN = ones(1,100) * 1e6;
BP_MAX = ones(1,100) * -1e6;
left_end = cell(size(params,1));
right_end = cell(size(params,1));

for n=1:size(params,1)   
    
    [garbo,pks] = findpeaks(CO{n});
    garbo_ind = find(garbo > 100);
    garbo = garbo(garbo_ind);
    pks = pks(garbo_ind);
    
    % Find beginning and end of the last complete waveform
    if (length(pks) > 1)
        left_end{n} = pks(1);
        right_end{n} = pks(2);
    else
        left_end{n} = pks(1);
        right_end{n} = length(CO{n});
    end
    
    % Shift end-diastolic phase of waveforms to all begin at same time
    t_CO_intvl = t_CO{n};
    CO_intvl = CO{n};
    BP_intvl = BP{n};
    if ( (t_CO_intvl(1) - t_CO_intvl(left_end{n})) > -0.2 && numel(pks) > 2)
        left_end{n} = pks(2);
        right_end{n} = pks(3);
    end
    
    % Interpolate CO/BP curves to the same sample rate
    x=t_CO_intvl(1:right_end{n})-t_CO_intvl(left_end{n});
    y=CO_intvl(1:right_end{n});
    xx = linspace(-0.2,0.5,100);
    
    % Compute mean CO/BP and min/max CO/BP curves for plotting
    CO_SUM = CO_SUM + spline(x,y,xx);   
    CO_MIN = min(CO_MIN, spline(x,y,xx));
    CO_MAX = max(CO_MAX, spline(x,y,xx));
    CO{n} = spline(x,y,xx); % Store only the normalised waveforms
    y = BP_intvl(1:right_end{n});
    BP{n} = spline(x,y,xx); % Apply the same shift to the BP waves
    BP_SUM = BP_SUM + spline(x,y,xx);   
    BP_MIN = min(BP_MIN, spline(x,y,xx));
    BP_MAX = max(BP_MAX, spline(x,y,xx));
end
CO_SUM = CO_SUM / size(params,1);
BP_SUM = BP_SUM / size(params,1);

%--------------------------------------------------------------------------
% CEREBRAL BLOOD FLOW -MODEL (IN MCA)
%--------------------------------------------------------------------------

% Full-model evaluation to compute cardiac output flow rate
disp('Solving cerebral autoregulation model for cerebral blood flow...')

% Predict cerebral blood flow velocity in MCA-L and MCA-R
for n=1:size(CO,2)
    [CBF_L{n}, CBF_R{n}] = simulateCA(holterData(n,INDEX_HEARTRATE), BP{n}, subject.age, subject.isFemale, subject.d_ICAL, subject.d_ICAR, subject.vmin_ICAL, subject.vmin_ICAR);
end

% Clean all temporary files
system('rm output.mat');
system('rm sol.mat');
system('rm mdl1.mat');
system('rm mdl2.mat');
system('rm mdl3.mat');
system('rm mdl4.mat');
system('rm mdl5.mat');
system('rm mdl1_OpenLoop.mat');
system('rm mdl2_OpenLoop.mat');
system('rm mdl3_OpenLoop.mat');
system('rm mdl4_OpenLoop.mat');
system('rm mdl5_OpenLoop.mat');
cd(OutputFolder)

%--------------------------------------------------------------------------
% PRINT OUTPUTS
%--------------------------------------------------------------------------


% Plotting of relevant model parameters
% figure(1)
% subplot(2,2,1)
% plot(holterData(:,INDEX_TIME)/60,params(:,1),'o-','LineWidth',2,'MarkerSize',7)
% title('Total blood volume')
% xlabel('Time [h]')
% ylabel('Scaling parameter [au]')
% title('TOTAL BLOOD VOLUME')
% axis([-1 25 0.55 1.45])

% subplot(2,2,2)
% plot(holterData(:,INDEX_TIME)/60,params(:,2),'o-','LineWidth',2,'MarkerSize',7)
% title('Systemic arterial compliance')
% xlabel('Time [h]')
% ylabel('Scaling parameter [au]')
% title('SYSTEMIC ARTERIAL COMPLIANCE')
% axis([-1 25 0.55 1.45])

% subplot(2,2,3)
% plot(holterData(:,INDEX_TIME)/60,params(:,3),'o-','LineWidth',2,'MarkerSize',7)
% title('Systemic arterial resistance')
% xlabel('Time [h]')
% ylabel('Scaling parameter [au]')
% title('SYSTEMIC ARTERIAL RESISTANCE')
% axis([-1 25 0.55 1.45])

% subplot(2,2,4)
% plot(holterData(:,INDEX_TIME)/60,params(:,4),'o-','LineWidth',2,'MarkerSize',7)
% title('Systolic interval')
% xlabel('Time [h]')
% ylabel('Scaling parameter [au]')
% title('SYSTOLIC INTERVAL')
% axis([-1 25 0.55 1.45])

% exportfig(gcf,'modelCalibration.eps','height',24,'width',36,'FontMode', 'fixed', 'FontSize', 18,'Color','cmyk');

% Compute model predictions using surrogate model
for n=1:size(params,1)
    optimParams = params(n,:);
    surrogate_output(n,:) = [predict(mdl1_OpenLoop,[optimParams optimParams.^2]) predict(mdl2_OpenLoop,[optimParams optimParams.^2]) predict(mdl3_OpenLoop,[optimParams optimParams.^2]) predict(mdl4_OpenLoop,[optimParams optimParams.^2]) predict(mdl5_OpenLoop,[optimParams optimParams.^2])];
end

% Plotting of relevant model predictions
% figure(2)
% for n=1:length(holterData(:,1))
%    fullOutput(n) = max(BP{n});
% end
% plot(holterData(:,INDEX_TIME)/60,holterData(:,INDEX_SYSTOLIC),'bs',holterData(:,INDEX_TIME)/60,fullOutput,'ko-',holterData(:,INDEX_TIME)/60,surrogate_output(:,2),'rd--','LineWidth',2,'MarkerSize',7)
% title('Systolic blood pressure')
% xlabel('Time [h]')
% ylabel('Pressure [mmHg]')
% title('SYSTOLIC BLOOD PRESSURE (observed vs. modelled)')
% legend('Ambulatory BP','Full model','Surrogate model')
% axis([-1 25 80 170])
% exportfig(gcf,'systolicBP.eps','height',12,'width',18,'FontMode', 'fixed', 'FontSize', 32,'Color','cmyk');

% figure(3)
% for n=1:length(holterData(:,1))
%     fullOutput(n) = min(BP{n});
% end
% plot(holterData(:,INDEX_TIME)/60,holterData(:,INDEX_DIASTOLIC),'bs',holterData(:,INDEX_TIME)/60,fullOutput,'ko-',holterData(:,INDEX_TIME)/60,surrogate_output(:,1),'rd--','LineWidth',2,'MarkerSize',7)
% title('Diastolic blood pressure')
% xlabel('Time [h]')
% ylabel('Pressure [mmHg]')
% title('DIASTOLIC BLOOD PRESSURE (observed vs. modelled)')
% axis([-1 25 50 120])
%legend('Ambulatory BP','Full model','Surrogate model')
% exportfig(gcf,'diastolicBP.eps','height',12,'width',18,'FontMode', 'fixed', 'FontSize', 32,'Color','cmyk');
% 
% 
% figure(4)
% plot(holterData(:,INDEX_TIME)/60,holterData(:,INDEX_HEARTRATE),'bo-',holterData(:,INDEX_TIME)/60,surrogate_output(:,3),'ks--','LineWidth',2,'MarkerSize',7)
% title('Heart rate')
% xlabel('Time [h]')
% ylabel('Frequency [bpm]')
% title('HEART RATE (observed vs. modelled)')
% legend('Ambulatory HR','Full/surrogate model')
% exportfig(gcf,'heartRate.eps','height',12,'width',18,'FontMode', 'fixed', 'FontSize', 32,'Color','cmyk');
% axis([-1 25 50 140])
% 
% %MAP = surrogate_output(:,3) + 1/3 * (surrogate_output(:,2) - surrogate_output(:,3));
% 
% figure(5)
% plot(xx,CO_SUM,'g-','LineWidth',3)
% hold on
% for n=0:1000
%     plot(xx,CO_MIN*(n/1000)+(1-n/1000)*CO_MAX,'Color',[0.6 0.6 0.6])
% end
% plot(xx,CO_SUM,'g-','LineWidth',3)
% hold off
% xlabel('Time [s]')
% ylabel('Cardiac output [ml/s]')
% legend('Mean wave','Circadian variability bounds')
% axis([-0.2 0.5 -50 800])
% title('CARDIAC OUTPUT (model predicted)')
% exportfig(gcf,'cardiacOutput.eps','height',12,'width',18,'FontMode', 'fixed', 'FontSize', 32,'Color','cmyk');
% 
% figure(6)
% plot(xx,BP_SUM,'g-','LineWidth',3)
% hold on
% for n=0:1000
%     plot(xx,BP_MIN*(n/1000)+(1-n/1000)*BP_MAX,'Color',[0.6 0.6 0.6])
% end
% plot(xx,BP_SUM,'g-','LineWidth',3)
% hold off
% xlabel('Time [s]')
% ylabel('Pressure [mmHg]')
% legend('Mean pressure','Circadian variability bounds')
% axis([-0.2 0.5 50 160])
% title('SYSTEMIC ARTERIAL PRESSURE (model predicted)')
% exportfig(gcf,'arterialPressure.eps','height',12,'width',18,'FontMode', 'fixed', 'FontSize', 32,'Color','cmyk');
% 
% % Plot CBF variability
% CBF_SUM_L = 0;
% CBF_MIN_L = 1e6;
% CBF_MAX_L = -1e6;
% xx = linspace(-0.2,0.5,100);
% for n=1:length(CBF_L)
%     CBF_SUM_L = CBF_SUM_L + CBF_L{n};
%     CBF_MIN_L = min(CBF_MIN_L, CBF_L{n});
%     CBF_MAX_L = max(CBF_MAX_L, CBF_L{n});
% end
% CBF_SUM_L = CBF_SUM_L / length(CBF_L);
% 
% CBF_SUM_R = 0;
% CBF_MIN_R = 1e6;
% CBF_MAX_R = -1e6;
% xx = linspace(-0.2,0.5,100);
% for n=1:length(CBF_R)
%     CBF_SUM_R = CBF_SUM_R + CBF_R{n};
%     CBF_MIN_R = min(CBF_MIN_R, CBF_R{n});
%     CBF_MAX_R = max(CBF_MAX_R, CBF_R{n});
% end
% CBF_SUM_R = CBF_SUM_R / length(CBF_R);
% 
% figure(7)
% %subplot(2,1,1)
% plot(xx,CBF_SUM_L,'g-','LineWidth',3)
% hold on
% for n=0:1000
%     plot(xx,CBF_MIN_L*(n/1000)+(1-n/1000)*CBF_MAX_L,'Color',[0.6 0.6 0.6])
% end
% plot(xx,CBF_SUM_L,'g-','LineWidth',3)
% hold off
% xlabel('Time [s]')
% ylabel('Velocity [cm/s]')
% legend('Mean velocity (MCA-L)','Circadian variability bounds')
% axis([-0.2 0.5 0 120])
% title('CEREBRAL BLOOD FLOW VELOCITY (model predicted)')
% exportfig(gcf,'cerebralBF_L.eps','height',12,'width',18,'FontMode', 'fixed', 'FontSize', 32,'Color','cmyk');
% 
% figure(8)
% plot(xx,CBF_SUM_R,'g-','LineWidth',3)
% hold on
% for n=0:1000
%     plot(xx,CBF_MIN_R*(n/1000)+(1-n/1000)*CBF_MAX_R,'Color',[0.6 0.6 0.6])
% end
% plot(xx,CBF_SUM_R,'g-','LineWidth',3)
% hold off
% xlabel('Time [s]')
% ylabel('Velocity [cm/s]')
% legend('Mean velocity (MCA-R)','Circadian variability bounds')
% axis([-0.2 0.5 0 120])
% title('CEREBRAL BLOOD FLOW VELOCITY (model predicted)')
% exportfig(gcf,'cerebralBF_R.eps','height',12,'width',18,'FontMode', 'fixed', 'FontSize', 32,'Color','cmyk');

% Open and write to ASCII file
fprintf('Writing cardiac output to file...')
data_set = [];
data_set2 = [];
data_set3 = [];
data_set4 = [];
ncols = size(CO,2);

for n=1:ncols
    times     = holterData(:,INDEX_TIME)';
    HR        = holterData(:,INDEX_HEARTRATE)';
    data_set  = [data_set CO{n}'];    
    data_set2 = [data_set2 BP{n}'];        
    data_set3 = [data_set3 CBF_L{n}];         
    data_set4 = [data_set4 CBF_R{n}];         
end

fid = fopen([id '_cardiacOutput.csv'], 'wt');
fprintf(fid,'#Time [min]\n');
fprintf(fid,[repmat('%1.0f, ',[1,ncols]) '\n'], times);
fprintf(fid,'#Heart Rate\n');
fprintf(fid,[repmat('%1.0f, ',[1,ncols]) '\n'], HR);
fprintf(fid,'#Cardiac Output [ml/s]\n');
fprintf(fid,[repmat('%1.2f, ',[1,ncols]) '\n'], data_set');
fprintf(fid,'#Systemic Blood Pressure [mm/hg]\n');
fprintf(fid,[repmat('%1.2f, ',[1,ncols]) '\n'], data_set2');
fprintf(fid,'#MCA-L Blood Flow Velocity [cm/s]\n');
fprintf(fid,[repmat('%1.2f, ',[1,ncols]) '\n'], data_set3');
fprintf(fid,'#MCA-R Blood Flow Velocity [cm/s]\n');
fprintf(fid,[repmat('%1.2f, ',[1,ncols]) '\n'], data_set4');
fclose(fid);
disp('done.')
close all
