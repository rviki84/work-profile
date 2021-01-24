function fs = ArterialPressure(t,HR,BP)

% Driving force is equal to the arterial pressure

% Original BP sample is from 0.7 s long, extend it to 1.2 s
if (size(BP,1) < size(BP,2))
    BP = BP';
end
BP_extended = [BP; BP(end)+0*(0.7:0.007:1.2)'];
tt = linspace(0,1.2,length(BP_extended));
tmod = mod(t,60/HR);
fs = spline(tt,BP_extended,tmod);