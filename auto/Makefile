#
# Makefile: mGEff Daily build makefile
# Copyright (C) 2010 FMSoft
# Author: humingming
#


# get the inc files
INC_FILES=$(wildcard *.inc)
INC_TOTAL=$(words $(INC_FILES))


all:
	@for inc in $(INC_FILES); do \
	  ./build.sh $$inc; \
	done; \


.PHONY: clean
clean:
	-rm -rf $(FOLDER)
	@echo "tmp file is clean."


