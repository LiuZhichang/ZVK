#!/bin/bash

curPath=$(pwd)

for shader in $(ls ./) ; do
   subfix="${shader##*.}"
   if [ $subfix == "vert" -o $subfix == "frag" ]; then
	   printf "compile shader file: %s >\n" $shader 
	   printf "\toutput file: %s\n\n" "${shader%.*}.spv" 
	   glslangValidator -V $curPath/$shader
   fi
done
