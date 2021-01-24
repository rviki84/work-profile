function [ rmsDist, minDist, maxDist, meanDist, TvertMov,...
    minDistVec, T ] = rmsDistSurfaces_v2( dof6, scaleXYZ, vertMov, surfFix, calcParams )
% RMSDISTSURFACES_V2    Computes RMS of distances between two surfaces.
% 'mesh_dists' is used for nearest point search.
%
%   The RMS of distances is calculated after applying the transformation
%   that is calculated using the 6 degrees of freedom (dof6): 3 Euler
%   angles for rotation + 3 translation parameters, and the scaling factors
%   scaleXYZ.
%   To compute the nearest point search 'mesh_dists' function is used
%   making the process more accurate than before.
%   The minimum, maximum and mean of the distances are also computed in 
%   addition to the RMS distance if the 'calcParams' is set to logical 1.
%   Also the transformed vertices, distance vector and final transformation
%   are available as output to perform further analysis.
%
%   Author: Vignesh Rammohan, 10-November-2020
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
    %%%Tri = delaunayn(unique(vertFix,'rows')); % T is a numt-by-n+1 matrix, a triangulation of vertFix
    %%%[~,minDistVec] = dsearchn(vertFix, Tri, TvertMov);
    
    % minDistVec is the distance of each input point from reference surface model,
    % defined by elements and nodes
    ref_surface.elements = surfFix.faces;
    ref_surface.nodes = surfFix.vertices;
    [minDistVec,~] = mesh_dists(TvertMov,ref_surface);
    
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

