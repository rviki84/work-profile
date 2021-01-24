function [ eulXYZ, scaleXYZ ] = Tm4x4ToEulerXYZ( Tm4x4, scalingSwitch )
% TM4X4TOEULERXYZ   Computes the Euler angles and the scaling factors from
% the rotational component of the 4x4 transformation matrix.

%Inputs:
    % Tm4x4 = 4x4 transformation matrix of the form,
    %   Tm4x4 = [sxr11 sxr12 sxr13 tx; 
    %            syr21 syr22 syr23 ty;
    %            szr31 szr32 szr33 tz;
    %             0   0   0  1];
    % where tx, ty and tz are translational components,
    % sx, sy and sz are scaling factors
    %
    % scalingSwitch = switch of type 'string' to set scaling factors
    % computation 'ON' or 'OFF'

%Output:
    % eulXYZ = Vector of rotation angles along x, y and z direction in radians
%
%
%   Author: Vignesh Rammohan, 06-February-2018
%
%   Mathematical Modelling in Medicine Group
%   Department of Infection, Immunity and Cardiovascular Disease
%   The University of Sheffield, Sheffield, United Kingdom.

    if strcmp(scalingSwitch,'ON')
        % Compute scaling factor 's' along x, y and z direction
        sx = sqrt(sum(Tm4x4(1,1:3).^2));
        sy = sqrt(sum(Tm4x4(2,1:3).^2));
        sz = sqrt(sum(Tm4x4(3,1:3).^2));
    else
        sx=1; sy=1; sz=1;
    end
    scaleXYZ = [sx, sy, sz];
    
%     x = atan2(-Tm4x4(2,3), Tm4x4(3,3))*(sz/sy);
    x = atan2(-Tm4x4(2,3)/sy, Tm4x4(3,3)/sz);
    cosY = sqrt(1 - (Tm4x4(1,3)/sx)^2);
    y = atan2(Tm4x4(1,3)/sx, cosY);
    sinZ = (cos(x) * Tm4x4(2,1)/sy) + (sin(x) * Tm4x4(3,1)/sz);
    cosZ = (cos(x) * Tm4x4(2,2)/sy) + (sin(x) * Tm4x4(3,2)/sz);
    z = atan2(sinZ, cosZ);
    
    eulXYZ = [x, y, z];
    
end

