proj3.pdf: proj3.ps
	ps2pdf proj3.ps

proj3.ps: proj3.dvi
	dvips -sPAPERSIZE=a4 proj3.dvi

proj3.dvi: proj3.tex
	latex proj3.tex
	latex proj3.tex
