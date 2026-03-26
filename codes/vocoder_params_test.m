function [type,freq,Q,gain,outMin,outMax] = vocoder_params(fmin,fmax,K)
%#codegen

  [type,freq,Q,gain] = fb_params(fmin,fmax,K);
  [outMin,outMax] = ctrl_params_from_freq(freq,K);
end