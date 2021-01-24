% Implementation of model from Mader, Olufsen and Mahdi 2014
function dv = OlufsenModel(t,v,HR,BP,age)

v1 = v(1,:);
v2 = v(2,:);

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

f_s = ArterialPressure(t,HR,BP);
%f_s = ArterialPressureFiltered(t);

% Cerebral autoregulation curve, cubic polynomial fit
f_aut = 2.03e-6 * f_s.^3 - 6.02e-4 * f_s.^2 + 5.94e-2 * f_s - 1.95;

d = b * c * f_aut / (M * c * f_s - (a + c) * f_aut);

% Equations for a two-element viscoelastic model
dv(1,:) = -(a + b + c) * v1 + (c - d) .* v2 + (a + b) * f_s;
dv(2,:) = -b * v1 - d .* v2 + b * f_s;