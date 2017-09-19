#
# Makefile - ITY projekt 5
# Autor: 	Milan Augustin, xaugus09
#

NAME=proj5

all: $(NAME)

# latex $(NAME).tex
# bibtex $(NAME).aux
# latex $(NAME).tex
# dvips -t a4 $(NAME).dvi
# ps2pdf -sPAPERSIZE=a4 $(NAME).ps

$(NAME): $(NAME).tex
	pdflatex $(NAME).tex
#	latex $(NAME).tex
#	dvips $(NAME).dvi
#	ps2pdf $(NAME).ps

.PHONY: zip clean

zip:
	zip xaugus09-fit.zip $(NAME).tex *.jpg *.docx Makefile

clean:
	rm -fv *.aux *.dvi *.log *.nav *.out *.pdf *.ps *.snm *.toc *.zip
