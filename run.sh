#!/bin/bash
if [ "$#" -ne 1 ]; then
    if [ "$#" -ne 2 ]; then
	echo "Illegal number of parameters."
	echo "Usage: $0 <image name> [negate]"
	exit
    fi
fi

picture=$1
binarized="$picture.binarized"
negated="$picture.negated"

echo "Picture: $picture"
echo "Image binarization..."
convert "$picture" -colorspace gray +dither -colors 2 -normalize "$binarized" || exit
echo "Done."

if [ "$2" == "negate" ]; then
    echo "Image negation..."
    convert -negate $binarized $negated || exit
    echo "Done."
    echo "Percent calculation"
    ./percent_cpp $negated
    rm "$negated"
else
    ./percent_cpp $binarized
fi

rm "$binarized"
