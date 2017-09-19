% Autor: Milan Augustin, xaugus09
% Zdroje: studijne materialy ISS (pomocne funkcie...)
% 
% Vo vypoctoch sa pouziva pevnych 320 samples ako subsignal, preto
%  by mal byt vstupny signal rovny nasobku 320*n, kde n je cele cislo

close all;
% 1. Uloha
    [signal, Fs] = audioread('xaugus09.wav'); signal = signal';
    Fs; % Vzorkovacia frekvencia
    t = (0:(length(signal)-1))/Fs;
    NumOfSamples = length(t); % Dlzka vo vzorkach
    Time = NumOfSamples / Fs; % Dlzka v sekundach

% 2. Uloha
    hm = Time * Fs; wf = 1;
    x = signal(wf:hm);
    N = length(x);
    G = 10 * log10(1/N * abs(fft(x)).^2);
    f = (0:N/2-1)/N * Fs; G = G(1:N/2);
    figure; plot (f,G); xlabel('f [Hz]'); ylabel ('PSD [dB]'); grid;

% 3. Uloha
    origos = find(G == max(G)); % najde poziciu maxima
    max_origos = f(origos);

% 4. Uloha
    b = [0.2324 -0.4112 0.2324]; a = [1 0.2289 0.4662];
    figure; zplane(b,a);
    stabilita = roots(a);
    if (isempty(stabilita) | abs(stabilita) < 1)
        disp('Stabilny filter');
    else
        disp('Nestabilny filter');
    end

% 5. Uloha
    H = freqz(b,a,256); f=(0:255) / 256 * Fs / 2;
    figure; plot(f, abs(H)); xlabel('f [Hz]'); ylabel('|H(f)|');
    grid; axis([0 8000 0 1.1]);

% 6. Uloha
    sf = filter(b, a, signal);
    hm = Time * Fs; wf = 1;
    xf = sf(wf:hm);
    N = length(xf);
    Gf = 10 * log10(1/N * abs(fft(xf)).^2);
    f = (0:N/2-1)/N * Fs; Gf = Gf(1:N/2);
    figure; plot (f,Gf); xlabel('f [Hz]'); ylabel ('PSD [dB]'); grid;
    %soundsc(signal, Fs); soundsc(sf, Fs);

% 7. Uloha
    filtered = find(Gf == max(Gf)); % najde poziciu maxima
    max_filtered = f(filtered);

% 8. Uloha
    figure;
    tt = 0.00001:1/Fs:0.02; % vytvori krasnych 40 period [1 1 1 1 -1 -1 -1 -1] vo funkcii square
        % zaciatok krokovania 0.00001, pretoze pri 0 sa vytvori "skaredy" signal
        % a dlzka signalu je potom 321 vzoriek
    rect = square(2*pi*2000*tt);
    s = 0:1/Fs:0.01999; % 320 samples; spravna casova os 20ms od nuly
    values = zeros(1,NumOfSamples-319);
    subplot(211); plot(s, rect); axis([0 0.02 -1.05 1.05]); title('20ms of rectangle impulses');
    for n = 1:NumOfSamples-319,
        piece = signal(n:n+319);
        rp = piece.*rect;
        values(n) = sum(rp);
    end
    s = 1:NumOfSamples-319;
    subplot(212); plot(s, values); title('Hodnota podobnosti kazdeho 20ms subsignalu');
    index_subsignal = find(abs(values) == max(abs(values))); % najde index maxima => podobnosti, dufam
    position = index_subsignal/Fs; % pozicia v sekundach

% 9. Uloha
    Rv = xcorr(signal, 'biased'); k = -50:50;
    Rx = Rv(NumOfSamples-50:NumOfSamples+50); % NumOfSamples == index 0
    figure; plot(k, Rx); xlabel('k');

% 10. Uloha
    R10 = Rx(51+10); % Rv(NumOfSamples+10)

% 11. Uloha
    xmin = min(signal); xmax = max(signal);
    x = linspace(xmin, xmax, 50); x = x(:);
    L = length(x); N = length(signal);
    bigx = repmat(x, 1, N);
    bigy = repmat(signal, L, 1);
    [dummy,ind1] = min(abs(bigy - bigx));
    ind2 = ind1(11:N);
    h = zeros(L, L);
    for ii = 1:N-10,
        d1 = ind1(ii);   d2 = ind2(ii); 
        h(d1, d2) = h(d1, d2) + 1; 
    end
    surf = (x(2) - x(1))^2;
    p = h / N / surf;
    figure; bar3(p); axis([0 51 0 51 0 3]); xlabel('x1'); ylabel('x2');

% 12. Uloha
    check = sum(sum(p)) * surf;
    disp(['hist2: check -- 2d integral should be 1 and is ' num2str(check)]);

% 13. Uloha
    x = x(:); X1 = repmat(x, 1, L);
    x = x'; X2 = repmat(x, L, 1);
    r = sum(sum(X1 .* X2 .* p)) * surf;
    R10_2 = r;
