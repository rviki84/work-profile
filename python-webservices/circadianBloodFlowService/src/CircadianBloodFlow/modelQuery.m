function output = modelQuery(params, modelParams, subject)

% Override parameters being tuned
modelParams.Vt = 5300 * params(1);

modelParams.Csa = 0.28   * params(2);  % compliance, systemic arteries
modelParams.Csp = 2.05;%   * params(2);  % compliance, splanchnic peripheral
modelParams.Cep = 1.67;%   * params(2);  % compliance, extrasplanchnic peripheral
modelParams.Cpa = 0.76;%   * params(2);  % compliance, pulmonary arteries
modelParams.Cpp = 5.80;%   * params(2);  % compliance, pulmonary peripheral

modelParams.Rsa = 0.06   * params(3);  % resistance, systemic arteries

% PULMONARY ARTERY RESISTANCE ELIMINATED AS UNIDENTIFIABLE PARAMETER
%modelParams.Rpa = 0.023  * params(4);  % resistance, pulmonary arteries
%modelParams.Rpp = 0.0894 * params(4);  % resistance, pulmonary peripheral
%modelParams.Rpv = 0.0056 * params(4);  % resistance, pulmonary venous
modelParams.Rpa = 0.023; 
modelParams.Rpp = 0.0894;
modelParams.Rpv = 0.0056;

modelParams.Tsys0 = 0.5  * params(4);  % systolic interval length

modelParams.Vu_la   = 25    * params(5); % left atrium unstressed volume
modelParams.Vu_lv   = 16.77 * params(5); % left ventricle unstressed volume
modelParams.Vu_ra   = 25    * params(5); % right atrium unstressed volume
modelParams.Vu_rv   = 40.8  * params(5); % right ventricle unstressed volume

init_values =1.0e+03 * [0.0013
    0.0393
    0.0004
    0.0000
    0.0002
   -0.0005
    0.0003
    0.0001
    5.3000
   -0.0000
    0.0166
    0.0002
    0.0015
    0.0007
    0.0000
    0.0000
    0.0000
    0.0000
   -0.0033
   -0.0017
   -0.0000
    0.0000
    0.0421
    0.0025
    0.0139
    0.0033];
y0 = init_values;

% y0 = inintialization (vector elements must be consistent with dydt of solver output)
%y0(1) = 0;     % y(1)    Ppa  [pa: pulmonary arterial, Table 1 footnote] 
%y0(2) = 0;     %  (2)    Fpa   
%y0(3) = 0;     %  (3)    Ppp  [pp: pulmonary peripheral] 
%y0(4) = 0;     %  (4)    Ppv  [pv: pulmonary venous] 
%y0(5) = 0;     %  (5)    Psa  (in closed-loop: Pcs = Psa, p. H1745)
%y0(6) = 0;     %  (6)    Fsa   
%y0(7) = 0;     %  (7)    Psp  [sp: splanchnic peripheral] 
%y0(8) = 0;     %  (8)    Pev  [ev: extrasplanchnic venous] 
y0(9) = modelParams.Vt;    %  (9)    Vt    
%y0(10)= 0;     %  (10)   Pla   
y0(11)= modelParams.Vu_lv; %  (11)   Vlv   [initialise to unstressed volume, in the lack of a better guess]
%y0(12)= 0;     %  (12)   CSI   
%y0(13)= 0;     %  (13)   Pra   
%y0(14)= 0;     %  (14)   Pbar  
%y0(15)= 0;     %  (15)   Delta Emax_rv  [assume: initially Delta = 0]
%y0(16)= 0;     %  (16)   Delta Emax_lv 
%y0(17)= 0;     %  (17)   Delta Rsp     
%y0(18)= 0;     %  (18)   Delta Rep     
%y0(19)= 0;     %  (19)   Delta Vu_sv   
%y0(20)= 0;     %  (20)   Delta Vu_ev   
%y0(21)= 0;     %  (21)   DEL_Ts 
%y0(22)= 0;     %  (22)   DEL_Tv 
y0(23)= modelParams.Vu_rv; %  (23)   Vrv    [initialise to unstressed volume, in the lack of a better guess]
y0(24) = (modelParams.fmin+modelParams.fmax*exp((y0(14)-modelParams.Pn)/modelParams.Ka))/(1+exp((y0(14)-modelParams.Pn)/modelParams.Ka)); % (24)   fcs    
y0(25) = exp(-modelParams.Kes*y0(24))*(modelParams.fes0-modelParams.fes_Inf)+modelParams.fes_Inf;                 % (25)   fes    
y0(26) = (modelParams.fev0+modelParams.fev_Inf*exp((y0(24)-modelParams.fcs0)/modelParams.Kev))/(1+exp((y0(24)-modelParams.fcs0)/modelParams.Kev));  % (26)   fev    

% ode = @(t,y) my_osc(t,y);
% [t,y] = ode45(ode, tspan, y0);

tspan  = [0 50];

options = ddeset('MaxStep', 0.01, 'RelTol', 1e-4, 'NormControl', 'on');

ddefun = @(t,y,Z,modelParams) my_osc(t,y,Z,modelParams);

lags   = [modelParams.D_Emax_lv modelParams.D_Emax_rv modelParams.D_R_sp modelParams.D_R_ep modelParams.D_Vu_sv modelParams.D_Vu_ev modelParams.D_Ts modelParams.D_Tv];
tic

sol    = dde23limited(ddefun,lags,y0,tspan, options, modelParams);

save sol sol;
tp = toc;

% Extract last heartbeat
ind_lhb = find(sol.x > 49);

% Diastolic BP, systemic arteries
output(1) = min(sol.y(5,ind_lhb)); 

% Systolic  BP, systemic arteries
output(2) = max(sol.y(5,ind_lhb)); 

% Heart rate
load HR HR;
output(3) = HR;

% Ejection fraction
output(4) = (max(sol.y(11,ind_lhb)) - min(sol.y(11,ind_lhb))) ./ max(sol.y(11,ind_lhb)); 

% LV volume, end-diastolic
output(5) = max(sol.y(11,ind_lhb)); 

% MISMATCH BETWEEN AORTIC AND CAROTID PULSATILITY INDEX NECESSITATED ITS
% REMOVAL FROM THE MODEL INPUTS
% Pulsatility index, aortic
%output(6) = (max(sol.y(6,ind_lhb)) - min(sol.y(6,ind_lhb))) / mean(sol.y(6,ind_lhb)); 