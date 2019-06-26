#!/bin/bash

cd camUpdate
rm update.md5sum
md5sum update* debian.img.gz > update.md5sum
