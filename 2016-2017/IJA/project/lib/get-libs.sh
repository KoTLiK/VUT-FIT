#!/bin/bash

cd lib
wget "http://www.stud.fit.vutbr.cz/~xaugus09/images.zip"
rm -rf images
mkdir images
unzip images.zip -d images
rm -fv images.zip
cd ..
