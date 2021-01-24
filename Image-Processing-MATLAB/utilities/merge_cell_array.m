%% Sort and Merge cell arrays

% cellA = {[9,17],[35,38]};
% cellB = {[38,39],[45,49],[52,69]};
cellA = {};
cellA{1}(1,2) = cellA{1}(1,1)+1;
cellB = {[1,1]};
if isempty(cellA); disp('empty cell'); end
segP_endpts = [cellA, cellB];
startIdx = cellfun(@(x)x(1,1),segP_endpts);
[sortedValues, sortOrder] = sort(startIdx);
segP_endpts = segP_endpts(sortOrder);

%% Check for very short parallel segments and rectify them
% Short segments makes the reconstruction inefficient so need to be
% extended by adding neighbouring points on either side of them.
% Care should be taken if there is any overlap between the
% segments. If so then merge segments.
% If there is only one point in the segment (same end points) then
% delete the segment.

has_overlap = 1;
arraySzInit = size(segP_endpts,2);
while has_overlap
    for i=1:size(segP_endpts,2)
        if isempty(segP_endpts{i}); continue; end
        e1 = segP_endpts{i}(1,1); % end point1
        e2 = segP_endpts{i}(1,2); % end point2
        if e1==e2 % if end points are same then empty the cell content
            segP_endpts{i} = [];
            continue;
        end
        while e2-e1 < 9 % extend short segments
            e1 = e1 - 1;
            if e1==0; e1 = 1; end
            e2 = e2 + 1;
            %if e2==size(ptBcorInt,1); e2 = e2-1; end
        end
        % merge with previous segment if conditions are met
        if i~=1 && ~isempty(segP_endpts{i-1}) && e1 <= segP_endpts{i-1}(1,2)+1
            e1 = segP_endpts{i-1}(1,1);
            segP_endpts{i-1} = [];
        end
        % merge with next segment if conditions are met
        if i~=size(segP_endpts,2) && e2 >= segP_endpts{i+1}(1,1)-1
            if e2 < segP_endpts{i+1}(1,2)
                e2 = segP_endpts{i+1}(1,2);
            end
            segP_endpts{i+1} = [];
        end
        % update segments
        segP_endpts{i}(1,1) = e1; % end point1
        segP_endpts{i}(1,2) = e2; % end point2
    end

    % Delete the empty cells, if any from above check
    segP_endpts(cellfun('isempty',segP_endpts)) = [];
    
    if size(segP_endpts,2) == arraySzInit
        has_overlap = 0;
    end
    arraySzInit = size(segP_endpts,2);
end
