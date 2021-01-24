function f = modelFit(params)

load output output;
load mdl1 mdl1;
load mdl2 mdl2;
load mdl3 mdl3;
load mdl4 mdl4;
load mdl5 mdl5;

%o1 = predict(mdl1,params); % Systolic BP
%o2 = predict(mdl2,params); % Diastolic BP
%o3 = predict(mdl3,params); % Heart rate
%o4 = predict(mdl4,params); % Ejection fraction
%o5 = predict(mdl5,params); % LV volume

o1 = predict(mdl1,[params, params.^2]); % Systolic BP
o2 = predict(mdl2,[params, params.^2]); % Diastolic BP
o3 = predict(mdl3,[params, params.^2]); % Heart rate
o4 = predict(mdl4,[params, params.^2]); % Ejection fraction
o5 = predict(mdl5,[params, params.^2]); % LV volume

xnorm = sum((params-1).^2);

penalty = 1;

f = (10*(output(1) - o1)/60).^2 + ...
    (10*(output(2) - o2)/120).^2 + ...
    ((output(3) - o3)/60).^2 + ...
    ((output(4) - o4)/0.5).^2 + ...
    ((output(5) - o5)/120).^2 + ...    
    penalty * sum((params - 1).^2);

misfit = ((output(1) - o1)/60).^2 + ...
    ((output(2) - o2)/120).^2 + ...
    ((output(3) - o3)/60).^2 + ...
    ((output(4) - o4)/0.5).^2 + ...
    ((output(5) - o5)/120).^2;

end