NAME=proj2

all: clean $(NAME).pdf

$(NAME).pdf: $(NAME).tex
	latex $(NAME).tex
	latex $(NAME).tex
	dvips -t a4 $(NAME).dvi
	ps2pdf $(NAME).ps

zip:
	zip xaugus09-fit.zip $(NAME).tex Makefile

clean:
	rm -fv $(NAME).dvi $(NAME).log $(NAME).aux $(NAME).ps $(NAME).pdf *.zip
