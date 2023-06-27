T = [10, 30, 100, 200];
%T = [10, 0, 100, -200];
%T =[0 ,-10,-100,-200];
%T =[0 ,-10,-100,200];
v1 = [T(1); T(2)]; 
v2 = [T(3); T(4)]; 
tvec = v2 - v1; 
length = norm(tvec); 
tvec = tvec/length;
nvec = [-tvec(2);tvec(1)];
sprintf(" << %f << %f << %f << %f << %f;", tvec(1), tvec(2), nvec(1), nvec(2), length)