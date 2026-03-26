function cycle = extract_stable_cycle(x, minLen, maxLen)
% x: vector mono
% devuelve un ciclo (vector) entre cruces por cero ascendentes

x = x(:);
x = x - mean(x);                    % quitar DC

% cruces por cero ascendentes
zc = find(x(1:end-1) <= 0 & x(2:end) > 0);
if numel(zc) < 2
    cycle = x(1:min(numel(x), maxLen));
    return
end

bestJ = inf;
best = [];

alpha = 1.0;
beta  = 1.0;

for i = 1:numel(zc)-1
    a = zc(i);
    b = zc(i+1);        % siguiente cruce ascendente
    L = b - a;
    if L < minLen || L > maxLen
        continue
    end

    c = x(a:b-1);

    % coste de cierre valor + pendiente
    dv = abs(c(1) - c(end));
    ds = abs((c(2)-c(1)) - (c(end)-c(end-1)));
    J  = alpha*dv + beta*ds;

    % opcional: penalizar ciclos de energía baja
    % J = J / (rms(c) + 1e-9);

    if J < bestJ
        bestJ = J;
        best = c;
    end
end

if isempty(best)
    % fallback: primer tramo válido o recorte simple
    a = zc(1);
    b = min(a+maxLen, numel(x));
    best = x(a:b);
end

cycle = best;
end
