#
# Makefile - ITY projekt 4
# Autor: 	Milan Augustin, xaugus09
#

NAME=proj4

all: clean $(NAME).pdf

# dvips -t a4 $(NAME).dvi
# ps2pdf -sPAPERSIZE=a4 $(NAME).ps

$(NAME).pdf: $(NAME).tex
	latex $(NAME).tex
	bibtex $(NAME).aux
	latex $(NAME).tex
	latex $(NAME).tex
	dvips -t a4 $(NAME).dvi
	ps2pdf $(NAME).ps

.PHONY: zip clean

zip:
	zip xaugus09-fit.zip $(NAME).tex Makefile czplain.bst zdroje.bib

clean:
	rm -fv $(NAME).dvi $(NAME).log $(NAME).aux $(NAME).ps $(NAME).pdf *.zip $(NAME).bbl $(NAME).blg
