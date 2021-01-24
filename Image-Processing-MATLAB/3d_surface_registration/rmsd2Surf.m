function [ rmsDist, minDist, maxDist, meanDist, TvertMov,...
    minDistVec ] = rmsd2Surf( dof6, scaleXYZ, vertMov, vertFix, calcParams )
% RMSD2SURF    Computes RMS of distances between two surfaces.
%
%   The RMS of distances is calculated after applying the transformation
%   that is calculated using the 6 degrees of freedom (dof6): 3 Euler
%   angles for rotation + 3 translation parameters, and the scaling factors
%   scaleXYZ.
%
%   Author: Vignesh Rammohan, 06-February-2018
%
%   Mathematical Modelling in Medicine Group
%   Department of Infection, Immunity and Cardiovascular Disease
%   The University of Sheffield, Sheffield, United Kingdom.

    rx = dof6(1); ry = dof6(2); rz = dof6(3);
    tx = dof6(4); ty = dof6(5); tz = dof6(6);
    Sxyz = [scaleXYZ(1) 0 0
            0 scaleXYZ(2) 0
            0 0 scaleXYZ(3)];
    
    Rx=[1 0 0
        0 cos(rx) -sin(rx)
        0 sin(rx) cos(rx)];
    Ry=[cos(ry) 0 sin(ry)
        0 1 0
        -sin(ry) 0 cos(ry)];
    Rz=[cos(rz) -sin(rz) 0
        sin(rz) cos(rz) 0
        0 0 1];

    rotmXYZ = Sxyz*Rx*Ry*Rz;
    
    % Transformation matrix to apply, in order to register the intended
    % surface to the fixed one
    T = [[rotmXYZ, [tx; ty; tz]]; 0 0 0 1];
    vertMov(:,4) = ones(size(vertMov,1),1);
    T_vmov = T*vertMov';
    TvertMov = T_vmov(1:3,:)';

    % Nearest neighbour search for each point in the moving surface to that
    % of fixed surface
    distSq = zeros(1,size(vertFix,1));
    minDistVec = zeros(1,size(T_vmov,2));
    for i = 1:size(T_vmov,2)
        for j = 1:size(vertFix,1)
            distSq(j) = (T_vmov(1,i)-vertFix(j,1))^2 ...
                        + (T_vmov(2,i)-vertFix(j,2))^2 ...
                        + (T_vmov(3,i)-vertFix(j,3))^2;
        end
        minDistVec(i) = sqrt(min(distSq));
    end
    
    % rms distance of each point in the moved surface to the nearest point
    % in the fixed surface
    rmsDist = norm(minDistVec)/sqrt(length(minDistVec)); % sqrt(sum(minDistVec.^2))/sqrt(size(minDistVec,1));
    minDist = 0; maxDist = 0; meanDist = 0;
    if calcParams
        minDist = norm(minDistVec, -Inf);
        maxDist = norm(minDistVec, Inf);
        meanDist = mean(minDistVec); % norm(minDistVec, 1)/length(minDistVec);
    end
    
end

%%%%%%%%%%%%% ARCHIVED %%%%%%%%%%%
%     xPts = zeros(2,size(vertFix,1));
%     yPts = zeros(2,size(vertFix,1));
%     zPts = zeros(2,size(vertFix,1));
%     tic
%     for i = 1:size(T_vmov,2)
% %         xPts(1,:) = T_vmov(1,i);
% %         xPts(2,:) = vertFix(:,1)';
% %         yPts(1,:) = T_vmov(2,i);
% %         yPts(2,:) = vertFix(:,2)';
% %         zPts(1,:) = T_vmov(3,i);
% %         zPts(2,:) = vertFix(:,3)';
% %         distSq2 = sum([diff(xPts).^2; diff(yPts).^2; diff(zPts).^2]);
%         xyzPts = zeros(6,size(vertFix,1));
%         xyzPts(1,:) = T_vmov(1,i);
%         xyzPts(3,:) = T_vmov(2,i);
%         xyzPts(5,:) = T_vmov(3,i);
%         xyzPts(2:2:6,:) = vertFix';
%         diffxyz = diff(xyzPts).^2;
%         distSq2 = sum(diffxyz(1:2:5,:));
%         minDistSq2(i) = min(distSq2);
%     end
%     toc
