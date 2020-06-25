#!/bin/sh

# test the various restartable loaders

# webp and ppm use streams, but they mmap the input, so you can't close() the
# fd on minimise

# set -x
set -e

. ./variables.sh

if test_supported jpegload_source; then
	./test_descriptors $image
fi

if test_supported pngload_source; then
	./test_descriptors $test_images/sample.png
fi

if test_supported tiffload_source; then
	./test_descriptors $test_images/sample.tif
fi

if test_supported radload_source; then
	./test_descriptors $test_images/sample.hdr
fi

if test_supported svgload_source; then
	./test_descriptors $test_images/logo.svg
fi

if test_supported matrixload_source; then
	$vips copy $image $tmp/sample.mat
	./test_descriptors $tmp/sample.mat
fi

if test_supported csvload_source; then
	$vips copy $image $tmp/sample.csv
	./test_descriptors $tmp/sample.csv
fi

if test_supported vipsload_source; then
	$vips copy $image $tmp/sample.v
	./test_descriptors $tmp/sample.v
fi
