function [CBF_L,CBF_R] = simulateCA(HR,BP,age,isFemale,d_ICAL,d_ICAR,vmin_ICAL,vmin_ICAR)

% Parameters for elderly subjects
aE = 2.5;
bE = 0.28;
cE = 2.71;
ME = 1.00;

% Parameters for middle-aged subjects
aM = 0.438;
bM = 0.01;
cM = 0.264;
MM = 1.2;

if (age < 35)
    a = aM; b = bM; c = cM; M = MM;
elseif (age > 75)
    a = aE; b = bE; c = cE; M = ME;
else
    lambda = (75-age)/(75-35);
    a = aM * lambda + (1 - lambda) * aE;
    b = bM * lambda + (1 - lambda) * bE;
    c = cM * lambda + (1 - lambda) * cE;
    M = MM * lambda + (1 - lambda) * ME;
end

f_s = mean(BP); % Initial values fitting the MAP
f_aut = 2.03e-6 * f_s.^3 - 6.02e-4 * f_s.^2 + 5.94e-2 * f_s - 1.95;
d = b * c * f_aut / (M * c * f_s - (a + c) * f_aut);

init_v(1) = f_s * (b * c + a * d) / (b * c + (a + c) * d);
init_v(2) =(b * c * f_s) / (b * c + (a + c) * d);
[t,v] = ode23s(@OlufsenModel,0:0.0071:20*60/HR,init_v,[],HR,BP,age);

% Driving force is equal to the arterial pressure
for n=1:length(t)   
    f_s(n) = ArterialPressure(t(n),HR,BP);
    %f_s(n) = ArterialPressureFiltered(t(n));
end

%d_ICAL    = 0.46;
%d_ICAR    = 0.45;
%vmin_ICAL = 15;
%vmin_ICAR = 22;
[Vbas_L,Vbas_R] = EstimateMCABaseline(d_ICAL, d_ICAR, vmin_ICAL, vmin_ICAR, age, isFemale);

Vdyn = M * (f_s' - v(:,1));
V_L  = Vbas_L + Vdyn; 
V_R  = Vbas_R + Vdyn; 

% Truncate to last heartbeat
hb_length = floor(60/HR/0.0071);
CBF_L = V_L(18*hb_length+1:18*hb_length+100);
CBF_R = V_R(18*hb_length+1:18*hb_length+100);