function [ld_spec,d] = mesh_dists(points,ref_surface)
%function ld_spec = mesh_dists(points,ref_surface)
%ld_spec is the distance of each input point from reference
%surface model, defined by elements and nodes

d=zeros(length(points),4);
d(:,:) = nan;
n_close=zeros(length(points),1);
d2 = 10000;
ref_surface.elems = ref_surface.elements;

for point_loop=1:length(points)
    my_point = points(point_loop,:);
    %  Find nearest node to point
    dist = (repmat(my_point,length(ref_surface.nodes),1))-ref_surface.nodes;
    dist = sqrt((dist(:,1).^2)+(dist(:,2).^2)+(dist(:,3).^2));
    d(point_loop,1) = min(dist);
    temp = find(dist==d(point_loop,1));
    n_close(point_loop) = temp(1);
    
    
    
    %find elems which contain our closest node;
    [elems_of_interest,b] = find(ref_surface.elems == n_close(point_loop));
    
    % Calculate distances to edges and to surface of surrounding elements
    for e_loop =1:length(elems_of_interest)
        e = elems_of_interest(e_loop);
       
        a=ref_surface.nodes(ref_surface.elems(e,1),:)-my_point;         %vector from point to first node on element
        b=ref_surface.nodes(ref_surface.elems(e,2),:)-my_point;         %vector from point to second node on element
        c=ref_surface.nodes(ref_surface.elems(e,3),:)-my_point;         %vector from point to third node on element
        ab=ref_surface.nodes(ref_surface.elems(e,2),:)-ref_surface.nodes(ref_surface.elems(e,1),:);     %vector from first node to second node on element
        bc=ref_surface.nodes(ref_surface.elems(e,3),:)-ref_surface.nodes(ref_surface.elems(e,2),:);     %vector from second node to third node on element
        ca=ref_surface.nodes(ref_surface.elems(e,1),:)-ref_surface.nodes(ref_surface.elems(e,3),:);     %vector from first node to first node on element
        
        % Compute normal distances to lines if intersect within line
        if (dot(a,ab)<=0 && dot(b,ab)>=0) 
            dp2=abs(norm(cross(ab,a))/norm(ab));
            if ~(dp2 >= d(point_loop,2))
                d(point_loop,2)=dp2;
            end
        end
        if (dot(b,bc)<=0 && dot(c,bc)>=0)
            dp2=abs(norm(cross(bc,b))/norm(bc));
            if ~(dp2 >= d(point_loop,2))
                d(point_loop,2)=dp2;
            end
        end
        if (dot(c,ca)<=0 && dot(a,ca)>=0)
            dp2=abs(norm(cross(ca,c))/norm(ca));
            if ~(dp2 >= d(point_loop,2))
                d(point_loop,2)=dp2;
            end
        end
        % Compute normal distance to plane
        n=cross(ab,bc)/norm(cross(ab,bc));
        magd=dot(b,n);
        % Find vectors of point of intersection
        ia=a-magd*n;
        ib=b-magd*n;
        ic=c-magd*n;
        % Check whether intersect is inside triangle
        t1=sign(dot(cross(ia,ib),n));
        t2=sign(dot(cross(ib,ic),n));
        t3=sign(dot(cross(ic,ia),n));
        if (t1==t2 && t2==t3)
            d(point_loop,3)=abs(magd);
            d(point_loop,4) = e;
        end
    end
end
dmin=(min(d(:,1:3)')');
ld_spec = dmin;
d = d(:,4);
% ref_surface.node_colours = ld_spec;
% figure
% plot_surface_model(ref_surface);




function i=find_unique_tris(faces)
% FIND_UNIQUE_TRIS  Finds unique triangular element faces
%
% function i=find_unique_tris(faces)
%
% This function finds those triangular faces which are not coindicent with any others
% These are the only faces which will be visible in a graphics plot of the model
%
% faces is a n x 3 matrix containing 'n' triplets of node numbers for 'n' triangular faces
% i gives the indices of the unique faces - faces(i,:) gives the unique faces

if isempty(faces)
    i=[];
    return;
end
s=sort(faces,2);												% sort the nodes in each triplet
[s,i]=sortrows(s);											% sort the triplets
s=diff(s);														% identical faces result in zero differential
s=[1;s(:,1)|s(:,2)|s(:,3);1];
s=s(1:size(s,1)-1) & s(2:size(s,1));			% s=0 for identical faces, s=1 for unique faces
i=i(s);																% extract the unique faces



function [tris]=get_faces_tets(elems)
% [tris]=get_faces_tets(elems);
%
% Get all triangular faces of four node tetrahedra
%  November 2004 DRH, based on DMJ code for ANSYS 3D elements

tris=[elems(:,[1 2 3]);elems(:,[1 4 2]);elems(:,[1 3 4]);elems(:,[2 4 3])];



function ref_surface=get_surface_tris(nodes,elems,components)
% ref_surface=get_surface_tris(nodes,elems,components)
% Extract a surface model from the full 3D volume model
%
% surface_tris.nodes = surface nodes
% surface_tris.elems = triangular surface faces
%
% nodes = 3D model nodes
% elems = 3D model elems
%
% 3D model elems are tetrahedra, specified as list of nodes
%
% DRH November 2004 base on DMJ code
%
[tris]=get_faces_tets(elems);                               % find all element surface triangles
ref_surface.elems=tris(find_unique_tris(tris),:);             % find surface triangles
surf_node_map=zeros(size(nodes,1),1);
surf_node_map([ref_surface.elems(:)])=1;          
surf_node_numbers=find(surf_node_map);                      % find out the surface nodes
surf_node_map(find(surf_node_map))=1:nnz(surf_node_map);    % find out numbering map for all nodes->surf nodes

ref_surface.nodes=nodes(surf_node_numbers,:);
ref_surface.elems=surf_node_map(ref_surface.elems);             % renumber triangle face connectivity
%
if exist('components','var')
    for a=1:length(components)
        components(a).nodes=surf_node_map(components(a).nodes);
    end
    ref_surface.components=components;
end





function h=plot_surface_model(surf_model)
% h=plot_surface_model(surf_model)
%
% David Jones 19 March 2004
% edited december 2004 by ADJ to cope with surface models without elems3 or
% elems4 defined.
%
h=[];

if ~isfield(surf_model,'elems3')
    surf_model.elems3 = [];
end
if ~isfield(surf_model,'elems4')
    surf_model.elems4 = [];
end

if isfield(surf_model,'elems')
    surf_model.elems3 = surf_model.elems;
end

if isfield(surf_model,'elements')
    surf_model.elems3 = surf_model.elements;
end


if ~isempty(surf_model.elems3)
    if isfield(surf_model,'node_colours')
        for loop = 1:length(surf_model.elems3)
            elem_colours(loop) = (surf_model.node_colours(surf_model.elems3(loop,1))+surf_model.node_colours(surf_model.elems3(loop,2))+surf_model.node_colours(surf_model.elems3(loop,3)))/3;
        end
               
        xc=surf_model.nodes(surf_model.elems3)';
        yc=surf_model.nodes(surf_model.elems3+size(surf_model.nodes,1))';
        zc=surf_model.nodes(surf_model.elems3+2*size(surf_model.nodes,1))';
        h(end+1)=patch(xc,yc,zc,elem_colours);
        hold on
    else
        xc=surf_model.nodes(surf_model.elems3)';
        yc=surf_model.nodes(surf_model.elems3+size(surf_model.nodes,1))';
        zc=surf_model.nodes(surf_model.elems3+2*size(surf_model.nodes,1))';

        h(end+1)=patch(xc,yc,zc,'b');
        hold on
    end
end

if ~isempty(surf_model.elems4)
    xc=surf_model.nodes(surf_model.elems4)';
    yc=surf_model.nodes(surf_model.elems4+size(surf_model.nodes,1))';
    zc=surf_model.nodes(surf_model.elems4+2*size(surf_model.nodes,1))';
    h(end+1)=patch(xc,yc,zc,'b');
    hold on
end

if isfield(surf_model,'components')
    plsmb='s+^ox';plcol='mycgrbk';
    for a=1:length(surf_model.components)
        cnodes=surf_model.components(a).nodes;
        plotstr=[plcol(floor((a-1)/length(plsmb))+1),plsmb(rem(a-1,length(plsmb))+1)];
        plot3(surf_model.nodes(cnodes,1),surf_model.nodes(cnodes,2),surf_model.nodes(cnodes,3),plotstr);
        hold on
        legstr{a}=surf_model.components(a).name;
        legstr{a}=strrep(legstr{a},'_',' ');
    end    
    
    if exist('legstr')
        legend(legstr);
    end
    
end
hold off
