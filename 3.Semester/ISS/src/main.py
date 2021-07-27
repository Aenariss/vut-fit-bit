# -*- coding: UTF-8 -*-
# ISS PROJECT 2020
# AUTHOR: Vojtech Fiala (xfiala61)

import numpy as np
from matplotlib import pyplot as plt
from matplotlib.backends.backend_pdf import PdfPages
import soundfile as sf
from scipy.signal import lfilter, freqz
import sys

save = 0

def arg_parse():
    if len(sys.argv) > 1:
        if sys.argv[1] == '-s':
            global save
            save = 1

def print_combined():
    global save
    s, fs = sf.read('audio/maskon_tone.wav')
    t = np.arange(s.size) / fs
    s = s * 2**15

    plt.figure(figsize=(18,9))
    on, = plt.plot(t, s)
    plt.tight_layout()
    on.set_label("Tón s rouskou")

    s, fs = sf.read('audio/maskoff_tone.wav')
    s = s * 2**15
    t = np.arange(s.size) / fs

    off, = plt.plot(t, s)
    off.set_label("Tón bez rousky")

    plt.gca().set_xlabel('$Čas (s)$')
    plt.gca().set_ylabel('$Frekvence (Hz)$')
    plt.legend(fontsize=15)

    plt.tight_layout()
    if save:
        plt.savefig('combined.pdf')

def print_1sec_segments():
    global save
    if save:
        pp = PdfPages('mask_segments.pdf')
    s, fs = sf.read('audio/maskoff_tone.wav')
    s = s[3000:19000]
    s = s * 2**15
    t = np.arange(s.size) / fs

    plt.figure(figsize=(18,9)) 
    plt.plot(t, s)

    plt.gca().set_title('Mask Off')
    plt.gca().set_xlabel('$Čas (s)$')
    plt.gca().set_ylabel('$Hz$')

    plt.tight_layout()
    if save:
        pp.savefig()

    s, fs = sf.read('audio/maskon_tone.wav')
    s = s[3000:19000]
    s = s * 2**15
    t = np.arange(s.size) / fs

    plt.figure(figsize=(18,9)) 
    plt.plot(t, s)

    plt.gca().set_title('Mask On')
    plt.gca().set_xlabel('$Čas (s)$')
    plt.gca().set_ylabel('$Hz$')

    plt.tight_layout()
    if save:
        pp.savefig()
    if save:
        pp.close()

def framerize(s):
    # s ma 16000 prvku a trva 1 vterinu -> 1 ms ma 16000/1000 prvku -> 16
    # ramec ma mit 20 ms -> 16*20 = 320 prvku
    # ramce se ale prekryvaji, takze jeden ramec ma 0-319, druhy 160-479, treti 320-639... Posledni zahodim
    x = []
    z = 0
    y = 319
    while y < 16000:
        tmp = s[z:y]
        x.append(tmp)
        z += 160
        y += 160
    
    return x

def print_2_frames(frame_on, frame_off):
    global save
    if save:
        pp = PdfPages("2_frames.pdf")
    t = np.arange(len(frame_on)) / 16

    plt.figure(figsize=(15,3)) 
    plt.plot(t, frame_on)

    plt.gca().set_title('Rámec s rouškou')
    plt.gca().set_xlabel('$Čas (ms)$')
    plt.gca().set_ylabel('$y$')

    plt.tight_layout()
    if save:
        pp.savefig()

    t = np.arange(len(frame_off)) / 16

    plt.figure(figsize=(15,3)) 
    plt.plot(t, frame_off)

    plt.gca().set_title('Rámec bez roušky')
    plt.gca().set_xlabel('$Čas (ms)$')
    plt.gca().set_ylabel('$y$')

    plt.tight_layout()
    if save:
        pp.savefig()
    if save:
        pp.close()

def centralize(tone):
    s, fs = sf.read(tone)
    s = s[3000:19000]
    s -= np.mean(s) # ustredneni dle metody ze zadani
    #s /= np.abs(s).max() -- neni potreba, je ustredneno ze zpusobu otevirani

    x = framerize(s)
    return x

def print_frame(status, frame, tone, clip_status, x, result, lag, threshold, fre, fre2):
    global save
    tone = tone[6:-4]
    tone = tone + "_clipping.pdf"
    if save:
        pp = PdfPages(tone)
    t = np.arange(len(frame)) / 16

    _, ax = plt.subplots(4, 1, figsize=(15,12))
    ax[0].plot(t, frame)

    ax[0].set_title('Rámec')
    ax[0].set_xlabel('$Čas (ms)$')
    ax[0].set_ylabel('$y$')

    plt.tight_layout()

    t = np.arange(len(frame)) / 16
    ax[1].plot(t, clip_status)

    ax[1].set_title('Centrální klipování s 70 %')
    ax[1].set_xlabel('$Čas (ms)$')
    ax[1].set_ylabel('$y$')

    plt.tight_layout()

    x = np.arange(x, x+1, 1) 
    t = np.arange(len(result))
    ax[2].plot(t, result)
    

    lag = np.arange(lag, lag+1, 1)   #np.arange(50, 51, 1) = [50]
    ax[2].stem(x, lag, label = "Lag",linefmt="r-")
    ax[2].axvline(threshold, label = 'Práh', color='k')
    ax[2].legend()

    if status == "on":
        ax[2].set_title('Autokorelace - mask on')
    else:
        ax[2].set_title('Autokorelace - mask off')
    ax[2].set_xlabel('$Vzorky$')
    ax[2].set_ylabel('$y$')

    plt.tight_layout()

    t = np.arange(len(fre))
    ax[3].plot(t, fre, label="S rouškou")

    t = np.arange(len(fre2))
    ax[3].plot(t, fre2, label="Bez roušky")

    ax[3].legend()
    ax[3].set_title('Základní frekvence rámců')
    ax[3].set_xlabel('$rámce$')
    ax[3].set_ylabel('$f0$')

    plt.tight_layout()
    if save:
        pp.savefig()
        pp.close()

def central_clipping(frame_original, frame_original2):
    tmp_frame = np.copy(frame_original)
    abs_frame = abs(tmp_frame)
    maxx = np.amax(abs_frame)
    minn = -maxx
    minn = (0.7)*minn
    maxx = (0.7)*maxx
    length = len(tmp_frame)
    for i in range(0, length):
        if tmp_frame[i] > maxx:
            tmp_frame[i] = 1
        elif tmp_frame[i] < minn:
            tmp_frame[i] = -1
        else:
            tmp_frame[i] = 0

    tmp_frame2 = np.copy(frame_original2)
    abs_frame2 = abs(tmp_frame2)
    maxx = np.amax(abs_frame2)
    minn = -maxx
    minn = 0.7*minn
    maxx = 0.7*maxx
    length = len(tmp_frame2)
    for i in range(0, length):
        if tmp_frame2[i] > maxx:
            tmp_frame2[i] = 1
        elif tmp_frame2[i] < minn:
            tmp_frame2[i] = -1
        else:
            tmp_frame2[i] = 0

    return tmp_frame, tmp_frame2

def autocorrelation(frame):
    maxx = len(frame)
    k = 0
    result = []
    tmp = 0
    n = 0
    solved = 0
    while solved < maxx:
        while n < maxx:
            if n-k < 0:
                n += 1
                continue
            tmp += frame[n]*frame[n-k]
            n+= 1
        k += 1
        n = 0
        result.append(tmp)
        tmp = 0
        solved += 1

    #
    # mame signal z 6 prvku => 2 4 2 0 -2 -4
    # 
    # k = 0
    # x[n] =                 2 4 2 0 -2 -4
    # x[k+n] =               2 4 2 0 -2 -4
    # x[n]*x[k+n] =          4 16 4 0 4 16 => 32+12 = 44
    # 
    # za prazdny bod dosadim NULL (= 0)
    # k = 1
    # x[n] =                 2 4 2 0 -2 -4
    # x[k+n] =                 2 4 2  0 -2 -4
    # x[n]*x[k+n] =          0 8 8 0  4  8 => 28
    #
    # k = 2
    # x[n] =                 2 4 2 0 -2 -4
    # x[k+n] =                   2 4  2  0 -2 -4
    # x[n]*x[k+n] =          0 0 4 0 -4  0 => 0
    #
    # k = 3
    # x[n] =                 2 4 2 0 -2 -4
    # x[k+n] =                     2  4  2  0 -2 -4
    # x[n]*x[k+n] =          0 0 0 0 -8 -8 => -16
    #
    # k = 4
    # x[n] =                 2 4 2 0 -2 -4
    # x[k+n] =                        2  4  2  0 -2 -4
    # x[n]*x[k+n] =          0 0 0 0 -4 -16 => -20

    # Pro vypocet frekvence potrebuju znat pozici maxima na ose X, potom f = pocet_vzorku_za_sekundu / index
    Fs = 16000 # Signal je tvoren 16000 vzorky za sekundu
    index = np.arange(0, 1, 1)
    new = result
    threshold = int(16000/500)  # Prah 500hz, neuvazuju prvnich 32 vzorku, protoze jsou prilis zkreslene tim, ze se signal podoba sam sobe
    for counter in range (0, threshold):
        new = np.delete(new, index)
    lag = np.amax(new)
    length = len(new)
    index_max = 0
    tmp_max = 0
    for i in range(0, length): # najde index lagu (polohu na ose x)
        if new[i] > tmp_max:
            tmp_max = new[i]
            index_max = i
    x = int(index_max + 32) # prictu prah
    frequency = Fs / x

    return frequency, x, result, lag, threshold

def remove_invalid_values(freq, freq2):
    avg = np.average(freq)
    total = len(freq)
    i = 0
    while i < total:
        if freq[i] > (avg+80) or freq[i] < (avg-100):
            index = np.arange(i, i+1, 1)
            freq = np.delete(freq, index)
            freq2 = np.delete(freq2, index)
            total = len(freq)
            i -= 1
            avg = np.average(freq)
        i += 1

    avg = np.average(freq2)
    i = 0
    while i < total:
        if freq2[i] > (avg+80) or freq2[i] < (avg-100):
            index = np.arange(i, i+1, 1)
            freq = np.delete(freq, index)
            freq2 = np.delete(freq2, index)
            total = len(freq2)
            i -= 1
            avg = np.average(freq2)
        i += 1

    return freq, freq2

def get_frequency(frames_on, frames_off):
    length = len(frames_on)
    frequency_on = np.empty(99)
    frequency_off = np.empty(99)
    for x in range (0, length):
        tmp_frame = frames_on[x]
        tmp_frame2 = frames_off[x]
        clipped_on, clipped_off = central_clipping(tmp_frame, tmp_frame2)
        z = autocorrelation(clipped_on)
        y = autocorrelation(clipped_off)
        frequency_on[x] = z[0]
        frequency_off[x] = y[0]
    return frequency_on, frequency_off

def analyze_freq(freq_on, freq_off):
    mean = []
    mean_on = np.mean(freq_on)
    mean_off = np.mean(freq_off)
    mean.append(mean_on)
    mean.append(mean_off)

    deviation_on = np.std(freq_on)
    deviation_off = np.std(freq_off)
    deviation = [deviation_on, deviation_off]

    return mean, deviation

def my_count_dft(frames):
    tmp = np.copy(frames)   # udelej kopii at neprepisuju puvodni
    tmp = tmp * (2**15)

    results = []
    frame_results = [[]]
    length = len(tmp[0])
    length_max = len(tmp)
    tmp = np.pad(tmp, (0,1024-length), 'constant')
    length = len(tmp[0])
    for i in range(0, length_max):
        for x in range(0, length):
            tmp_result = 0
            for y in range(0, length):
                b = np.exp((-(1j) * np.pi * 2 * x * y) / length)
                if b == np.exp(-(1j) * np.pi): # Python nezvlada cisla kolem Eulerovy identity, proto fixuju rucne
                    b = -1+0j
                elif b == np.exp(1j * np.pi):
                    b = 1+0j
                elif b == (np.exp(1j * np.pi)) + 1:
                    b = 0+0j
                elif x == length/2:
                    b = np.real(b) + 0j
                tmp_result += tmp[i][y] * b
            results.append(tmp_result)  
        frame_results.append(results)
        results = []
    frame_results.pop(0)    # [] prazdny array z inicializace
    frame_results = np.array(frame_results) # zkonverttuj na numpy array


    return frame_results

def trim_abs_my_dft(frame_results):
    trimmed = trim_512(frame_results)
    trimmed = 10 * np.log10(np.abs(trimmed)**2)
    return trimmed

def trim_512(frame_results):
    x = 512       # chci 0-511, 1024-1535...
    length = len(frame_results)
    new_trimmed = np.copy(frame_results[0][:x])
    new_trimmed = np.expand_dims(new_trimmed, axis=0)
    for i in range(1, length):
        tmp = np.copy(frame_results[i][:x])
        tmp = np.expand_dims(tmp, axis=0)
        new_trimmed = np.append(new_trimmed, tmp, axis=0)
    return new_trimmed

def count_dft(frames):
    tmp = np.copy(frames)   # udelej kopii at neprepisuju puvodni
    tmp = tmp * (2**15)
    tmpp = np.fft.fft(tmp, 1024)   # transformuj
    new = tmpp                     # udelej array na bazi vysledku prvni transformace
    x = 512
    length = len(new)
    new_trimmed = np.copy(new[0][:x])
    new_trimmed = np.expand_dims(new_trimmed, axis=0)
    for i in range(1, length):
        tmp = np.copy(new[i][:x])
        tmp = np.expand_dims(tmp, axis=0)
        new_trimmed = np.append(new_trimmed, tmp, axis=0)
    return new_trimmed, new

def print_spectrums(transformed,  transformed_off):
    global save
    if save:
        pp = PdfPages('spectrums.pdf')
    transformed = 10 * np.log10(np.abs(transformed)**2)         # pro zobrazeni zlogaritmuju
    transformed_off = 10 * np.log10(np.abs(transformed_off)**2)

    plt.figure(figsize=(9,3))
    plt.imshow(transformed.T, extent=[0,1, 0, 8000], aspect='auto', origin='lower')
    plt.gca().set_title('Spektrogram s rouškou')
    plt.gca().set_xlabel('Čas [s]')
    plt.gca().set_ylabel('Frekvence [Hz]')
    cbar = plt.colorbar()
    cbar.set_label('Spektralní hustota výkonu [dB]', rotation=270, labelpad=15)
    plt.tight_layout()
    if save:
        pp.savefig()

    plt.figure(figsize=(9,3))
    plt.imshow(transformed_off.T, extent=[0,1, 0, 8000], aspect='auto', origin='lower')
    plt.gca().set_title('Spektrogram bez roušky')
    plt.gca().set_xlabel('Čas [s]')
    plt.gca().set_ylabel('Frekvence [Hz]')
    cbar = plt.colorbar()
    cbar.set_label('Spektralní hustota výkonu [dB]', rotation=270, labelpad=15)
    plt.tight_layout()
    if save:
        pp.savefig()
    if save:
        pp.close()

def freq_characteristic(complex_frames_on, complex_frames_off):
    global save
    # H(e^jw) = B(e^jw) / A(e^jw)  
    x = complex_frames_on
    y = complex_frames_off
    tmp = [[]]
    charc = []
    maxx = len(x)
    for r in range(0, maxx):
        frequencies, characteristic = freqz(b=x[r], a=y[r], worN=1024)
        tmp.append(characteristic)
    tmp.pop(0)
    tmp = np.array(tmp) # 2D array, pro kazdy ramec frekv. charakteristika
    frame_characteristics = tmp

    #absolutni hodnota, prumer
    tmp = []
    maxx2 = len(frame_characteristics[0])
    result_array = []
    for i in range(0, maxx2):
        for k in range(0, maxx):
            tmp.append(frame_characteristics[k][i])
        tmp = np.abs(tmp)
        x = np.mean(tmp)
        result_array.append(x)
        tmp = []
    
    result_array = np.array(result_array)
    print_arr = np.copy(result_array[:int(len(result_array)/2)])
    f = np.arange(len(print_arr))
    tone = "freq_characteristic_mask.pdf"
    if save:
        pp = PdfPages(tone)

    plt.figure(figsize=(15,4)) 
    plt.plot(f, print_arr)

    plt.gca().set_title('Frekvenční charakteristika filtru')
    plt.gca().set_xlabel('$n$')
    plt.gca().set_ylabel('$y$')

    plt.tight_layout()
    if save:
        pp.savefig()  
        pp.close()

    return result_array

def impulse_response(freq_characteristic):
    global save
    freq_characteristic = np.fft.ifft(freq_characteristic)
    freq_characteristic = np.copy(freq_characteristic[:int(len(freq_characteristic)/2)])
    a = [1.0]
    b = np.real(freq_characteristic)    # chci jen realnou cast
    N_imp = len(freq_characteristic)

    imp = np.zeros(N_imp) # jednotkovy impuls
    imp[0] = 1
    h = lfilter(b, a, imp)
    tone = "impulse_response.pdf"
    if save:
        pp = PdfPages(tone)

    plt.figure(figsize=(15,4))
    plt.plot(np.arange(N_imp), h)
    plt.gca().set_xlabel('$n$')
    plt.gca().set_title('Impulsní odezva $h[n]$')

    plt.tight_layout()
    if save:
        pp.savefig()
        pp.close()

    return b, freq_characteristic

def my_count_idft(frame):
    tmp = np.copy(frame)   # udelej kopii at neprepisuju puvodni
    results = []
    length_max = len(tmp)
    for x in range(0, length_max):
        tmp_result = 0
        for y in range(0, length_max):
            b = np.exp(1j * ((np.pi * 2) / length_max) * x * y)
            if b == np.exp(-(1j) * np.pi): # python nefunguje jak by mel, proto napravim tuto hodnotu rucne
                b = -1+0j
            elif b == np.exp(1j * np.pi):
                b = 1+0j
            elif b == (np.exp(1j * np.pi) + 1):
                b = 0
            elif x == length/2:
                b = np.real(b) + 0j
            tmp_result += tmp[y] * b
        tmp_result = tmp_result * (1/length_max)
        results.append(tmp_result)
    results = np.array(results) # zkonverttuj na numpy array
    return results

def get_simulated_filter(mask_filter):
    global save
    a = [1.0]
    b = mask_filter 
    x, fs = sf.read('audio/maskoff_sentence.wav')

    result = lfilter(b, a, x)
    sf.write('audio/sim_maskon_sentence.wav', result, fs)   # vytvor nasimulovanou rousku

    q, fs = sf.read('audio/maskoff_tone.wav')
    result_tone = lfilter(b, a, q)
    sf.write('audio/sim_maskon_tone.wav', result_tone, fs)   # vytvor nasimulovanou rousku na tonu


    z, fs = sf.read('audio/maskon_sentence.wav')
    plt.figure(figsize=(18,9))
    t = np.arange(x.size) / fs  
    x = x * 2**15
    result = result * 2**15
    z = z * 2**15


    if save:
        pp = PdfPages('masks_sentences_simulated.pdf')

    plt.figure(figsize=(18,9)) 
    plt.plot(t, x)

    plt.gca().set_title('Mask Off')
    plt.gca().set_xlabel('$Čas (s)$')
    plt.gca().set_ylabel('$Hz$')

    plt.tight_layout()
    if save:
        pp.savefig()

    plt.figure(figsize=(18,9)) 
    plt.plot(t, z)

    plt.gca().set_title('Mask On')
    plt.gca().set_xlabel('$Čas (s)$')
    plt.gca().set_ylabel('$Hz$')

    plt.tight_layout()
    if save:
        pp.savefig()

    plt.figure(figsize=(18,9)) 
    plt.plot(t, result)

    plt.gca().set_title('Mask On - Simulated')
    plt.gca().set_xlabel('$Čas (s)$')
    plt.gca().set_ylabel('$Hz$')

    plt.tight_layout()
    if save:
        pp.savefig()

    plt.figure(figsize=(18,9))
    sim, = plt.plot(t, result)
    sim.set_label("Mask On - Simulated")
    on, = plt.plot(t, x)
    on.set_label("Mask Off")
    off, = plt.plot(t, z)
    off.set_label("Mask On")

    plt.gca().set_title('On/Off/Simulated')
    plt.gca().set_xlabel('$Čas (s)$')
    plt.gca().set_ylabel('$Frekvence (Hz)$')
    plt.legend(fontsize=15)

    plt.tight_layout()
    if save:
        pp.savefig()

    plt.figure(figsize=(18,9))
    sim, = plt.plot(t, result)
    sim.set_label("Mask On - Simulated")
    off, = plt.plot(t, x)
    off.set_label("Mask Off")

    plt.gca().set_title('Off/Simulated On')
    plt.gca().set_xlabel('$Čas (s)$')
    plt.gca().set_ylabel('$Frekvence (Hz)$')
    plt.legend(fontsize=15)

    plt.tight_layout()
    if save:
        pp.savefig()
        pp.close()
    else:
        plt.show()

def main():
    arg_parse()
    # tisknuti 1 sec segmentu, kombinace 2 ramcu
    maskon = "audio/maskon_tone.wav"
    maskoff = "audio/maskoff_tone.wav"
    print_combined()
    print_1sec_segments()

    # ustredni vterinu dlouhe signaly, vytvor ramce (99, posledni zahod)
    # proved clipping, autokorelaci a ziskej zakladni frekvenci ramcu
    frames_off = centralize(maskoff)
    frames_on = centralize(maskon)
    tmp_frame = frames_on[8]
    tmp_frame2 = frames_off[8]
    print_2_frames(frames_on[8], frames_off[8])

    clipped_on, clipped_off = central_clipping(tmp_frame, tmp_frame2)
    frequency, x, result, lag, threshold = autocorrelation(clipped_on)
    frequency_2, x_2, result_2, lag_2, threshold_2 = autocorrelation(clipped_off)
    frequency_on, frequency_off = get_frequency(frames_on, frames_off)
    
    # V pripade, ze se nepovedlo vytvorit lag (nepovedlo se udrzet ton), zahod ten ramec
    # spolecne s nim i ramec z druhe casti (chyba v ramci s rouskou, zahod i ten bez rousky - musi byt stejne velke)
    trimmed_on, trimmed_off = remove_invalid_values(frequency_on, frequency_off)

    mean, deviation = analyze_freq(trimmed_on, trimmed_off)
    print("Stredni hodnota mask_on, mask_off:", mean, "\nRozptyl mask_on, mask_off:", deviation)

    # vytiskni ramce s clippingem, autokorelaci a zakladni frekvenci
    print_frame("on", frames_on[8], maskon, clipped_on, x, result, lag, threshold, trimmed_on, trimmed_off)
    print_frame("off", frames_off[8], maskoff, clipped_off, x_2, result_2, lag_2, threshold_2, trimmed_on, trimmed_off)

    #transformace
    transformed_on, transformed_on_original = count_dft(frames_on)
    transformed_off, transformed_off_original = count_dft(frames_off)
    print_spectrums(transformed_on, transformed_off)
    #transformed_on_my = my_count_dft(frames_on)
    #transformed_on_my = trim_abs_my_dft(transformed_on_my)
    #for i in range(0, len(transformed_on[0])):
    #    print(transformed_on_original[1][i])
    #    print(transformed_on_my[1][i])
    
    
    frequency_characteristic = freq_characteristic(transformed_on_original, transformed_off_original)
    #frequency_characteristic = freq_characteristic(transformed_on, transformed_off)
    mask_filter, orig = impulse_response(frequency_characteristic)

    #mask_filter_my = my_count_idft(frequency_characteristic)
    #for i in range(0, len(mask_filter_my)):
    #    print(orig[i])
    #    print(mask_filter_my[i])

    get_simulated_filter(mask_filter)

main()
