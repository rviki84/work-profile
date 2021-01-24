%
% Sets values for constants in Model1.mdl
%
% Ref:  M. Ursino
%       "Interaction between carotid baroregulation and the pulsating heart:
%       a mathematical model"
%       Am J Physiol, 1998

%if (~exist('Vt'))
%    global_declarations;
%end

SEDENTARY = 1;
ACTIVE    = 2;
TRAINED   = 3;

% Nomenclature
%
% sa: Systemic Arterial
% sp: Splanchnic Peripheral
% ep: Extra-spanchnic Peripheral
% sv: Splanchnic Venous
% ev: Extra-splanchnic Venous
% pa: Pulmonary Arterial
% pp: Pulmonary Peripheral
% pv: Pulmonary Venous

% Du Bois -formula for body surface area
%BSA = 0.007184 * subject.weight^0.425 * subject.height^0.725; 
BSA = subject.BSA;

%-------------------------------------------------------------------------
% Table 1
%modelParams.Vt = 5300; % [ml] / footnote 

% Total blood volume from formula in (Morgan, Mikhail, and Murray. Clinical Anesthesiology. 3rd Edition.)
modelParams.Vt = 75 * subject.weight - 10 * subject.weight * subject.isFemale;

% Default value of compliance factor
complianceFactor = 1.8;

% Compliance [ml/mmHg] (from Tanaka et al. 2000)
if (subject.age < 40)
    switch (subject.activityStatus)
        case SEDENTARY
            complianceFactor = 2.0;
        case ACTIVE
            complianceFactor = 2.0; 
        case TRAINED
            complianceFactor = 2.2; 
    end
elseif ((subject.age(1) >= 40) && (subject.age(1) < 65))
    switch (subject.activityStatus)
        case SEDENTARY
            complianceFactor = 1.25;
        case ACTIVE
            complianceFactor = 1.5;
        case TRAINED
            complianceFactor = 1.6;
    end
elseif (subject.age >= 65)
    switch (subject.activityStatus)
        case SEDENTARY
            complianceFactor = 1.2; 
        case ACTIVE
            complianceFactor = 1.5;
        case TRAINED
            complianceFactor = 1.6;
    end
end

if (subject.isSmoker)
    complianceFactor = complianceFactor * 0.7649;
end

complianceFactor = complianceFactor / 1.8; % Ursino's values for young adult

modelParams.Csa = 0.28  * complianceFactor;  % compliance, systemic arteries
modelParams.Csp = 2.05  * complianceFactor;  % compliance, splanchnic peripheral
modelParams.Cep = 1.67  * complianceFactor;  % compliance, extrasplanchnic peripheral
modelParams.Csv = 61.11;                     % compliance, systemic veins
modelParams.Cev = 50.0;                      % compliance, extrasplanchnic venous
modelParams.Cpa = 0.76  * complianceFactor;  % compliance, pulmonary arteries
modelParams.Cpp = 5.80  * complianceFactor;  % compliance, pulmonary peripheral
modelParams.Cpv = 25.37;                     % compliance, pulmonary veins

% Unstr. volume [ml]
volumeFactor = BSA / 1.9;
%volumeFactor = 1;

modelParams.Vu_sa = 0     * volumeFactor; % unstressed volume, systemic arteries
modelParams.Vu_sp = 274.4 * volumeFactor; % unstressed volume, splanchnic peripheral
modelParams.Vu_ep = 336.6 * volumeFactor; % unstressed volume, extrasplanchnic peripheral
% Vu_sv = 1121;
% Vu_ev = 1375; 
modelParams.Vu_pa = 0     * volumeFactor; % unstressed volume, pulmonary arteries
modelParams.Vu_pp = 123   * volumeFactor; % unstressed volume, pulmonary peripheral 
modelParams.Vu_pv = 120   * volumeFactor; % unstressed volume, pulmonary veins

% Hydraulic resistance [mmHg*s/ml]
% NO CHANGE from Otsuki et al. 2008
modelParams.Rsa = 0.06;

% Rsp = 3.307;
% Rep = 1.407;

modelParams.Rsv = 0.038;  % Not Available
modelParams.Rev = 0.016;  % Not Available

% (from van Empel et al. 2014)
if (subject.age > 55)
    pulmonaryResistanceScaling =  1.1818;
else
    pulmonaryResistanceScaling = 1;
end
modelParams.Rpa = 0.023 * pulmonaryResistanceScaling; 
modelParams.Rpp = 0.0894 * pulmonaryResistanceScaling;
modelParams.Rpv = 0.0056 * pulmonaryResistanceScaling;

% Inertance [mmHg*ml/s^2]
modelParams.Lsa = 0.22*10^-3; % Not Available
modelParams.Lpa = 0.18*10^-3; % Not Available

%-------------------------------------------------------------------------
% Table 2
modelParams.Ksys  = 0.075; % [s^2] / footnote

% from McDonnell et al. 2013
if (subject.age < 40)
    switch (subject.activityStatus)
        case SEDENTARY
            modelParams.Tsys0 = 0.319 + modelParams.Ksys / 0.845;
        case ACTIVE
            modelParams.Tsys0 = 0.329 + modelParams.Ksys / 0.9375;
        case TRAINED
            modelParams.Tsys0 = 0.329 + modelParams.Ksys / 0.9375;
    end
elseif (subject.age >= 40)
    switch (subject.activityStatus)
        case SEDENTARY
            modelParams.Tsys0 = 0.328 + modelParams.Ksys / 0.8955;
        case ACTIVE
            modelParams.Tsys0 = 0.331 + modelParams.Ksys / 0.9524;
        case TRAINED
            modelParams.Tsys0 = 0.331 + modelParams.Ksys / 0.9524;
    end
end
    
%Tsys0 = 0.5; % [s]     / footnote

% Left Heart
modelParams.Cla     = 19.23; % Not Available

% Chamber volume estimations from:
%
% Lang RM, Bierig M, Devereux RB, Flachskampf FA, Foster E, Pellikka PA, 
% Picard MH, Roman MJ, Seward J, Shanewise J, Solomon S, Spencer KT, St John 
% Sutton M, and Stewart W. Recommendations for chamber quantification. Eur J 
% Echocardiogr 2006 Mar; 7(2) 79-108. doi:10.1016/j.euje.2005.12.014 pmid:16458610. 
% PubMed

modelParams.Vu_la = 22 * BSA;
% Vu_la   = 25;

modelParams.Rla     = 2.5*10^-3;  % Not Available
modelParams.P0_lv   = 1.5;        % Not Available
modelParams.Ke_lv   = 0.014;      % Not Available

modelParams.Vu_lv   = 12 * BSA;
%Vu_lv   = 16.77;

% Emax_lv = 2.95;
modelParams.Kr_lv   = 3.75*10^-4; % Not Available

% Right Heart
modelParams.Cra     = 31.25;      % Not Available

modelParams.Vu_ra   = 13 * BSA;
%Vu_ra   = 25;         % Not Available

modelParams.Rra     = 2.5*10^-3;  % Not Available
modelParams.P0_rv   = 1.5;        % Not Available
modelParams.Ke_rv   = 0.011;      % Not Available

modelParams.Vu_rv   = 21 * BSA;
%Vu_rv   = 40.8;       % Not Available

% Emax_rv = 1.75;
modelParams.Kr_rv   = 1.4*10^-3;  % Not Available

%-------------------------------------------------------------------------
% Table 3

% Carotid sinus afferent pathway
modelParams.Pn = 92;        % [mmHg]
modelParams.Ka = 11.758;    % [mmHg]
modelParams.fmin = 2.52;    % [spikes/s]
modelParams.tau_z = 6.37;   % [s]
modelParams.fmax = 47.78;   % [spikes/s]
modelParams.tau_p = 2.076;  % [s]

% Sympathetic efferent pathway
modelParams.fes_Inf = 2.10; % [spikes/s]
modelParams.fes0 = 16.11;
modelParams.fes_min = 2.66;
modelParams.Kes = 0.0675;   % [s]

% Vagal efferent pathway
modelParams.fev0 = 3.2;     % [spikes/s]
modelParams.fev_Inf = 6.3;
modelParams.fcs0 = 25;
modelParams.Kev = 7.06; % [spikes/s]

% Effectors
modelParams.G_Emax_lv = 0.475;
modelParams.G_Emax_rv = 0.282;
modelParams.G_R_sp    = 0.695;
modelParams.G_R_ep = 0.53;
modelParams.G_Vu_sv = -265.4;
modelParams.G_Vu_ev = -132.5;
modelParams.G_Ts = -0.13;
modelParams.G_Tv = 0.09;

modelParams.tau_Emax_lv = 8; % [s]
modelParams.tau_Emax_rv = 8;
modelParams.tau_R_sp = 6;
modelParams.tau_R_ep = 6;
modelParams.tau_Vu_sv = 20;
modelParams.tau_Vu_ev = 20;
modelParams.tau_Ts = 2;
modelParams.tau_Tv = 1.5;

modelParams.D_Emax_lv = 2; % [s]
modelParams.D_Emax_rv = 2;
modelParams.D_R_sp = 2;
modelParams.D_R_ep = 2;
modelParams.D_Vu_sv = 5;
modelParams.D_Vu_ev = 5;
modelParams.D_Ts = 2;
modelParams.D_Tv = 0.2;

modelParams.Emax_lv0 = 2.392; % [mmHg/ml]
modelParams.Emax_rv0 = 1.412;
modelParams.Rsp0 = 2.49;
modelParams.Rep0 = 0.78;
modelParams.Vu_sv0 = 1435.4;
modelParams.Vu_ev0 = 1537;
modelParams.T0 = 0.58; % [s] 

