function fs = ArterialPressureFiltered(t)

alpha = 1;
FilteredPressure = @(s)(ArterialPressure(t) * exp(-alpha * (t-s)));

for n=1:length(t)
    fs(n) = quad(FilteredPressure,0,t(n));
end