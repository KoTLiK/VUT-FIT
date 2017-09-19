NAME=proj3

all: clean $(NAME).pdf

# dvips -t a4 $(NAME).dvi

$(NAME).pdf: $(NAME).tex
	latex $(NAME).tex
	latex $(NAME).tex
	dvips $(NAME).dvi
	ps2pdf -sPAPERSIZE=a4 $(NAME).ps

.PHONY: zip clean

zip:
	zip xaugus09-fit.zip $(NAME).tex Makefile *.eps *.sty

clean:
	rm -fv $(NAME).dvi $(NAME).log $(NAME).aux $(NAME).ps $(NAME).pdf *.zip
