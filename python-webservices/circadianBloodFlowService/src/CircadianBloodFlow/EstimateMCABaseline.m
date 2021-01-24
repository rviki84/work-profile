% Reproduce linear relationship between ICA flow rate and MCA flow
% velocity as reported in Lindegaard, K. F., Lundar, T., Wiberg, J., 
% Sjøberg, D., Aaslid, R., & Nornes, H. (1987). Variations in middle 
% cerebral artery blood flow investigated with noninvasive transcranial 
% blood velocity measurements. Stroke, 18(6), 1025-1030.
function [v_MCA_L,v_MCA_R,Q_ICA_L,Q_ICA_R] = EstimateMCABaseline(d_ICAL, d_ICAR, vmin_ICAL, vmin_ICAR, age, isFemale)
% 
% d_MCA     = 0.28;     % MCA diameter [cm]
% fract_MCA = 1; % Fraction of flow ICA->MCA
% 

Q_ICA_L = vmin_ICAL * pi * (d_ICAL/2)^2;
Q_ICA_R = vmin_ICAR * pi * (d_ICAR/2)^2;
%Q_ICA = (Q_ICAL*60+Q_ICAR*60) / 2;

% 
% Q_MCA = fract_MCA * (Q_ICAL + Q_ICAR) / 2;
% 
% v_MCA = Q_MCA / (pi * (d_MCA/2)^2);
% 

if (isFemale == 1)
    v_MCA_L = (1.67 + age * 0.005) * vmin_ICAL;
    v_MCA_R = (1.67 + age * 0.005) * vmin_ICAR;
else
    v_MCA_L = 2 * vmin_ICAL;    
    v_MCA_R = 2 * vmin_ICAR;    
end
