function dydt = my_osc(t, y, Z, modelParams)
%
% funtion:   implements ODE system of the Ursino model (see ref. below)
%
% reference: Ursino M. "Interaction between carotid baroregulation and the
%            pulsating heart: a mathematical model" Am J Physiol. 1998 Nov;275(5 Pt 2):H1733-47
%

%if (~exist('Vt'))
%    global_declarations;
%end

% Note: elements in dydt below must be consistent with the definition of initial
%       condition vector y0 (in Solve_ODE.m)

% dydt =  
%     dydt(1)    Ppa   Eq. 1
%         (2)    Fpa   Eq. 2
%         (3)    Ppp   Eq. 3
%         (4)    Ppv   Eq. 4
%         (5)    Psa   Eq. 5  (in closed-loop: Pcs = Psa, p. H1745)
%         (6)    Fsa   Eq. 6
%         (7)    Psp   Eq. 7
%         (8)    Pev   Eq. 8 
%         (9)    Vt    Eq. 11
%         (10)   Pla   Eq. 12
%         (11)   Vlv   Eq. 14
%         (12)   CSI   Eq. 21
%         (13)   Pra   Eq. 23
%         (14)   Pbar  Eq. 30
%         (15)   Delta Emax_rv Eq. 36 (for THETA = Emax,rv) 
%         (16)   Delta Emax_lv Eq. 36 (for THETA = Emax,lv) 
%         (17)   Delta Rsp     Eq. 36 (for THETA = Rsp) 
%         (18)   Delta Rep     Eq. 36 (for THETA = Rep) 
%         (19)   Delta Vu_sv   Eq. 36 (for THETA = Vu,sv) 
%         (20)   Delta Vu_ev   Eq. 36 (for THETA = Vu,ev) 
%         (21)   DEL_Ts Eq. 39  
%         (22)   DEL_Tv Eq. 41
%         (23)   Vrv    Eq. 25
%         (24)   fcs    [needed for delayed terms, in Eq. 35]
%         (25)   fes    [needed for delayed terms, in Eq. 35]
%         (26)   fev    [needed for delayed terms, in Eq. 40]

%------------------------------------------------

% ========= Efferent Paths (actuators) ====================================
% sigma Emax,rv - Eq. 35
%   y(25) = fes     |  D_Emax_rv = lags(2) ---> fes(t-D_Emax_rv) = Z(25, 2)
if( Z(25,2)>=modelParams.fes_min ) 
    sigma_Emax_rv = modelParams.G_Emax_rv*log(Z(25,2)-modelParams.fes_min+1);
else
    sigma_Emax_rv = 0;
end
% theta (Emax,rv) - Eq. 36
%                   y(15) = Delta Emax_rv
dydt(15) = (1/modelParams.tau_Emax_rv)*(-y(15) + sigma_Emax_rv);
Emax_rv  = y(15) + modelParams.Emax_rv0;

% sigma Emax,lv - Eq. 35
%   y(25) = fes     |  D_Emax_lv = lags(1) ---> fes(t-D_Emax_lv) = Z(25, 1)
if( Z(25,1)>=modelParams.fes_min ) 
    sigma_Emax_lv = modelParams.G_Emax_lv*log(Z(25,1)-modelParams.fes_min+1);
else
    sigma_Emax_lv = 0;
end
% theta (Emax,lv) - Eq. 36
%                   y(16) = Delta Emax_lv
dydt(16) = (1/modelParams.tau_Emax_lv)*(-y(16) + sigma_Emax_lv);
Emax_lv  = y(16) + modelParams.Emax_lv0;

% sigma Rsp - Eq. 35
%   y(25) = fes     |  D_R_sp = lags(3) ---> fes(t-D_R_sp) = Z(25, 3)
if( Z(25,3)>=modelParams.fes_min ) 
    sigma_R_sp = modelParams.G_R_sp*log(Z(25,3)-modelParams.fes_min+1);
else
    sigma_R_sp = 0;
end
% theta (Rsp) - Eq. 36
%                   y(17) = Delta Rsp
dydt(17) = (1/modelParams.tau_R_sp)*(-y(17) + sigma_R_sp);
Rsp = y(17) + modelParams.Rsp0;

% sigma Rep - Eq. 35
%   y(25) = fes     |  D_R_ep = lags(4) ---> fes(t-D_R_ep) = Z(25, 4)
if( Z(25,4)>=modelParams.fes_min ) 
    sigma_R_ep = modelParams.G_R_ep*log(Z(25,4)-modelParams.fes_min+1);
else
    sigma_R_ep = 0;
end
% theta (Rep) - Eq. 36
%                   y(18) = Delta Rep
dydt(18) = (1/modelParams.tau_R_ep)*(-y(18) + sigma_R_ep);
Rep = y(18) + modelParams.Rep0;

% sigma Vu,sv - Eq. 35
%   y(25) = fes     |  D_Vu_sv = lags(5) ---> fes(t-D_Vu_sv) = Z(25, 5)
if( Z(25,5)>=modelParams.fes_min ) 
    sigma_Vu_sv = modelParams.G_Vu_sv*log(Z(25,5)-modelParams.fes_min+1);
else
    sigma_Vu_sv = 0;
end
% theta (Vu,sv) - Eq. 36
%                   y(19) = Delta Vu,sv
dydt(19) = (1/modelParams.tau_Vu_sv)*(-y(19) + sigma_Vu_sv);
Vu_sv = y(19) + modelParams.Vu_sv0;

% sigma Vu,ev - Eq. 35
%   y(25) = fes     |  D_Vu_ev = lags(6) ---> fes(t-D_Vu_ev) = Z(25, 6)
if( Z(25,6)>=modelParams.fes_min ) 
    sigma_Vu_ev = modelParams.G_Vu_ev*log(Z(25,6)-modelParams.fes_min+1);
else
    sigma_Vu_ev = 0;
end
% theta (Vu,ev) - Eq. 36
%                   y(20) = Delta Vu,ev
dydt(20) = (1/modelParams.tau_Vu_ev)*(-y(20) + sigma_Vu_ev);
Vu_ev = y(20) + modelParams.Vu_ev0;

% sigma Ts  Eq. 38 (added condition: t>=D_Ts)
%   y(25) = fes     |  D_Ts = lags(7) ---> fes(t-D_Ts) = Z(25, 7)
if( Z(25,7)>=modelParams.fes_min )
    SIGMA_Ts = modelParams.G_Ts*log(Z(25,7)-modelParams.fes_min+1);
else
    SIGMA_Ts = 0;
end

% Eq. 39  
%     y(21) = Delta Ts 
dydt(21) = (1/modelParams.tau_Ts)*(-y(21)+SIGMA_Ts);
% sigma Tv  Eq. 40 
%   y(26) = fev     |  D_Tv = lags(8) ---> fev(t-D_Tv) = Z(26, 8)
SIGMA_Tv = modelParams.G_Tv*Z(26,8);

% Eq. 41
%   y(22) = Delta Tv
dydt(22) = (1/modelParams.tau_Tv)*(-y(22)+ SIGMA_Tv);
% Eq. 42
T = y(21) + y(22) + modelParams.T0;

HR = 60 / T;
save HR HR;

% Eq. 22 
Tsys = modelParams.Tsys0-modelParams.Ksys*(1/T);
% Eq. 21 - y(12) = CSI
dydt(12) = 1/T;
u = y(12) - fix(y(12)); % frac[]
% Eq. 19
if( u<=(Tsys/T) )
    phi = sin(u*(pi*T/Tsys))^2;
else
    phi = 0;
end

% Eq. 18
%      y(11) = Vlv
Pmax_lv = phi*Emax_lv*(y(11)-modelParams.Vu_lv) + (1-phi)*modelParams.P0_lv*(exp(modelParams.Ke_lv*y(11))-1);
% Eq. 16
Rlv = modelParams.Kr_lv*Pmax_lv;
% Eq. 15
if( Pmax_lv<=y(5))   % y(5) = Psa
    Fo_l = 0;
else
    Fo_l = (Pmax_lv-y(5))/Rlv;
end
% Eq. 17
Plv = Pmax_lv - Rlv*Fo_l;
% Eq. 13
if( y(10)<=Plv)    % y(10) = Pla
    Fi_l = 0;
else
    Fi_l = (y(10)-Plv)/modelParams.Rla;
end
% Eq. 14 - y(11) = Vlv
dydt(11) = Fi_l - Fo_l;
% Eq. 12 
%     y(4)  = Ppv
%     y(10) = Pla
dydt(10) = (1/modelParams.Cla)*((y(4)-y(10))/modelParams.Rpv - Fi_l);

% Eq. 11 - y(9) = Vt
%if (t > 30 && t < 40)
%    dydt(9) = -10; % Hemorrhage
%else
    dydt(9) = 0; % assume injection rate I(t)=0 [should be replaced by long-term BP control: modified Guyton-Coleman model]
%end

% Eq. 10
Vu = modelParams.Vu_sa + modelParams.Vu_sp + modelParams.Vu_ep + Vu_sv + Vu_ev + modelParams.Vu_ra + modelParams.Vu_pa + modelParams.Vu_pp + modelParams.Vu_pv + modelParams.Vu_la;

% Eq. 9
%     y(1)  = Ppa
%     y(3)  = Ppp 
%     y(4)  = Ppv
%     y(5)  = Psa 
%     y(7)  = Psp  
%     y(8)  = Pev
%     y(9)  = Vt
%     y(10) = Pla  
%     y(11) = Vlv
%     y(13) = Pra 
%     y(23) = Vrv   
Psv = (1/modelParams.Csv)*(y(9)- modelParams.Csa*y(5) - (modelParams.Csp+modelParams.Cep)*y(7) - modelParams.Cev*y(8) - modelParams.Cra*y(13) - y(23) - modelParams.Cpa*y(1) - modelParams.Cpp*y(3) - modelParams.Cpv*y(4) - modelParams.Cla*y(10) - y(11) - Vu);

% Conservation of mass at Extraspl. Venous Circ - Eq. 8
%                                                 y(7)  = Psp  
%                                                 y(8)  = Pev
%                                                 y(13) = Pra 
%                                                 d(Vu_ev)/dt = d(DELTA Vu_ev)/dt 
dydt(8) = (1/modelParams.Cev)*( (y(7)-y(8))/Rep - (y(8)-y(13))/modelParams.Rev - dydt(20) );

% Conservation of mass at Peripheral Systemic Circ (Csp and Cep) - Eq. 7
%                                                 y(6)  = Fsa
%                                                 y(7)  = Psp  
%                                                 y(8)  = Pev
dydt(7) = (1/(modelParams.Csp+modelParams.Cep))*(y(6) - (y(7)-Psv)/Rsp - (y(7)-y(8))/Rep);

% Balance of forces at Systemic Arteries (Lsa) - Eq. 6
%                                                y(5) = Psa
%                                                y(6) = Fsa
%                                                y(7) = Psp  
dydt(6) = (1/modelParams.Lsa)*(y(5) - y(7) - modelParams.Rsa*y(6));

% Conservation of mass at Systemic Arteries (Csa) - Eq. 5
%                                                   y(5) = Psa
%                                                   y(6) = Fsa
dydt(5) = (1/modelParams.Csa)*(Fo_l-y(6));

% Conservation of mass at Pulmonary Veins (Cpv) - Eq. 4
%                                                 y(3)  = Ppp 
%                                                 y(4)  = Ppv
%                                                 y(10) = Pla  
dydt(4) = (1/modelParams.Cpv)*((y(3) - y(4))/modelParams.Rpp - (y(4)-y(10))/modelParams.Rpv);

% Conservation of mass at Pulmonary Peripheral Circulation (Cpp) - Eq. 3
%                                                 y(2)  = Fpa
%                                                 y(3)  = Ppp 
%                                                 y(4)  = Ppv
dydt(3) = (1/modelParams.Cpp)*(y(2) - (y(3)-y(4))/modelParams.Rpp);

% Balance of forces at Pulmonary Arteries (Lpa) - Eq. 2
%                                                 y(1)  = Ppa 
%                                                 y(2)  = Fpa
%                                                 y(3)  = Ppp 
dydt(2) = (1/modelParams.Lpa)*(y(1) - y(3) - modelParams.Rpa*y(2));

% Isometric pressure in Right Ventricle - Eq. 29
%                                          y(23) = Vrv   
Pmax_rv = phi*Emax_rv*(y(23) - modelParams.Vu_rv) + (1-phi)*modelParams.P0_rv*(exp(modelParams.Ke_rv*y(23))-1);
% Viscous resistance of Right Ventricle - Eq. 27
Rrv = modelParams.Kr_rv * Pmax_rv;
% Cardiac output from Right Ventricle - Eq. 26
if( Pmax_rv<=y(1))    % y(1)  = Ppa 
    Fo_r = 0;
else
    Fo_r = (Pmax_rv - y(1))/Rrv;
end
% Inst. Right Ventricle Pressure - Eq. 28
Prv = Pmax_rv - Rrv*Fo_r;

% Blood flow entering Right Ventricle - Eq. 24
if(y(13)<=Prv)    % y(13)=Pra
    Fi_r = 0;
else
    Fi_r = (y(13) - Prv)/modelParams.Rra;
end

% Conservation of mass at Pulmonary Arteries (Cpa) - Eq. 1
%                                                 y(2)  = Fpa
dydt(1) = (1/modelParams.Cpa)*(Fo_r-y(2));


% Conservation of mass at Right Ventricle - Eq. 25
%                                           y(23) = Vrv
dydt(23) = Fi_r - Fo_r;
% Conservation of mass at Right Atrium - Eq. 23
%                                        y(8) = Pev
%                                        y(13) = Pra
dydt(13) = (1/modelParams.Cra)*((Psv-y(13))/modelParams.Rsv + (y(8)-y(13))/modelParams.Rev - Fi_r);


% Carotid baroreflex control - Eq. 30
%     y(5)  = Psa = Pcs
%     y(14) = Pbar
%
dydt(14) = (1/modelParams.tau_p)*(y(5)+modelParams.tau_z*dydt(5) - y(14));


%==================== Carotid Baroreflex Control System ===================

% Eq. 31
%     y(14) = Pbar
% fcs = (fmin + fmax*exp((y(14)-Pn)/Ka))/(1+exp((y(14)-Pn)/Ka));

% Efferent Sympathetic path - Eq. 33  
% fes = fes_Inf + (fes0-fes_Inf)*exp(-Kes*fcs);

% Efferent Vagal path - Eq. 34  
% fev = (fev0 + fev_Inf*exp((fcs-fcs0)/Kev))/(1+exp((fcs-fcs0)/Kev));


% Differential equations introduced to handle delayed terms (in Eq. 35 and Eq. 40) using DDE in Matlab toolbox
%                                      y(14) = Pbar
%                                      y(24) = fcs 
dydt(24) = ((modelParams.fmax*exp((y(14)-modelParams.Pn)/modelParams.Ka)*(1/modelParams.Ka)*dydt(14))*(1+exp((y(14)-modelParams.Pn)/modelParams.Ka)) - (exp((y(14)-modelParams.Pn)/modelParams.Ka)*(1/modelParams.Ka)*dydt(14))*(modelParams.fmin + modelParams.fmax*exp((y(14)-modelParams.Pn)/modelParams.Ka)))/((1+exp((y(14)-modelParams.Pn)/modelParams.Ka))^2);
%                                      y(25) = fes 
dydt(25) = (modelParams.fes0-modelParams.fes_Inf)*exp(-modelParams.Kes*y(24))*(-modelParams.Kes)*dydt(24);
%                                      y(26) = fev 
dydt(26) = ((modelParams.fev_Inf*exp((y(24)-modelParams.fcs0)/modelParams.Kev)*(1/modelParams.Kev)*dydt(24))*(1+exp((y(24)-modelParams.fcs0)/modelParams.Kev)) - (exp((y(24)-modelParams.fcs0)/modelParams.Kev)*(1/modelParams.Kev)*dydt(24))*(modelParams.fev0 + modelParams.fev_Inf*exp((y(24)-modelParams.fcs0)/modelParams.Kev)))/((1+exp((y(24)-modelParams.fcs0)/modelParams.Kev))^2);



dydt = dydt';

