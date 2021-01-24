% Load the pre-computed simulation runs
model_params = [ ...   
    0.9000    0.7000    0.7000    0.7000    1.3000
    0.9000    0.7000    0.7000    1.3000    0.7000
    0.9000    0.7000    1.3000    0.7000    0.7000
    0.9000    0.7000    1.3000    1.3000    1.3000
    0.9000    1.3000    0.7000    0.7000    0.7000
    0.9000    1.3000    0.7000    1.3000    1.3000
    0.9000    1.3000    1.3000    0.7000    1.3000
    0.9000    1.3000    1.3000    1.3000    0.7000
    1.1000    0.7000    0.7000    0.7000    0.7000
    1.1000    0.7000    0.7000    1.3000    1.3000
    1.1000    0.7000    1.3000    0.7000    1.3000
    1.1000    0.7000    1.3000    1.3000    0.7000
    1.1000    1.3000    0.7000    0.7000    1.3000
    1.1000    1.3000    0.7000    1.3000    0.7000
    1.1000    1.3000    1.3000    0.7000    0.7000
    1.1000    1.3000    1.3000    1.3000    1.3000
    0.8000    1.0000    1.0000    1.0000    1.0000
    1.2000    1.0000    1.0000    1.0000    1.0000
    1.0000    0.4000    1.0000    1.0000    1.0000
    1.0000    1.6000    1.0000    1.0000    1.0000
    1.0000    1.0000    0.4000    1.0000    1.0000
    1.0000    1.0000    1.6000    1.0000    1.0000
    1.0000    1.0000    1.0000    0.4000    1.0000
    1.0000    1.0000    1.0000    1.6000    1.0000
    1.0000    1.0000    1.0000    1.0000    0.4000
    1.0000    1.0000    1.0000    1.0000    1.6000
    1.0000    1.0000    1.0000    1.0000    1.0000
    1.0000    1.0000    1.0000    1.0000    1.0000
    1.0000    1.0000    1.0000    1.0000    1.0000
    1.0000    1.0000    1.0000    1.0000    1.0000
    1.0000    1.0000    1.0000    1.0000    1.0000
    1.0000    1.0000    1.0000    1.0000    1.0000
    1.0000    1.0000    1.0000    1.0000    1.0000
    1.0000    1.0000    1.0000    1.0000    1.0000
    1.0000    1.0000    1.0000    1.0000    1.0000];

model_outputs = [ ...
   78.4019  112.6312   81.4327    0.4846  106.4362
   80.0083  106.5005   72.7703    0.5970  106.2092
   81.6145  138.8099   78.3335    0.5191  114.2582
   75.4565  108.4085   77.9824    0.5052  102.9446
   81.6416  117.6262   77.4769    0.5549  109.4979
   75.7877   96.9201   78.5210    0.5112  101.5422
   78.3436  122.7444   88.3546    0.4428  108.2204
   82.8492  117.9717   76.0047    0.5788  107.1769
   95.8170  144.0246   70.8599    0.5805  146.2178
   89.9363  123.6183   65.6764    0.5899  145.8574
   91.2472  157.6600   75.7923    0.4927  147.9728
  100.6736  154.5327   67.1491    0.6245  153.6215
   89.6442  131.7327   71.4637    0.5334  142.5660
   94.8852  130.4476   66.4957    0.6473  151.3095
   96.3947  161.7309   74.3827    0.5334  150.7077
   98.3738  142.4009   72.1175    0.5581  146.3647
   67.2574   93.3366   84.3562    0.4893   81.6553
  100.8994  154.1573   66.0538    0.6001  166.0805
   88.5088  134.9664   70.4042    0.5771  131.9969
   89.7705  129.2494   73.5787    0.5654  130.2500
   85.3652  110.4186   68.5585    0.5965  128.8526
   96.2620  151.8793   80.7579    0.5288  133.2464
   65.3532  122.3106   95.8455    0.3378  107.1659
   87.3068  118.8717   69.4869    0.5894  125.4326
   93.4844  141.3441   70.3092    0.6320  136.5284
   84.9532  122.4793   77.0473    0.5016  125.3260
   89.1917  132.0053   70.9297    0.5698  131.0395
   89.1917  132.0053   70.9297    0.5698  131.0395
   89.1917  132.0053   70.9297    0.5698  131.0395
   89.1917  132.0053   70.9297    0.5698  131.0395
   89.1917  132.0053   70.9297    0.5698  131.0395
   89.1917  132.0053   70.9297    0.5698  131.0395
   89.1917  132.0053   70.9297    0.5698  131.0395
   89.1917  132.0053   70.9297    0.5698  131.0395
   89.1917  132.0053   70.9297    0.5698  131.0395];

n = size(model_outputs,1);
 
% Fit five multiple regression models

warning off
indsValid = find((model_outputs(1:n,1) > 0) & ~isnan(model_outputs(1:n,3)));
disp('Fitting for the Diastolic BP...')
%mdl1 = LinearModel.fit(model_params(indsValid ,:), model_outputs(indsValid,1))
mdl1 = LinearModel.fit([model_params(indsValid ,:) model_params(indsValid ,:).^2], model_outputs(indsValid,1));
disp('--------------------------------------------')

disp('Fitting for the Systolic BP...')
%mdl2 = LinearModel.fit(model_params(indsValid ,:), model_outputs(indsValid,2))
mdl2 = LinearModel.fit([model_params(indsValid ,:) model_params(indsValid ,:).^2], model_outputs(indsValid,2));
disp('--------------------------------------------')

disp('Fitting for the Heart Rate...')
%mdl3 = LinearModel.fit(model_params(indsValid ,:), model_outputs(indsValid,3))
mdl3 = LinearModel.fit([model_params(indsValid ,:) model_params(indsValid ,:).^2], model_outputs(indsValid,3));
disp('--------------------------------------------')

disp('Fitting for the Ejection Fraction...')
%mdl4 = LinearModel.fit(model_params(indsValid ,:), model_outputs(indsValid,4))
mdl4 = LinearModel.fit([model_params(indsValid ,:) model_params(indsValid ,:).^2], model_outputs(indsValid,4));
disp('--------------------------------------------')

disp('Fitting for the LV Volume...')
%mdl5 = LinearModel.fit(model_params(indsValid ,:), model_outputs(indsValid,5))
mdl5 = LinearModel.fit([model_params(indsValid ,:) model_params(indsValid ,:).^2], model_outputs(indsValid,5));
disp('--------------------------------------------')
warning on

% Save the surrogate models for further processing
save mdl1 mdl1
save mdl2 mdl2
save mdl3 mdl3
save mdl4 mdl4
save mdl5 mdl5