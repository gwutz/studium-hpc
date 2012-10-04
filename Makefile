# Initial Makefile lend from Christian Hergert
#

NAME := mitschrift

PDFLATEX := $(shell which pdflatex)
BUILDDIR := build
PDFTARGET := $(BUILDDIR)/$(NAME).pdf

all: $(PDFTARGET)

TEX_FILES :=
TEX_FILES += src/mitschrift.tex
TEX_FILES += src/preambel.tex
TEX_FILES += src/einfuehrung.tex

FIGURE_FILES :=
EXERCISE_FILES :=

TEX_DEPS :=
TEX_DEPS += $(TEX_FILES)

$(PDFTARGET): $(TEX_DEPS)
	@mkdir -p $(BUILDDIR)
	@echo "  [COPY]     tex"
	@cp -a src/*.tex $(BUILDDIR)
	@echo "  [COPY]     images"
	@cp -a data/*.png $(BUILDDIR)
	@echo "  [TOC]      $(NAME)"
	@TEXINPUTS=:$(BUILDDIR) $(PDFLATEX) -halt-on-error -output-directory $(BUILDDIR) -output-format pdf $(NAME).tex >/dev/null
	@echo "  [PDFLATEX] $(NAME).pdf"
	@rm $(BUILDDIR)/$(NAME).pdf
	@TEXINPUTS=:$(BUILDDIR) $(PDFLATEX) -halt-on-error -output-directory $(BUILDDIR) -output-format pdf $(NAME).tex >/dev/null

view: $(PDFTARGET)
	evince $(PDFTARGET)

clean:
	@echo "  [CLEAN]    build"
	@rm -rf $(BUILDDIR)
