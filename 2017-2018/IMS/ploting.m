clear all;
load runtime.out;
typA = runtime(1:2:length(runtime));
typB = runtime(2:2:length(runtime));
os = [1:length(runtime)/2];
figure;
subplot(211); plot(typA); xlabel("Hodin"); ylabel("Kusy"); title("Typ A");
subplot(212); plot(typB); xlabel("Hodin"); ylabel("Kusy"); title("Typ B");
