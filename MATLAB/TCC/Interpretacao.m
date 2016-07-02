classdef Interpretacao < matlab.System
    % Untitled3 Add summary here
    %
    % This template includes the minimum set of functions required
    % to define a System object with discrete state.

    properties
        % Public, tunable properties.
    end

    properties (DiscreteState)
    end

    properties (Access = private)
        % Pre-computed constants.
    end

    methods (Access = protected)
        function setupImpl(obj,dist, ang, vrel)
            % Implement tasks that need to be performed only once, 
            % such as pre-computed constants.
        end

        function [dist, vrel, ang, vmax] = stepImpl(obj, vrels, dists, angs)
            [dist, idx] = min(dists);
            ang = angs(idx);
            vrel = vrels(idx);
            vmax = 30;
        end

        function resetImpl(obj)
            % Initialize discrete-state properties.
        end
    end
end
