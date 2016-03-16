#!/bin/bash

../../../depends/bin/protoc --cpp_out=./sdk-cpp/	-I=../../proto/		../../proto/*.proto
