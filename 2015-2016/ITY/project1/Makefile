NAME=proj1

all: clean do

do: $(NAME).tex
	latex $(NAME).tex
	dvips $(NAME).dvi
	ps2pdf $(NAME).ps

zip:
	zip xaugus09-fit.zip $(NAME).tex Makefile

clean:
	rm -fv $(NAME).dvi $(NAME).log $(NAME).aux $(NAME).ps $(NAME).pdf *.zip
