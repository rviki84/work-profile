function f = modelFit_OpenLoop(params)

load output output;
load mdl1_OpenLoop mdl1_OpenLoop;
load mdl2_OpenLoop mdl2_OpenLoop;
load mdl3_OpenLoop mdl3_OpenLoop;
load mdl4_OpenLoop mdl4_OpenLoop;
load mdl5_OpenLoop mdl5_OpenLoop;

%o1 = predict(mdl1_OpenLoop,params); % Systolic BP
%o2 = predict(mdl2_OpenLoop,params); % Diastolic BP
%o3 = predict(mdl3_OpenLoop,params); % Heart rate
%o4 = predict(mdl4_OpenLoop,params); % Ejection fraction
%o5 = predict(mdl5_OpenLoop,params); % LV volume

o1 = predict(mdl1_OpenLoop,[params, params.^2]); % Systolic BP
o2 = predict(mdl2_OpenLoop,[params, params.^2]); % Diastolic BP
o3 = predict(mdl3_OpenLoop,[params, params.^2]); % Heart rate
o4 = predict(mdl4_OpenLoop,[params, params.^2]); % Ejection fraction
o5 = predict(mdl5_OpenLoop,[params, params.^2]); % LV volume

xnorm = sum((params-1).^2);

penalty = 1;

f = (10*(output(1) - o1)/60).^2 + ...
    (10*(output(2) - o2)/120).^2 + ...
    ((output(4) - o4)/0.5).^2 + ...
    ((output(5) - o5)/120).^2 + ...    
    penalty * sum((params - 1).^2);

misfit = ((output(1) - o1)/60).^2 + ...
    ((output(2) - o2)/120).^2 + ...
    ((output(4) - o4)/0.5).^2 + ...
    ((output(5) - o5)/120).^2;

end